// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 851784
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>

#include "371pass.h"
#include "lib_cxxopts.hpp"
#include "wallet.h"

// TODO Complete this function. You have been provided some skeleton code which
//  retrieves the database file name from cxxopts.
//  1. Load the database file by calling load() on a Wallet object
//  2. Parse the 'action' argument to decide what action should be taken (
//     create, read, update, or delete). Read is the easiest to implement, and
//     update is the hardest. The details of how these arguments work is in the
//     coursework specification.
//  4. Save the updated Wallet object to the JSON file if there have been
//     changes (calling save() on the Wallet object).
//
/* Some commented out code has been provided. Using some of this will be
 demonstrated in the coursework video on Canvas. Remember, this coursework is
 meant to be challenging and testing your understanding of programming in C++.
 Part of the challenge is figuring things out on your own. That is a major
 part of software development.

 Example:
  int main(int argc, char *argv[]) { return App::run(argc, argv); }*/
int App::run(int argc, char *argv[]) {
    auto options = App::cxxoptsSetup();
    auto args = options.parse(argc, argv);
    args.arguments().size();

    // Print the help usage if requested
    if (args.count("help")) {
        std::cout << options.help() << '\n';
        return 0;
    }

    // Open the database and construct the Wallet
    const std::string db = args["db"].as<std::string>();
    Wallet wObj{};
    wObj.load(db);


    try {
        const Action a = parseActionArgument(args);
        switch (a) {
            case Action::CREATE:
                if (args["category"].count()) {
                    if (args["item"].count()) {
                        if (args["entry"].count()) {

                            std::string entry_input = args["entry"].as<std::string>();
                            std::string entry_delimiter = ",";

                            if (entry_input.find(entry_delimiter)) {
                                std::string entry_identifier = entry_input.substr(0, entry_input.find(entry_delimiter));
                                std::string entry_value = entry_input.substr(entry_input.find(entry_delimiter) + 1);
                                wObj.newCategory(args["category"].as<std::string>()).newItem(
                                        args["item"].as<std::string>()).addEntry(entry_identifier, entry_value);
                            } else {
                                wObj.newCategory(args["category"].as<std::string>()).newItem(
                                        args["item"].as<std::string>()).addEntry(entry_input, "");
                            }
                        } else {
                            wObj.newCategory(args["category"].as<std::string>()).newItem(
                                    args["item"].as<std::string>());
                        }

                    } else if (args["entry"].count()) {
                        throw std::out_of_range("Error: missing item argument(s).");

                    } else {
                        wObj.newCategory(args["category"].as<std::string>());

                    }
                } else if (args["item"].count() || args["entry"].count()) {
                    throw std::out_of_range("Error: missing category argument(s).");

                } else {
                    throw std::out_of_range("Error: missing category, item or entry argument(s).");
                }
                break;

            case Action::READ:
                if (args["category"].count()) {
                    if (args["item"].count()) {
                        if (args["entry"].count()) {
                            std::string value = wObj.getCategory(args["category"].as<std::string>())
                                    .getItem(args["item"].as<std::string>())
                                    .getEntry(args["entry"].as<std::string>());
                            std::cout << value;
                        } else {
                            std::cout << getJSON(wObj, args["category"].as<std::string>(),
                                                 args["item"].as<std::string>());
                        }
                    } else if (args["entry"].count()) {
                        throw std::out_of_range("Error: missing item argument(s).");
                    } else {
                        std::cout << getJSON(wObj, args["category"].as<std::string>());
                    }
                } else if (args["item"].count() || args["entry"].count()) {
                    throw std::out_of_range("Error: missing category argument(s).");
                } else {
                    std::cout << getJSON(wObj);
                }
                break;
            case Action::UPDATE:
                if (args["category"].count()) {
                    std::string key_delimiter = ":";

                    if (args["item"].count()) {
                        std::string cat_str = args["category"].as<std::string>();
                        if (args["entry"].count()) {
                            std::string item_str = args["item"].as<std::string>();
                            std::string entry_input = args["entry"].as<std::string>();
                            std::string value_delimiter = ",";

                            if (entry_input.find(key_delimiter)) {
                                std::string old_entry_ident =
                                        entry_input.substr(0, entry_input.find(key_delimiter));
                                std::string new_entry_ident =
                                        entry_input.substr(entry_input.find(key_delimiter) + 1);

                                if (new_entry_ident.empty()) {
                                    throw std::invalid_argument("entry");
                                }

                                std::string entry_val = wObj.getCategory(cat_str).getItem(item_str).getEntry(
                                        old_entry_ident);

                                wObj.getCategory(cat_str)
                                        .getItem(item_str)
                                        .addEntry(new_entry_ident, entry_val);
                                wObj.getCategory(cat_str)
                                        .getItem(item_str)
                                        .deleteEntry(old_entry_ident);

                            } else if (entry_input.find(value_delimiter)) {
                                std::string entry_identifier =
                                        entry_input.substr(0, entry_input.find(value_delimiter));
                                std::string new_entry_val =
                                        entry_input.substr(entry_input.find(value_delimiter) + 1);

                                if (new_entry_val.empty()) {
                                    throw std::invalid_argument("entry");
                                }

                                wObj.getCategory(cat_str)
                                        .getItem(item_str)
                                        .addEntry(entry_identifier, new_entry_val);
                            } else {
                                throw std::invalid_argument("entry");
                            }

                        } else {
                            std::string item_input = args["item"].as<std::string>();

                            if (item_input.find(key_delimiter)) {
                                std::string old_item_ident =
                                        item_input.substr(0, item_input.find(key_delimiter));
                                std::string new_entry_ident =
                                        item_input.substr(item_input.find(key_delimiter) + 1);
                                Item &cur_item = wObj.getCategory(cat_str).getItem(old_item_ident);
                                cur_item.setIdent(new_entry_ident);

                                wObj.getCategory(cat_str).addItem(cur_item);
                                wObj.getCategory(cat_str).deleteItem(old_item_ident);

                            } else {
                                throw std::invalid_argument("item");
                            }
                        }
                    } else {
                        std::string cat_input = args["category"].as<std::string>();

                        if (cat_input.find(key_delimiter)) {
                            std::string old_cat_ident =
                                    cat_input.substr(0, cat_input.find(key_delimiter));
                            std::string new_cat_ident =
                                    cat_input.substr(cat_input.find(key_delimiter));
                            Category &cur_cat = wObj.getCategory(old_cat_ident);
                            cur_cat.setIdent(new_cat_ident);

                            wObj.addCategory(cur_cat);
                            wObj.deleteCategory(old_cat_ident);

                        } else {
                            throw std::invalid_argument("category");
                        }
                    }
                } else if (args["item"].count() || args["entry"].count()) {
                    throw std::out_of_range("Error: missing category argument(s).");

                } else {
                    throw std::out_of_range("Error: missing category, item or entry argument(s).");

                }
                break;

            case Action::DELETE:
                if (args["category"].count()) {
                    std::string cat_str = args["category"].as<std::string>();
                    if (args["item"].count()) {
                        std::string item_str = args["item"].as<std::string>();
                        if (args["entry"].count()) {
                            wObj.getCategory(cat_str).getItem(args["item"].as<std::string>()).deleteEntry(
                                    args["entry"].as<std::string>());
                        } else {
                            wObj.getCategory(cat_str).deleteItem(args["item"].as<std::string>());
                        }
                    } else if (args["entry"].count()) {
                        throw std::out_of_range("Error: missing item argument(s).");
                    } else {
                        wObj.deleteCategory(args["category"].as<std::string>());
                    }
                } else {
                    throw std::out_of_range("Error: missing category, item or entry argument(s).");
                }
                break;

            default:
                throw std::runtime_error("Unknown action not implemented");
        }
        wObj.save(args["db"].as<std::string>());

    } catch (std::invalid_argument &e) {
        std::cerr << "Error: invalid " << e.what() << " argument(s).";
        return 1;
    } catch (std::out_of_range &e) {
        std::cerr << e.what();
        return 1;
    }
    return 0;
}

/* Create a cxxopts instance. You do not need to modify this function.

 Example:
  auto options = App::cxxoptsSetup();
  auto args = options.parse(argc, argv);*/
cxxopts::Options App::cxxoptsSetup() {
    cxxopts::Options cxxopts("371pass", "Student ID: " + STUDENT_NUMBER + "\n");

    cxxopts.add_options()(
            "db", "Filename of the 371pass database",
            cxxopts::value<std::string>()->default_value("database.json"))(

            "action", "Action to take, can be: 'create', 'read', 'update', 'delete'.",
            cxxopts::value<std::string>())(

            "category",
            "Apply action to a category (e.g., if you want to add a category, set the"
            " action argument to 'add' and the category argument to your chosen"
            " category identifier).",
            cxxopts::value<std::string>())(

            "item",
            "Apply action to an item (e.g., if you want to add an item, set the "
            "action argument to 'add', the category argument to your chosen category "
            "identifier and the item argument to the item identifier).",
            cxxopts::value<std::string>())(

            "entry",
            "Apply action to an entry (e.g., if you want to add an entry, set the "
            "action argument to 'add', the category argument to your chosen category "
            "identifier, the item argument to your chosen item identifier, and the "
            "entry argument to the string 'key,value'). If there is no comma, an "
            "empty entry is inserted. If you are simply retrieving an entry, set the "
            "entry argument to the 'key'. If you are updating an entry key, use a : "
            "e.g., oldkey:newkey,newvalue.",
            cxxopts::value<std::string>())(

            "h,help", "Print usage.");

    return cxxopts;
}

/* Example:
  auto options = App::cxxoptsSetup();
  auto args = options.parse(argc, argv);
  App::Action action = parseActionArgument(args);*/
App::Action App::parseActionArgument(cxxopts::ParseResult &args) {
    std::string input = args["action"].as<std::string>();
    std::transform(input.begin(), input.end(), input.begin(), ::toupper);

    if (input == "CREATE") {
        return Action::CREATE;
    }
    if (input == "READ") {
        return Action::READ;
    }
    if (input == "UPDATE") {
        return Action::UPDATE;
    }
    if (input == "DELETE") {
        return Action::DELETE;
    }
    throw std::invalid_argument("action");
}


/* This function has been implemented for you, but you may wish to adjust it.
 You will have to uncomment the code, which has been left commented to ensure
 the coursework framework compiles (i.e., it calls functions that you must
 implement, once you have implemented them you may uncomment this function).

 Example:
  Wallet wObj{};
  std::cout << getJSON(wObj);*/
std::string App::getJSON(Wallet &wObj) {
    return wObj.str();
}

/* This function has been implemented for you, but you may wish to adjust it.
 You will have to uncomment the code, which has been left commented to ensure
 the coursework framework compiles (i.e., it calls functions that you must
 implement, once you have implemented them you may uncomment this function).

 Example:
  Wallet wObj{};
  std::string c = "category argument value";
  std::cout << getJSON(wObj, c);*/
std::string App::getJSON(Wallet &wObj, const std::string &c) {
    auto cObj = wObj.getCategory(c);
    return cObj.str();
}

/* This function has been implemented for you, but you may wish to adjust it.
 You will have to uncomment the code, which has been left commented to ensure
 the coursework framework compiles (i.e., it calls functions that you must
 implement, once you have implemented them you may uncomment this function).

 Example:
  Wallet wObj{};
  std::string c = "category argument value";
  std::string i = "item argument value";
  std::cout << getJSON(wObj, c, i);*/
std::string App::getJSON(Wallet &wObj, const std::string &c,
                         const std::string &i) {
    auto cObj = wObj.getCategory(c);
    const auto iObj = cObj.getItem(i);
    return iObj.str();
}


/* This function has been implemented for you, but you may wish to adjust it.
 You will have to uncomment the code, which has been left commented to ensure
 the coursework framework compiles (i.e., it calls functions that you must
 implement, once you have implemented them you may uncomment this function).

 Example:
  Wallet wObj{};
  std::string c = "category argument value";
  std::string i = "item argument value";
  std::string e = "entry argument value";
  std::cout << getJSON(wObj, c, i, e);*/
std::string App::getJSON(Wallet &wObj, const std::string &c,
                         const std::string &i, const std::string &e) {
    // Only uncomment this once you have implemented the functions used!
    auto cObj = wObj.getCategory(c);
    auto iObj = cObj.getItem(i);
    return iObj.getEntry(e);
}