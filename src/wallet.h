// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 851784
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------
// The root object that holds all data for 371pass. This
// class contains Categories, which in turn hold Items,
// which hold a mapping of entries.
// -----------------------------------------------------

#ifndef WALLET_H
#define WALLET_H

#include <unordered_map>
#include "category.h"

class Wallet {
    std::unordered_map<std::string, Category> categories;
public:
    Wallet();

    unsigned int size();

    bool empty();

    Category newCategory(std::string category_identifier);

    bool addCategory(Category category);

    Category getCategory(std::string category_identifier);

    bool deleteCategory(std::string category_identifier);

    bool load(std::string filename);

    bool save(std::string filename);

    friend bool operator==(const Wallet &lhs, const Wallet &rhs);

    std::string str();
};

#endif // WALLET_H
