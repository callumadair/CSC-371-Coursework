//
// Created by Callum Adair on 23/12/22.
//

#include "../src/lib_catch.hpp"

#include <fstream>
#include <iostream>
#include <string>

#include "../src/lib_cxxopts.hpp"
#include "../src/lib_cxxopts_argv.hpp"

#include "../src/371pass.h"


SCENARIO(
        "The database and action program arguments can be parsed correctly such that a file can be opened, data added and correct json parsing behaviours confirmed.") {

    const std::string filePath = "./tests/testdatabasethirdalt.json";

    auto fileExists = [](const std::string &path) {
        return std::ifstream(path).is_open();
    };

    GIVEN("a valid path to a reset JSON database file") {

        REQUIRE(fileExists(filePath));

        const std::string testCatIdent = "Bank Accounts";
        const std::string testItemIdent = "Nationwide";
        const std::string testEntryKey = "Sort Code";
        const std::string testEntryValue = "12-34-56";

        Wallet wObj{};
        wObj.newCategory(testCatIdent);
        wObj.save(filePath);

        Wallet wObj2{};
        wObj2.load(filePath);
        Category &testCat = wObj2.getCategory(testCatIdent);

        REQUIRE(testCat.str() == "null");
    }
}
