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

        [[eosio::action]]
        void register_as_voter(
            name user,
            uint64_t age,
            uint64_t aadhar_no;
            uint64_t uid
        ) {
            requrie_auth(user);

            add_voters(
                user,
                age,
                aadhar_no,
                uid
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

        void add_voters(
            name user,
            uint64_t age,
            uint64_t aadhar_no;
            uint64_t uid
        ) {
            action add = action (
                permission_level{get_self(), "active"_n},
                "evoters"_n,
                "upsert"_n,
                make_tuple(user, age, aadhar_no, uid)
            );

            add.send()
        }
}