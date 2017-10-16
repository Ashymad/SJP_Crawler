#include <iostream>
#include <fstream>
#include "SJPCrawler.h"

using namespace std;

int main(void){
  SJPCrawler SJPC;
  //ofstream file;
  //file.open("test.html");
  cout << SJPC.getCommentsNmb("https://sjp.pl/zds") << endl;
  //cout << "#" <<SJPC.whenLastComment("https://sjp.pl/zds") << "#" << endl;
  cout << SJPC.getWordsNmb() << endl;
  //SJPC.indexWords(1,SJPC.getPageCount(),true);

  //cout << SJPC.getWordsNmb() << endl;

  //string site = SJPC.site2string("https://sjp.pl/%B1%E6%EA%B3%F1%F3%B6%BF%BC");
  //file.close();
  //cURLCpp cupp;
  //cupp.getPage("https://sjp.pl/zds");
  //cout << cupp.toStr() << endl;
  //file.close();
  //system("read -n 1");
  return 0;
}
