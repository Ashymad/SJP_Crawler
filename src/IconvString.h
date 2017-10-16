#ifndef ICONVSTRING_H_INCLUDED
#define ICONVSTRING_H_INCLUDED

#include <string>
#include <iconv.h>
#include <iostream>

class IconvString{
  char * str_1;
  char * str_2;
  size_t _size, _capacity_2, _capacity;
  iconv_t conv;

  void swapstr();
 public:
  IconvString();

  IconvString(const std::string & sourceString);

  IconvString& operator=(const std::string & rhs);

  std::string operator=(const IconvString & other) const;

  char operator[](size_t index) const;

  operator std::string() const;

  friend std::ostream & operator<<(std::ostream & os, const IconvString & ic);

  class iterator{
    char * ptr;

  public:
    iterator(){};

    iterator(char * p);

    iterator(const IconvString::iterator & it);

    IconvString::iterator & operator++();

    IconvString::iterator operator++(int);

    IconvString::iterator & operator=(const IconvString::iterator & other);

    IconvString::iterator operator+(int rhs) const;

    IconvString::iterator operator-(int rhs) const;

    int operator-(const IconvString::iterator & rhs) const;

    bool operator==(const IconvString::iterator & rhs) const;

    bool operator!=(const IconvString::iterator & rhs) const;

    char operator*() const;
  };

  void append(const std::string & rhs);

  void reencode(const std::string & source_encoding, const std::string & target_encoding);

  void clear();

  size_t find(const std::string & str, size_t pos) const;

  std::string substr(size_t startPos, size_t length) const;

  size_t size() const;

  std::string str() const;

  IconvString::iterator begin() const;

  IconvString::iterator end() const;

  ~IconvString();
};

std::ostream & operator<<(std::ostream & os, const IconvString & ic);

int countInString(const IconvString & sourceString, const std::string & stringToFind);

std::string substrBetween(const std::string & lhsstring, const IconvString & sourceString, const std::string & rhsstring, size_t beggining);

#endif // ICONVSTRING_H_INCLUDED
