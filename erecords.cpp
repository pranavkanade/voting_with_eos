#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>

using namespace std;
using namespace eosio;

class [[eosio::contract]] erecords : public contract {
    public:
        using contract::contract;

        erecords(
            name receiver,
            name code,
            datastream<const char*> ds
        ): contract(receiver, code, ds) {}

        // upsert
        [[eosio::action]]
        void insert(
            name user,
            uint64_t party_code
        ) {
            require_auth( name("ecandidates") );
            // Create a handle to the table below
            vote_record_index vote_record(_code, _code.value);
            // Find if the user already exists in the table
            auto iterator = vote_record.find(user.value);

            eosio_assert(iterator != vote_record.end(), "Candidate already exist");

            voters.emplace(
                "ecandidates"_n,
                [&](auto& row) {
                    row.key = user;
                    row.party_code = party_code;
                    row.vote_count = 0;
                }
            );
        }

        [[eosio::action]]
        void vote(
            name user,
            uint64_t party_code
        ) {
            // TODO: This function should be called out every time a person
            // votes. Only election contract account should be allowed to
            // call out this function using notification mechanism.
            require_auth(user);
            // Create a handle to the table below
            vote_record_index vote_record(_code, _code.value);
            // Find if the user already exists in the table
            auto iterator = vote_record.find(user.value);

            eosio_assert(iterator == vote_record.end(), "Candidate does not exist");

            voters.modify(
                iterator
                user,
                [&](auto& row) {
                    row.key = user;
                    row.party_code = party_code;
                    row.vote_count += 1;
                }
            );
        }

    private:
        // Create a table to hold the info regarding all the voters
        struct [[eosio::table]] voterecord {
            name key;
            uint64_t party_code;
            uint64_t vote_count = 0;

            uint64_t primary_key() { return key.value; }
            uint64_t get_secondary_key() { return vote_count; }
        }

        typedef multi_index<
            "voterecords"_n,
            voterecord,
            indexed_by<
                "byvotecount"_n,
                const_mem_fun<
                    voterecord,
                    uint64_t,
                    &voterecord::get_secondary_key
                >
            >
        > vote_record_index;
}
