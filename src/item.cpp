// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 851784
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include <sstream>
#include "item.h"

// TODO Write a constructor that takes one parameter, a string identifier
//  and initialises the object and member data.
//
// Example:
//  Item iObj{"identIdent"};
Item::Item(const std::string identifier) : identifier(identifier) {}

unsigned int Item::size() {
    return entries.size();
}

bool Item::empty() {
    return entries.empty();
}


// TODO Write a function, getIdent, that returns the identifier for the Item. - DONE!
//
// Example:
//  Item iObj{"identIdent"};
//  auto ident = iObj.getIdent();
std::string Item::getIdent() {
    return identifier;
}

// TODO Write a function, setIdent, that takes one parameter, a string for a new
//  Item identifier, and updates the member variable. It returns nothing. - DONE!
//
// Example:
//  Item iObj{"identIdent"};
//  iObj.setIdent("identIdent2");
void Item::setIdent(const std::string identifier) {
    this->identifier = identifier;
}

// TODO Write a function, addEntry, that takes two parameters, an entry
//  key and value and returns true if the entry was inserted into the
//  container or false if the entry already existed and was replaced. - DONE!
//
// Example:
//  Item iObj{"identIdent"};
//  iObj.addEntry("key", "value");
bool Item::addEntry(const std::string key, const std::string value) {
    auto search = entries.find(key);
    if (search != entries.end()) {
        entries.insert(std::make_pair(key, value));
        return false;
    } else {
        entries.insert(std::make_pair(key, value));
        return true;
    }
}

void Item::mergeEntries(Item &other) {
    entries.insert(other.entries.begin(), other.entries.end());
}

// TODO Write a function, getEntry, that takes one parameter, an entry
//  key and returns it's value. If no entry exists, throw an appropriate
//  exception. - DONE!
//
// Example:
//  Item iObj{"identIdent"};
//  iObj.addEntry("key", "value");
//  auto value = iObj.getEntry("key");
std::string Item::getEntry(const std::string key) {
    auto search = entries.find(key);
    if (search != entries.end()) {
        return search->second;
    } else {
        throw std::out_of_range("No such key found.");
    }
}

// TODO Write a function, deleteEntry, that takes one parameter, an entry
//  key, deletes it from the container, and returns true if the Item was
//  deleted. If no entry exists, throw an appropriate exception. - DONE!
//
// Example:
//  Item iObj{"identIdent"};
//  iObj.addEntry("key", "value");
//  iObj.deleteEntry("key");
bool Item::deleteEntry(const std::string key) {
    auto search = entries.find(key);
    if (search != entries.end()) {
        entries.erase(key);
        return true;
    }
    throw std::out_of_range("No such key found.");
}


// TODO Write an == operator overload for the Item class, such that two
//  Item objects are equal only if they have the same identifier and same
//  entries. - DONE!
//
// Example:
//  Item iObj1{"identIdent"};
//  iObj1.addEntry("key", "value");
//  Item iObj2{"identIdent2"};
//  if(iObj1 == iObj2) {
//    ...
//  }
bool operator==(const Item &lhs, const Item &rhs) {
    return lhs.identifier == rhs.identifier && lhs.entries == rhs.entries;
}

// TODO Write a function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the Item. - DONE!
//
// See the coursework specification for how this JSON should look.
//
// Example:
//  Item iObj{"itemIdent"};
//  std::string s = iObj.str();
std::string Item::str() const {
    std::stringstream sstr;
    sstr << "{\"" << identifier << "\":{";
    for (auto it = entries.begin(); it != entries.end(); it++) {
        sstr << "\"" << it->first << "\":\"" << it->second << "\"";
        if (std::next(it) != entries.end()) sstr << ",";
    }
    sstr << "}";
    return sstr.str();
}