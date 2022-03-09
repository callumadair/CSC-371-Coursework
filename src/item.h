// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 851784
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------
// An Item class contains multiple 'entries' as
// key/value pairs (e.g., a key might be 'username'
// and a value would be the username, another might be
// 'url' and the value is the website address the
// username is for.
// -----------------------------------------------------

#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <unordered_map>
#include <sstream>
#include "lib_json.hpp"

class Item {
    std::string identifier;
    std::map<std::string, std::string> entries;
public:
    Item(const std::string identifier);

    unsigned int size();

    bool empty();

    std::string getIdent();

    void setIdent(std::string identifier);

    bool addEntry(std::string key, std::string value);

    void mergeEntries(Item &other);

    std::string getEntry(std::string key);

    bool deleteEntry(std::string key);

    friend bool operator==(const Item &lhs, const Item &rhs);

    std::string str() const;

};
namespace std {
    template<>
    struct hash<Item> {
        size_t operator()(Item &item) const {
            std::stringstream sstr;
            sstr << item.str();
            return std::hash<std::string>{}(sstr.str());
        }
    };
}

#endif // ITEM_H
