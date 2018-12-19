#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>

using namespace std;
using namespace eosio;

class [[eosio::contract]] ecandidates : public contract {
    public:
        using contract::contract;

        ecandidates(name receiver, name code, datastream<const char*> ds) :
            contract(receiver, code, ds) {}

        [[eosio::action]]
        void upsert(
            name user,
            uint64_t uid,
            string party_name,
            uint64_t party_code
        ) {
            require_auth(name("ecommissioner"));
            candidates_index candidates(_code, _code.value);

            auto iterator = candidates.find(user.value);

            if (iterator == candidates.end()) {
                candidates.emplace("ecommissioner"_n, [&]( auto& row ) {
                    row.key = user;
                    row.uid = uid;
                    row.party_name = party_name;
                    row.party_code = party_code;
                });
                insert_candidate_record(user, party_code);
            }
            else {
                candidates.modify(
                    iterator
                    "ecommissioner"_n,
                    [&]( auto& row ) {
                        row.key = user;
                        row.uid = uid;
                        row.party_name = party_name;
                        row.party_code = party_code;
                    }
                );
            }
        }



    private:
        void insert_candidate_record(
            name user,
            uint64_t party_code
        ) {
            action insert = action (
                permission_level{get_self(), "active"_n},
                "erecords"_n,
                "insert"_n,
                make_tuple(user, party_code)
            );

            insert.send();
        }

        // we need table to store the candidates over here
        struct [[eosio::table]] candidate {
            name key;
            uint64_t uid;
            string party_name;
            uint64_t party_code;

            uint64_t primary_key() { return key.value; }
            uint64_t get_secondary_key() { return party_code; }
        }

        typedef multi_index<
            "candidates"_n,
            candidate,
            indexed_by<
                "bypartycode"_n,
                const_mem_fun<
                    candidate,
                    uint64_t,
                    &candidate::get_secondary_key
                >
            >
        > candidates_index;
}