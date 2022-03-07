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

#include <sstream>

class Category {

};
namespace std {
template<>
struct hash<Category> {
    size_t operator()(const Category &obj) const {
        std::stringstream sstr;
        sstr << obj.str();
        return std::hash<std::string>{}(sstr.str());
    }
};
}
#endif // CATEGORY_H
