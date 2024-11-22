//
//#ifndef COMPS_H
//#define COMPS_H
//
//#include <functional>
//#include "User.h"
//
//const std::function<bool(const int&, const int&)> ascending= [](const int &a, const int &b) { return a < b; };
//const std::function<bool(const int&, const int&)> descending= [](const int &a, const int &b) { return a > b; };
//
//const std::function<bool(const QString&, const QString&)> compareByLastNameAscending =[](const QString &a, const QString &b) { return a < b; };
//const std::function<bool(const QString&, const QString&)> compareByLastNameDescending =[](const QString &a, const QString &b) { return a > b; };
//
//const std::function<bool(const QString&, const QString&)> compareByFirstNameAscending =[](const QString &a, const QString &b) { return a < b; };
//const std::function<bool(const QString&, const QString&)> compareByFirstNameDescending =[](const QString &a, const QString &b) { return a > b; };
//
//const std::function<bool(const int&, const int&)> compareByAgeAscending =[](const int &a, const int &b) { return a < b; };
//const std::function<bool(const int&, const int&)> compareByAgeDescending =[](const int &a, const int &b) { return a > b; };
//
//auto ageExtractor = [](const User& user) { return user.getAge(); };
//auto lastNameExtractor = [](const User& user) { return user.getLastName(); };
//auto firstNameExtractor = [](const User& user) { return user.getFirstName(); };
//
auto intExtractor = [](const int& number) { return number; };
//
//#endif COMPS_H
