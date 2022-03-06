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

class Item {
    std::string identifier;
    std::unordered_map<std::string, std::string> entries;
public:
    Item(std::string identifier);

    unsigned int size();

    bool empty();

    std::string getIdent();

    void setIdent(std::string identifier);

    bool addEntry(std::string key, std::string value);

    std::string getEntry(std::string key);

    bool deleteEntry(std::string key);

    friend bool operator==(Item &lhs, Item &rhs);

    std::string str();

};

#endif // ITEM_H
