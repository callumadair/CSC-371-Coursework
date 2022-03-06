// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 851784
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------
// A category contains one or more Items, each with
// their own identifier ('ident').
// -----------------------------------------------------

#ifndef CATEGORY_H
#define CATEGORY_H

#include <string>
#include "item.h"

class Category {
    std::string identifier;
    std::unordered_map<std::string, Item> items;
public:
    Category(const std::string identifier);

    unsigned int size();

    bool empty();

    std::string getIdent();

    void setIdent(std::string identifier);

    Item &newItem(const std::string &item_identifier);

    bool addItem(const Item &item);

    Item &getItem(const std::string &item_identifier);

    bool deleteItem(const std::string &item_identifier);

    friend bool operator==(const Category &lhs, const Category &rhs);

    std::string str();
};

#endif // CATEGORY_H
