#ifndef SJPCRAWLER_H_INCLUDED
#define SJPCRAWLER_H_INCLUDED

#include <string>
#include "cURLCpp.h"

class SJPCrawler{
  cURLCpp cupp;

public:

  int getCommentsNmb(const std::string & URL);

  std::string whenLastComment(const std::string & URL);

  int getWordsNmb();

  size_t getPageCount();

  void indexWords(size_t startPage, size_t endPage, bool verbose);

};

#endif
