#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>

using namespace std;
using namespace eosio;

class [[eosio::contract]] ecommissioner : public contract {
    public:
        using contract::contract;

        ecommissioner(name receiver, name code, datastream<const char*> ds) :
            contract(receiver, code, ds) {}


        [[eosio::action]]
        void register_as_candidate(
            name user,
            uint64_t uid,
            uint64_t party_code,
            string party_name
        ) {
            requrie_auth(user);

            add_candidates(
                user,
                uid,
                party_code,
                party_name
            );
        }
    private:
        void add_candidates(
            name user,
            uint64_t uid,
            uint64_t party_code,
            string party_name
        ) {
            action add = action (
                permission_level{get_self(), "active"_n},
                "ecandidates"_n,
                "upsert"_n,
                make_tuple(user, uid, party_name, party_code)
            );

            add.send()
        }

        // TODO: Add another notification action for the evoters
}