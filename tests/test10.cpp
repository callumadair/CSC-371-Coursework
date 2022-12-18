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
}