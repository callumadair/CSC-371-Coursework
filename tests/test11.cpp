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
        "The database and action program arguments can be parsed correctly such that a file can be opened, read and data updated.") {

    const std::string filePath = "./tests/testdatabasethirdalt.json";

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
}
