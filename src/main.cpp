#include <iostream>
#include "SJPCrawler.h"

using namespace std;

int main(void){
  SJPCrawler SJPC;

  // Get number of pages
  int sjp_page_count = SJPC.getPageCount();

  // Index all pages. This takes very long time.
  SJPC.indexWords(1,sjp_page_count,true);

  return 0;
}
