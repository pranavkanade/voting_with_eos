#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>

using namespace std;
using namespace eosio;

class [[eosio::contract]] election : public contract {
    public:
        using contract::contract;

        election(name receiver, name code, datastream<const char*> ds) :
            contract(receiver, code, ds) {}

        [[eosio::action]]
        void cast_my_vote(
            name user,
            name candidate_name,
            uint64_t party_code
        ) {
            requrie_auth(user);

            cast_vote(
                candidate_name,
                party_code
            );
        }
    private:
        void cast_vote(
            name candidate,
            uint64_t party_code
        ) {
            action add = action (
                permission_level{get_self(), "active"_n},
                "erecords"_n,
                "vote"_n,
                make_tuple(candidate, party_code)
            );

            add.send();
        }
}