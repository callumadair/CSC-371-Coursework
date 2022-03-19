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
// username is for).
// -----------------------------------------------------

#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <map>

class Item {
    std::string identifier;
    std::map<std::string, std::string> entries;
public:
    explicit Item(std::string identifier);

    unsigned int size();

    bool empty();

    std::string getIdent();

    void setIdent(const std::string &new_identifier);

    bool addEntry(const std::string &key, const std::string &value);

    void mergeEntries(Item &other);

    std::string getEntry(const std::string &key);

    bool deleteEntry(const std::string &key);

    friend bool operator==(const Item &lhs, const Item &rhs);

    std::string str() const;

};

#endif // ITEM_H
