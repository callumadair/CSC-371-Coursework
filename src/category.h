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
#include <map>

#include "item.h"

class Category {
    std::string identifier;
    std::map<std::string, Item> items;
public:
    explicit Category(std::string identifier);

    unsigned int size();

    bool empty();

    std::string getIdent();

    void setIdent(const std::string& new_identifier);

    Item& newItem(const std::string& item_identifier);

    bool addItem(Item item);

    void mergeItems(Category other);

    Item &getItem(const std::string& item_identifier);

    bool deleteItem(const std::string& item_identifier);

    friend bool operator==(const Category &lhs, const Category &rhs);

    std::string str() const;

};
#endif // CATEGORY_H
