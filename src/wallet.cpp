// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 851784
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include <fstream>
#include <iostream>
#include <sstream>
#include "lib_json.hpp"
#include "wallet.h"

/* Example:
  Wallet wObj{};*/

Wallet::Wallet() {}

/* Example:
  Wallet wObj{};
  auto size = wObj.size();*/

unsigned int Wallet::size() {
    return categories.size();
}

/* Example:
  Wallet wwObj{};
  auto isEmpty = wObj.empty();*/

bool Wallet::empty() {
    return categories.empty();
}

/* Example:
  Wallet wObj{};
  wObj.newCategory("categoryIdent");*/

Category Wallet::newCategory(std::string category_identifier) {
    auto search = categories.find(category_identifier);
    if (search == categories.end()) {
        Category new_category(category_identifier);
        categories.insert(std::make_pair(category_identifier, new_category));

        if (categories.find(category_identifier) == categories.end()) {
            throw std::runtime_error("Category was not inserted successfully.");
        }

        Category &ref = new_category;
        return ref;
    } else {
        return categories.find(category_identifier)->second;
    }
}

/* Example:
  Wallet wObj{};
  Category cObj{"categoryIdent"};
  wObj.addCategory(cObj);*/

bool Wallet::addCategory(Category category) {
    auto search = categories.find(category.getIdent());
    if (search == categories.end()) {
        categories.insert(std::make_pair(category.getIdent(), category));

        if (categories.find(category.getIdent()) == categories.end()) {
            throw std::runtime_error("Item was not inserted successfully.");
        }

        return true;
    } else {
        categories.find(category.getIdent())->second.mergeItems(category);
        return false;
    }
}

/* Example:
 Wallet wObj{};
  wObj.newCategory("categoryIdent");
  auto cObj = wObj.getCategory("categoryIdent");*/

Category Wallet::getCategory(std::string category_identifier) {
    auto search = categories.find(category_identifier);
    if (search != categories.end()) {
        Category &ref = search->second;
        return ref;
    } else {
        throw std::out_of_range("Error: invalid category argument(s).");
    }
}


/* Example:
  Wallet wObj{};
  wObj.newCategory("categoryIdent");
  wObj.deleteCategory("categoryIdent");*/

bool Wallet::deleteCategory(const std::string category_identifier) {
    auto search = categories.find(category_identifier);
    if (search != categories.end()) {
        categories.erase(category_identifier);
        if (categories.find(category_identifier) == categories.end()) return true;
    }
    throw std::out_of_range("No such item found.");
}


/* A note on clashes:
  If you encounter two categories with the same key, the categories should be
  merged (not replaced!). If you encounter two items with the same key in the
  same category, the items should be merged (not replaced!). If you encounter
  two entries with the same key in the same item, the entries should be merged
  (undefined as to which value is picked). Two items in different categories
  can have the same key, as can two entries in different items.

 JSON formatting:
  The JSON file has the following format (see the sample database.json file
  also provided with the coursework framework):
    {
      "Category 1" : {
        "Item 1":  {
          "detail 1 key": "detail 1 value",
          "detail 2 key": "detail 2 value",
          ...
        },
        "Item 2":  {
          "detail 1 key": "detail 1 value",
          "detail 2 key": "detail 2 value"
        },
        ...
      },
      "Category 2": {
        "Item 1": {
          "detail 1 key": "detail 1 value"
        }
        ...
      }
    }

 More help:
  To help you with this function, I've selected the nlohmann::json
  library that you must use for your coursework. Read up on how to use it
  here: https://github.com/nlohmann/json. You may not use any other external
  library other than the one I have provided. You may choose to process the
  JSON yourself without the help of the library but I guarantee this will be
  more work.

  Understanding how to use external libraries is part of this coursework! You
  will need to use this file to deserialize the JSON from string
  to the JSON object provided by this library. Don't just look at the code
  provided below, or in the README on the GitHub and despair. Google search,
  look around, try code out in a separate file to all figure out how to use
  this library.

  Once you have deserialized the JSON string into an object, you will need to
  loop through this object, constructing Category and Item objects according
  to the JSON data in the file.

 Example:
  Wallet wObj{};
  wObj.load("database.json");*/

bool Wallet::load(std::string filename) {
    std::fstream json_file(filename);
    if (json_file.is_open()) {
        std::string json_str((std::istreambuf_iterator<char>(json_file)),
                             (std::istreambuf_iterator<char>()));

        auto json_obj = nlohmann::json::parse(json_str);

        for (auto cat_it = json_obj.begin(); cat_it != json_obj.end(); cat_it++) {
            Category new_category(cat_it.key());
            auto category_items = cat_it.value();

            for (auto item_it = category_items.begin(); item_it != category_items.end(); item_it++) {
                Item new_item(item_it.key());
                auto item_entries = item_it.value();

                for (auto entry_it = item_entries.begin(); entry_it != item_entries.end(); entry_it++) {
                    new_item.addEntry(entry_it.key(), entry_it.value());
                }

                try {
                    Item existing_item = new_category.getItem(item_it.key());
                    existing_item.mergeEntries(new_item);
                } catch (std::out_of_range &e) {
                    new_category.addItem(new_item);
                }
            }

            try {
                Category existing_category = getCategory(cat_it.key());
                existing_category.mergeItems(new_category);
            } catch (std::out_of_range &e) {
                addCategory(new_category);
            }
        }
        return true;
    }
    throw std::runtime_error("Json file did not open successfully");
}

/* Example:
  Wallet wObj{};
   wObj.load("database.json");
  ...
  wObj.save("database.json");*/

bool Wallet::save(std::string filename) {
    std::fstream json_file(filename);
    if (json_file.is_open()) {
        json_file << str();
        return true;
    }
    throw std::runtime_error("Json file did not open successfully");
}

/* Example:
  Wallet wObj1{};
  Wallet wObj2{};
  if(wObj1 == wObj2) {
    ...
  }*/
bool operator==(const Wallet &lhs, const Wallet &rhs) {
    return lhs.categories == rhs.categories;
}

/* Hint:
  See the coursework specification for how this JSON should look.

 Example:
  Wallet wObj{};
  std::string s = wObj.str();*/

std::string Wallet::str() const {
    std::stringstream sstr;
    sstr << "{";
    for (auto it = categories.begin(); it != categories.end(); it++) {
        sstr << "\"" << it->first << "\":" << it->second.str();
        if (std::next(it) != categories.end()) sstr << ",";
    }
    sstr << "}";
    return sstr.str();
}
