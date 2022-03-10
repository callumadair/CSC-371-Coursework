// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 851784
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include <sstream>
#include "category.h"


/* Example:
  Category c{"categoryIdent"};*/

Category::Category(std::string identifier) : identifier(identifier) {}

unsigned int Category::size() {
    return items.size();
}

bool Category::empty() {
    return items.empty();
}

/* Example:
  Category cObj{"categoryIdent"};
  auto ident = cObj.getIdent();*/

std::string Category::getIdent() {
    return identifier;
}

/*Example:
 Category cObj{"categoryIdent"};
 cObj.setIdent("categoryIdent2");*/

void Category::setIdent(const std::string identifier) {
    this->identifier = identifier;
}

/* Example:
  Category cObj{"categoryIdent"};
  cObj.newItem("itemIdent");*/

Item Category::newItem(const std::string item_identifier) {
    auto search = items.find(item_identifier);
    if (search == items.end()) {
        Item new_item(item_identifier);
        items.insert(std::make_pair(item_identifier, new_item));

        if (items.find(item_identifier) == items.end()) {
            throw std::runtime_error("Item was not inserted successfully.");
        }

        Item &ref = new_item;
        return ref;
    } else {
        return items.find(item_identifier)->second;
    }
}

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
    } else {
        items.find(item.getIdent())->second.mergeEntries(item);
        return false;
    }
}

void Category::mergeItems(Category other) {
    items.insert(other.items.begin(), other.items.end());
}


/* Hint:
  See the test scripts for the exception expected.

 Example:
  Category cObj{"categoryIdent"};
  cObj.newItem("itemIdent");
  auto iObj = cObj.getItem("itemIdent");*/

Item &Category::getItem(const std::string item_identifier) {
    auto search = items.find(item_identifier);
    if (search != items.end()) {
        Item &ref = search->second;
        return ref;
    } else {
        throw std::out_of_range("No such item found.");
    }
}

/* Example:
  Category cObj{"categoryIdent"};
  cObj.newItem("itemIdent");
  bool result = cObj.deleteItem("itemIdent");*/

bool Category::deleteItem(const std::string item_identifier) {
    auto search = items.find(item_identifier);
    if (search != items.end()) {
        items.erase(item_identifier);
        if(items.find(item_identifier) == items.end()) return true;
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


/* See the coursework specification for how this JSON should look.

 Example:
  Category cObj{"categoryIdent"};
  std::string s = cObj.str();*/
std::string Category::str() const {
    std::stringstream sstr;
    sstr << "{";
    for (auto it = items.begin(); it != items.end(); it++) {
        sstr << "\"" << it->first << "\":" << it->second.str() << "";
        if (std::next(it) != items.end()) sstr << ",";
    }
    sstr << "}";
    return sstr.str();
}