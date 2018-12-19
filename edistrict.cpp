#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>

using namespace std;
using namespace eosio;

class [[eosio::contract]] edistrict : public contract {
    public:
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
        > candidate_index;
}