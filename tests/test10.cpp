//
// Created by Callum Adair on 06/12/2022.
//

#include "../src/lib_catch.hpp"

#include <fstream>
#include <iostream>
#include <string>

#include "../src/lib_cxxopts.hpp"
#include "../src/lib_cxxopts_argv.hpp"

#include "../src/371pass.h"


SCENARIO(
        "The database and action program arguments can be parsed correctly such that a file can be opened, read and data updated."
) {
    const std::string filePath = "./tests/testdatabasesecondalt.json";


    auto fileExists = [](const std::string &path) {
        return std::ifstream(path).is_open();
    };

    auto writeFileContents = [](const std::string &path,
                                const std::string &contents) {
        // Not a robust way to do this, but here it doesn't matter so much, if it
        // goes wrong we'll fail the test anyway…
        std::ofstream f{path};
        f << contents;
    };

    GIVEN("a valid path to a reset database JSON file") {

        REQUIRE(fileExists(filePath));
        REQUIRE_NOTHROW(writeFileContents(
                filePath, "{\"Bank Accounts\":{\"Starling\":{\"Account "
                          "Number\":\"12345678\",\"Name\":\"Mr John Doe\",\"Sort "
                          "Code\":\"12-34-56\"}},\"Websites\":{\"Facebook\":{"
                          "\"password\":\"pass1234fb\",\"url\":\"https://"
                          "www.facebook.com/"
                          "\",\"username\":\"example@gmail.com\"},\"Google\":{"
                          "\"password\":\"pass1234\",\"url\":\"https://www.google.com/"
                          "\",\"username\":\"example@gmail.com\"},\"Twitter\":{"
                          "\"password\":\"r43rfsffdsfdsf\",\"url\":\"https://"
                          "www.twitter.com/\",\"username\":\"example@gmail.com\"}}}"));

        const std::string oldTestCategory = "Bank Accounts";
        const std::string oldTestItem = "Starling";
        const std::string oldTestEntryKey = "Account Number";
        const std::string oldTestEntryValue = "12345678";

        const std::string newTestCategory = "Current Accounts";
        const std::string newTestItem = "Santander";
        const std::string newTestEntryKey = "Account";
        const std::string newTestEntryValue = "87654321";

        WHEN("the db program argument is '" + filePath +
             "', the action program argument is 'update'") {

            Wallet wObj1{};
            wObj1.load(filePath);

            REQUIRE_NOTHROW(wObj1.getCategory(oldTestCategory)
                                    .getItem(oldTestItem)
                                    .getEntry(oldTestEntryKey));

            REQUIRE(wObj1.getCategory(oldTestCategory)
                            .getItem(oldTestItem)
                            .getEntry(oldTestEntryKey) == oldTestEntryValue);

            REQUIRE_THROWS_AS(wObj1.getCategory(newTestCategory)
                                      .getItem(newTestItem)
                                      .getEntry(newTestEntryKey),
                              std::out_of_range);

            AND_WHEN("and the category program argument is '" + oldTestCategory + ':' + newTestCategory +
                     "', the item program argument is '" + oldTestItem + ':' + newTestItem +
                     "', and the entry program argument is '" + oldTestEntryKey + ':' + newTestEntryKey + "," +
                     newTestEntryValue + "'") {

                Argv argvObj({"test", "--db", filePath.c_str(), "--action", "update",
                              "--category", (oldTestCategory + ":" + newTestCategory).c_str(),
                              "--item", (oldTestItem + ":" + newTestItem).c_str(),
                              "--entry", (oldTestEntryKey + ":" + newTestEntryKey + "," + newTestEntryValue).c_str()});

                auto **argv = argvObj.argv();
                auto argc = argvObj.argc();

                THEN("no exception is thrown") {

                    REQUIRE_NOTHROW(App::run(argc, argv));

                    AND_WHEN("loading the saved file into a new Wallet object will work") {

                        Wallet wObj2{};
                        REQUIRE(wObj2.empty());
                        REQUIRE_NOTHROW(wObj2.load(filePath));

                        THEN("the new Wallet will not contain the old entry") {

                            REQUIRE_THROWS_AS(wObj2.getCategory(oldTestCategory)
                                                      .getItem(oldTestItem)
                                                      .getEntry(oldTestEntryKey),
                                              std::out_of_range);

                        } // THEN

                        THEN("the new wallet will contain the new entry") {
                            REQUIRE_NOTHROW(wObj2.getCategory(newTestCategory)
                                                    .getItem(newTestItem)
                                                    .getEntry(newTestEntryKey));

                            REQUIRE(wObj2.getCategory(newTestCategory)
                                            .getItem(newTestItem)
                                            .getEntry(newTestEntryKey) == newTestEntryValue);
                        }
                    }
                }
            }
        }
    }
}