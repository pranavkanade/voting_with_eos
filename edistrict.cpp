#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>

using namespace std;
using namespace eosio;

class [[eosio::contract]] edistrict : public contract {
    public:
        using contract::contract;

        edistrict(name receiver, name code, datastream<const char*> ds) :
            contract(receiver, code, ds) {}

        [[eosio::action]]
        void upsert(
            name user,
            uint64_t age,
            string party_name,
            uint64_t party_code
        ) {
            // TODO: The user should be election commisioner

            require_auth(user);
            candidates_index candidates(_code, _code.value);

            auto iterator = candidates.find(user.value);

            if (iterator == candidates.end()) {
                candidates.emplace(user, [&]( auto& row ) {
                    row.key = user;
                    row.age = age;
                    row.party_name = party_name;
                    row.party_code = party_code;
                });
            }
            else {
                candidates.modify(
                    user,
                    [&]( auto& row ) {
                        row.key = user;
                        row.age = age;
                        row.party_name = party_name;
                        row.party_code = party_code;
                    }
                );
            }
        }



    private:
        // we need table to store the candidates over here
        struct [[eosio::table]] candidate {
            name key;
            uint64_t age;
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