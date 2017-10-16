#ifndef STRINGHELPERS_H_INCLUDED
#define STRINGHELPERS_H_INCLUDED

#include <string>
#include "IconvString.h"

int countInString(const std::string & sourceString, const std::string & stringToFind);

int countInString(const IconvString & sourceString, const std::string & stringToFind);

std::string substrBetween(const std::string & lhsstring, const std::string & sourceString, const std::string & rhsstring, size_t beggining);

std::string substrBetween(const std::string & lhsstring, const IconvString & sourceString, const std::string & rhsstring, size_t beggining);

#endif
