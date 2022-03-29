// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 851784
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include "category.h"
#include "lib_json.hpp"


/* Constructs a Category object with the given identifier. */
/* Example:
  Category c{"categoryIdent"};*/

Category::Category(std::string identifier) : identifier(std::move(identifier)) {}

unsigned int Category::size() const {
    return items.size();
}

bool Category::empty() const {
    return items.empty();
}

/* Example:
  Category cObj{"categoryIdent"};
  auto ident = cObj.getIdent();*/

std::string Category::getIdent() const {
    return identifier;
}

/*Example:
 Category cObj{"categoryIdent"};
 cObj.setIdent("categoryIdent2");*/

void Category::setIdent(const std::string &new_identifier) {
    this->identifier = new_identifier;
}

/* Creates a new item object, inserts it into the items map and returns a reference to that object,
 * if an item with that identifier already exists the function returns a reference to the existing item
 * object already held by the category. */
/* Example:
  Category cObj{"categoryIdent"};
  cObj.newItem("itemIdent");*/

Item &Category::newItem(const std::string &item_identifier) {
    auto search = items.find(item_identifier);
    if (search == items.end()) {
        Item new_item(item_identifier);
        items.insert(std::make_pair(item_identifier, new_item));

        if (items.find(item_identifier) == items.end()) {
            throw std::runtime_error("Item was not inserted successfully.");
        }
    }
    Item &ref = items.find(item_identifier)->second;
    return ref;
}

/* Adds an item object to the category's map, if the category already contains an item with the same identifier, the
 * two items will be merged, leaving one item holding all of their entries.*/
/* Example:
  Category cObj{"categoryIdent"};
  Item iObj{"itemIdent"};
  cObj.addItem(iObj);*/

bool Category::addItem(Item item) {
    auto search = items.find(item.getIdent());
    if (search == items.end()) {
        items.insert(std::make_pair(item.getIdent(), item));

        if (items.find(item.getIdent()) == items.end()) {
            throw std::runtime_error("Item was not inserted successfully.");
        }

        return true;
    }
    items.find(item.getIdent())->second.mergeEntries(item);
    return false;
}

/* Merges the item maps of two categories into one, favouring the other category's items over the existing category's
 * items. */
void Category::mergeItems(Category other) {
    other.items.insert(items.begin(), items.end());
    std::swap(items, other.items);
}


/* Returns a reference to the item with the given identifier, if the item does not exist, throws an out_of_range
 * exception */
/* Hint:
  See the test scripts for the exception expected.

 Example:
  Category cObj{"categoryIdent"};
  cObj.newItem("itemIdent");
  auto iObj = cObj.getItem("itemIdent");*/

Item &Category::getItem(const std::string &item_identifier) {
    auto search = items.find(item_identifier);
    if (search != items.end()) {
        Item &ref = search->second;
        return ref;
    }
    throw std::out_of_range("Error: invalid item argument(s).");
}

/* Deletes the item with the provided identifier and returns true if successful, if the item does not exist or
 * was not deleted, an out_of_range exception is thrown. */
/* Example:
  Category cObj{"categoryIdent"};
  cObj.newItem("itemIdent");
  bool result = cObj.deleteItem("itemIdent");*/

bool Category::deleteItem(const std::string &item_identifier) {
    auto search = items.find(item_identifier);
    if (search != items.end()) {
        items.erase(item_identifier);
        if (items.find(item_identifier) == items.end()) return true;
    }
    throw std::out_of_range("Error: invalid item argument(s).");
}

/* Example:
  Category cObj1{"categoryIdent1"};
  cObj1.newItem("itemIdent");
  Category cObj2{"categoryIdent2"};
  if(cObj1 == cObj2) {
    ...
*/
bool operator==(const Category &lhs, const Category &rhs) {
    return lhs.identifier == rhs.identifier && lhs.items == rhs.items;
}

/* Returns a JSON string representation of the category object and the items it holds. */
/* See the coursework specification for how this JSON should look.

 Example:
  Category cObj{"categoryIdent"};
  std::string s = cObj.str();*/
std::string Category::str() const {
    nlohmann::json j;
    for (const auto &item: items) {
        // Removes the escape characters from the JSON string and saves the item key/value pair.
        auto json_val = nlohmann::json::parse(item.second.str());
        j[item.first] = json_val.empty() ? nlohmann::json::object() : json_val;
    }
    return j.dump();
}