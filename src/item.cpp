// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 851784
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include <utility>

#include "lib_json.hpp"
#include "item.h"


/* Constructs an Item object with the given identifier. */
/* Example:
  Item iObj{"identIdent"};*/
Item::Item(std::string identifier) : identifier(std::move(identifier)) {}

unsigned int Item::size() {
    return entries.size();
}

bool Item::empty() {
    return entries.empty();
}


/* Example:
  Item iObj{"identIdent"};
  auto ident = iObj.getIdent();*/
std::string Item::getIdent() {
    return identifier;
}

/* Example:
  Item iObj{"identIdent"};
  iObj.setIdent("identIdent2");*/
void Item::setIdent(const std::string &new_identifier) {
    this->identifier = new_identifier;
}

/* Adds a new entry to the entries map, if there is already an entry with the given key, the entry will be overwritten
 * and the function will return false otherwise the function will return true. */
/* Example:
  Item iObj{"identIdent"};
  iObj.addEntry("key", "value");*/
bool Item::addEntry(const std::string &key, const std::string &value) {
    auto search = entries.find(key);
    if (search != entries.end()) {
        entries.insert(std::make_pair(key, value));
        return false;
    }
    entries.insert(std::make_pair(key, value));
    return true;
}

/* Merges the entries maps of two items into one, favouring the other item's entries over the existing item's
 * entries. */
void Item::mergeEntries(Item &other) {
    other.entries.insert(entries.begin(), entries.end());
    std::swap(entries, other.entries);
}

/* Returns the value of the entry with the given key, if the entry does not exist, throws an out_of_range exception. */
/* Example:
  Item iObj{"identIdent"};
  iObj.addEntry("key", "value");
  auto value = iObj.getEntry("key");*/
std::string Item::getEntry(const std::string &key) {
    auto search = entries.find(key);
    if (search != entries.end()) {
        return search->second;
    }
    throw std::out_of_range("Error: invalid entry argument(s).");
}

/* Deletes the entry with the provided key and returns true if successful, if the entry does not exist or
 * was not deleted, an out_of_range exception is thrown. */
/* Example:
  Item iObj{"identIdent"};
  iObj.addEntry("key", "value");
  iObj.deleteEntry("key");*/
bool Item::deleteEntry(const std::string &key) {
    auto search = entries.find(key);
    if (search != entries.end()) {
        entries.erase(key);
        if (entries.find(key) == entries.end()) return true;
    }
    throw std::out_of_range("Error deleting entry.");
}


/* Example:
  Item iObj1{"identIdent"};
  iObj1.addEntry("key", "value");
  Item iObj2{"identIdent2"};
  if(iObj1 == iObj2) {
    ...
  }*/
bool operator==(const Item &lhs, const Item &rhs) {
    return lhs.identifier == rhs.identifier && lhs.entries == rhs.entries;
}

/* Returns a JSON string representation of the item object and the entries it holds. */
/* See the coursework specification for how this JSON should look.

 Example:
  Item iObj{"itemIdent"};
  std::string s = iObj.str();*/
std::string Item::str() const {
    nlohmann::json j;
    for (const auto &entry: entries) {
        j[entry.first] = entry.second;
    }
    return j.dump();
}