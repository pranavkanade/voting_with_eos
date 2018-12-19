#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>

using namespace std;
using namespace eosio;

class [[eosio::contract]] evoters : public contract {
    public:
        using contract::contract;

        evoters(
            name receiver,
            name code,
            datastream<const char*> ds
        ): contract(receiver, code, ds) {}

        // upsert
        // TODO: The author of this action should be election commisiner account
        [[eosio::action]]
        void upsert(
            name user,
            uint64_t age,
            uint64_t aadhar_no;
            uint64_t uid
        ) {
            // check if correct user has called out the action
            require_auth(user);
            // Create a handle to the table below
            voters_index voters(_code, _code.value);
            // Find if the user already exists in the table
            auto iterator = voters.find(user.value);

            if (iterator == voters.end()) {
                // table does not have user info
                voters.emplace(
                    user,
                    [&](auto& row) {
                        row.key = user;
                        row.age = age;
                        row.aadhar_no = aadhar_no;
                        row.uid = uid;
                    }
                );
            }
            else {
                // table does have the user info
                voters.modify(
                    user,
                    [&](auto& row) {
                        row.age = age;
                        row.aadhar_no = aadhar_no;
                        row.uid = uid;
                    }
                );
            }
        }

        // TODO: This should be only done by the election commisioner account
        [[eosio::action]]
        void erase(
            name user
        ) {
            require_auth(user);
            voters_index voters(_code, _code.value);

            auto iterator = voters.find(user.value);
            eosio_assert(iterator != voters.end(), "Record does not exist");

            voters.erase(iterator);
        }
    private:
        // Create a table to hold the info regarding all the voters
        struct [[eosio::table]] voter {
            name key;
            uint64_t age;
            uint64_t aadhar_no;
            uint64_t uid;       // other than aadhar

            uint64_t primary_key() { return key.value; }
        }

        typedef multi_index<
            "voters"_n,
            voter
        > voters_index;
}
