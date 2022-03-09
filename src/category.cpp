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

// TODO Write a constructor that takes one parameter, a string identifier
//  and initialises the object and member data. - DONE!
//
// Example:
//  Category c{"categoryIdent"};

Category::Category(std::string identifier) : identifier(identifier) {}

unsigned int Category::size() {
    return items.size();
}

bool Category::empty() {
    return items.empty();
}

// TODO Write a function, getIdent, that returns the identifier for the
//  Category. - DONE!
//
// Example:
//  Category cObj{"categoryIdent"};
//  auto ident = cObj.getIdent();

std::string Category::getIdent() {
    return identifier;
}

// TODO Write a function, setIdent, that takes one parameter, a string for a new
//  Category identifier, and updates the member variable. It returns nothing. - DONE!
//
// Example:
//  Category cObj{"categoryIdent"};
//  cObj.setIdent("categoryIdent2");

void Category::setIdent(const std::string identifier) {
    this->identifier = identifier;
}

// TODO Write a function, newItem, that takes one parameter, an Item identifier,
//  (a string) and returns the Item object as a reference. If an object with the
//  same identifier already exists, then the existing object should be returned.
//  Throw a std::runtime_error if the Item object cannot be inserted into the
//  container for whatever reason. - DONE!
//
// Example:
//  Category cObj{"categoryIdent"};
//  cObj.newItem("itemIdent");

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

// TODO Write a function, addItem, that takes one parameter, an Item object,
//  and returns true if the object was successfully inserted. If an object with
//  the same identifier already exists, then the contents should be merged and
//  return false. - DONE!
//
// Example:
//  Category cObj{"categoryIdent"};
//  Item iObj{"itemIdent"};
//  cObj.addItem(iObj);

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

// TODO Write a function, getItem, that takes one parameter, an Item
//  identifier (a string) and returns the Item as a reference. If no Item
//  exists, throw an appropriate exception. - DONE!
//
// Hint:
//  See the test scripts for the exception expected.
//
// Example:
//  Category cObj{"categoryIdent"};
//  cObj.newItem("itemIdent");
//  auto iObj = cObj.getItem("itemIdent");

Item &Category::getItem(const std::string item_identifier) {
    auto search = items.find(item_identifier);
    if (search != items.end()) {
        Item &ref = search->second;
        return ref;
    } else {
        throw std::out_of_range("No such item found.");
    }
}

// TODO Write a function, deleteItem, that takes one parameter, an Item
//  identifier (a string), deletes it from the container, and returns true if
//  the Item was deleted. If no Item exists, throw an appropriate exception.
//
// Example:
//  Category cObj{"categoryIdent"};
//  cObj.newItem("itemIdent");
//  bool result = cObj.deleteItem("itemIdent");

bool Category::deleteItem(const std::string item_identifier) {
    auto search = items.find(item_identifier);
    if (search != items.end()) {
        items.erase(item_identifier);
        return true;
    }
    throw std::out_of_range("No such item found.");
}

// TODO Write an == operator overload for the Category class, such that two
//  Category objects are equal only if they have the same identifier and same
//  Items.
//
// Example:
//  Category cObj1{"categoryIdent1"};
//  cObj1.newItem("itemIdent");
//  Category cObj2{"categoryIdent2"};
//  if(cObj1 == cObj2) {
//    ...
//
bool operator==(const Category &lhs, const Category &rhs) {
    return lhs.identifier == rhs.identifier && lhs.items == rhs.items;
}

// TODO Write a function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the Category. - DONE!
//
// See the coursework specification for how this JSON should look.
//
// Example:
//  Category cObj{"categoryIdent"};
//  std::string s = cObj.str();
std::string Category::str() const {
    std::stringstream sstr;
    sstr << "{" << identifier << ": " << " {";
    for (auto it = items.begin(); it != items.end(); it++) {
        sstr << it->first << ":" << it->second.str();
        if (std::next(it) != items.end()) sstr << ",";
        sstr << "\n";
    }
    sstr << "}";
    return sstr.str();
}