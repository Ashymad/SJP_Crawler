#ifndef CURLCPP_H_INCLUDED
#define CURLCPP_H_INCLUDED

#include <string>
#include <curl/curl.h>
#include "IconvString.h"

class cURLCpp{
  CURL* curl;
  IconvString IcStr;
  std::string _URL;

  static size_t write_data(void *contents, size_t size, size_t nmemb, IconvString & IS);

public:

  cURLCpp();

  bool getPage(const std::string & URL);

  std::string currentURL();

  std::string toStr();

  ~cURLCpp();
};

#endif
