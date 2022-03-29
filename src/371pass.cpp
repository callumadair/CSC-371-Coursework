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

#include "371pass.h"
#include "lib_cxxopts.hpp"
#include "wallet.h"


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


    // Check if a valid action argument is passed and try to perform the action.
    try {
        const Action a = parseActionArgument(args);
        switch (a) {
            case Action::CREATE:
                executeCreateAction(args, wObj);
                break;

            case Action::READ:
                executeReadAction(args, wObj);
                break;

            case Action::UPDATE:
                executeUpdateAction(args, wObj);
                break;

            case Action::DELETE:
                executeDeleteAction(args, wObj);
                break;

            default:
                throw std::runtime_error("Unknown action not implemented");
        }

        if (a != Action::READ) wObj.save(args["db"].as<std::string>());

    } catch (const std::invalid_argument &e) {
        std::cerr << "Error: invalid " << e.what() << " argument(s).";
        return 1;
    } catch (const std::out_of_range &e) {
        std::cerr << e.what();
        return 1;
    } catch (const std::runtime_error &e) {
        std::cerr << e.what();
        return 1;
    }
    return 0;
}

/* Creates new containers as far as the arguments specify, and adds an entry if correctly specified, if there are
 * missing required arguments, an applicable out_of_range exception is thrown. */
void App::executeCreateAction(const cxxopts::ParseResult &args, Wallet &wObj) {
    if (args["category"].count()) {
        Category &new_cat = wObj.newCategory(args["category"].as<std::string>());
        if (args["item"].count()) {
            Item &new_item = new_cat.newItem(args["item"].as<std::string>());
            if (args["entry"].count()) {
                std::string entry_input = args["entry"].as<std::string>();
                std::string entry_delimiter = ",";

                if (entry_input.find(entry_delimiter) != std::string::npos) {
                    std::string entry_identifier = entry_input.substr(0, entry_input.find(entry_delimiter));
                    std::string entry_value = entry_input.substr(entry_input.find(entry_delimiter) + 1);
                    new_item.addEntry(entry_identifier, entry_value);
                } else {
                    new_item.addEntry(entry_input, "");
                }
            }
        } else if (args["entry"].count()) {
            throw std::out_of_range("Error: missing item argument(s).");
        }
    } else if (args["item"].count() || args["entry"].count()) {
        throw std::out_of_range("Error: missing category argument(s).");
    } else {
        throw std::out_of_range("Error: missing category, item or entry argument(s).");
    }
}

/* Prints to the standard output either the correctly specified container in JSON format or the value of the correctly
 * specified entry, if required arguments are missing or incorrect an appropriate exception is thrown. */
void App::executeReadAction(const cxxopts::ParseResult &args, Wallet &wObj) {
    if (args["category"].count()) {
        if (args["item"].count()) {
            if (args["entry"].count()) {
                std::cout << getJSON(wObj, args["category"].as<std::string>(),
                                     args["item"].as<std::string>(),
                                     args["entry"].as<std::string>());
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
}

/* Updates the specified container(s) and/or entry, if given the correct arguments at all levels, if an argument is
 * missing a delimiter, a required argument is missing, or the new identifier is missing an exception is thrown, and
 * none of the changes will be applied. */
void App::executeUpdateAction(const cxxopts::ParseResult &args, Wallet &wObj) {
    if (args["category"].count()) {
        std::string key_delimiter = ":";
        std::string cat_input = args["category"].as<std::string>();
        std::string cur_cat_ident = cat_input.find(key_delimiter) == std::string::npos
                                    ? cat_input : cat_input.substr(0, cat_input.find(key_delimiter));

        if (args["item"].count()) {
            Category &cur_cat = wObj.getCategory(cur_cat_ident);
            std::string item_input = args["item"].as<std::string>();
            std::string cur_item_ident = item_input.find(key_delimiter) == std::string::npos
                                         ? item_input : item_input.substr(0, item_input.find(key_delimiter));

            if (args["entry"].count()) {
                Item &cur_item = cur_cat.getItem(cur_item_ident);
                std::string entry_input = args["entry"].as<std::string>();
                std::string value_delimiter = ",";

                /* Checks if the key and/or value delimiters are present in the entry string, performing the correct
                update depending on the delimiter(s) and throws an invalid_argument exception if neither is present. */
                if (entry_input.find(key_delimiter) != std::string::npos
                    && entry_input.find(value_delimiter) != std::string::npos) {
                    std::string old_entry_ident = entry_input.substr(0, entry_input.find(key_delimiter));
                    std::string new_entry_ident = entry_input.substr(entry_input.find(key_delimiter) + 1,
                                                                     entry_input.find(value_delimiter) -
                                                                     entry_input.find(key_delimiter) - 1);
                    std::string new_entry_val = entry_input.substr(entry_input.find(value_delimiter) + 1);

                    cur_item.addEntry(new_entry_ident, new_entry_val);
                    cur_item.deleteEntry(old_entry_ident);

                } else if (entry_input.find(key_delimiter) != std::string::npos) {
                    std::string old_entry_ident = entry_input.substr(0, entry_input.find(key_delimiter));
                    std::string new_entry_ident = entry_input.substr(entry_input.find(key_delimiter) + 1);

                    if (new_entry_ident.empty()) {
                        throw std::invalid_argument("entry");
                    }

                    std::string entry_val = cur_item.getEntry(old_entry_ident);
                    cur_item.addEntry(new_entry_ident, entry_val);
                    cur_item.deleteEntry(old_entry_ident);

                } else if (entry_input.find(value_delimiter) != std::string::npos) {
                    std::string entry_identifier =
                            entry_input.substr(0, entry_input.find(value_delimiter));
                    std::string new_entry_val =
                            entry_input.substr(entry_input.find(value_delimiter) + 1);

                    cur_item.deleteEntry(entry_identifier);
                    cur_item.addEntry(entry_identifier, new_entry_val);
                } else {
                    throw std::invalid_argument("entry");
                }

            }
            if (item_input.find(key_delimiter) != std::string::npos) {
                std::string new_item_ident = item_input.substr(item_input.find(key_delimiter) + 1);
                Item &cur_item = cur_cat.getItem(cur_item_ident);

                if (new_item_ident.empty()) {
                    throw std::invalid_argument("entry");
                }
                cur_item.setIdent(new_item_ident);

                cur_cat.addItem(cur_item);
                cur_cat.deleteItem(cur_item_ident);
            }

        } else if (args["entry"].count()) {
            throw std::out_of_range("Error: missing item argument(s).");
        }
        if (cat_input.find(key_delimiter) != std::string::npos) {
            std::string new_cat_ident = cat_input.substr(cat_input.find(key_delimiter) + 1);
            Category &cur_cat = wObj.getCategory(cur_cat_ident);

            if (new_cat_ident.empty()) {
                throw std::invalid_argument("entry");
            }
            cur_cat.setIdent(new_cat_ident);

            wObj.addCategory(cur_cat);
            wObj.deleteCategory(cur_cat_ident);

        }
    } else if (args["item"].count() || args["entry"].count()) {
        throw std::out_of_range("Error: missing category argument(s).");
    } else {
        throw std::out_of_range("Error: missing category, item or entry argument(s).");
    }
}

/* Deletes the specified container or entry, if in an invalid argument is given or a required argument is missing, an
 * appropriate exception is thrown and nothing is deleted. */
void App::executeDeleteAction(const cxxopts::ParseResult &args, Wallet &wObj) {
    if (args["category"].count()) {
        std::string cat_str = args["category"].as<std::string>();
        if (args["item"].count()) {
            std::string item_str = args["item"].as<std::string>();
            if (args["entry"].count()) {
                wObj.getCategory(cat_str)
                        .getItem(item_str)
                        .deleteEntry(args["entry"].as<std::string>());
            } else {
                wObj.getCategory(cat_str).deleteItem(item_str);
            }
        } else if (args["entry"].count()) {
            throw std::out_of_range("Error: missing item argument(s).");
        } else {
            wObj.deleteCategory(args["category"].as<std::string>());
        }
    } else {
        throw std::out_of_range("Error: missing category, item or entry argument(s).");
    }
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

    if (input == "CREATE") return Action::CREATE;

    if (input == "READ") return Action::READ;

    if (input == "UPDATE") return Action::UPDATE;

    if (input == "DELETE") return Action::DELETE;
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