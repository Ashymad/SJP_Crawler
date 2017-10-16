#include <SQLiteCpp/SQLiteCpp.h>
#include <sqlite3.h>
#include "SJPCrawler.h"
#include "stringhelpers.h"

using namespace std;

int SJPCrawler::getCommentsNmb(const string & URL){
  if (URL != cupp.currentURL())
    cupp.getPage(URL);
  return countInString(cupp.toStr(), "\">#</a>");
}

string SJPCrawler::whenLastComment(const string & URL){
  if (URL != cupp.currentURL())
    cupp.getPage(URL);
  string site = cupp.toStr();
  return substrBetween(">#</a> ",site,"</span>",site.rfind(">#</a> "));
}

int SJPCrawler::getWordsNmb(){
  string URL{"https://sjp.pl/slownik/lp.phtml"};
  if (URL != cupp.currentURL())
    cupp.getPage(URL);
  return atoi(substrBetween("&nbsp;<tt><b>", cupp.toStr(), "</b></tt>",0).c_str());
}

size_t SJPCrawler::getPageCount(){
  string URL{"https://sjp.pl/slownik/lp.phtml"};
  if (URL != cupp.currentURL())
    cupp.getPage(URL);
  string site = cupp.toStr();
  return atoi(substrBetween("\">", site , "</a>", site.find("...")).c_str());
}

void SJPCrawler::indexWords(size_t startPage, size_t endPage, bool verbose){
  int j{0};
  size_t i{0};
  try{
    string url, word, comments, site, date;
    SQLite::Database db("words.db", SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE);
    db.exec("DROP TABLE IF EXISTS words");
    SQLite::Transaction transaction(db);
    db.exec("CREATE TABLE words (word TEXT, url TEXT, comments INTEGER, lastComment DATE)");
    if (verbose){
      cout << "Info: Commencing indexing all word pages from " <<
        startPage << " to " << endPage << " on SJP..." << endl;
      cout << "Info: There are currently " << getWordsNmb() << " words on " << getPageCount() << " pages." << endl;
    }
    int startpoint, wordsOnPage{50};
    for (i = startPage; i < endPage + 1; i++){
      cupp.getPage("https://sjp.pl/slownik/lp.phtml?page=" + to_string(i));
      site = cupp.toStr();
      site = substrBetween("<table class=\"ltab\"", site, "</table>", 0);
      if(i == endPage){
        wordsOnPage = countInString(site, "<tr><td><a href=\"/");
      }
      startpoint = 0;
      for (j = 0; j < wordsOnPage; j++){
        url = substrBetween("<tr><td><a href=\"/", site, "\">", startpoint);
        word = substrBetween("\"/" + url + "\">", site, "</a>", startpoint);
        comments = to_string(getCommentsNmb("https://sjp.pl/" + url));
        if (atoi(comments.c_str()) > 0){
          date = "\"" + whenLastComment("https://sjp.pl/" + url) + "\"";
        }
        else{
          date = "NULL";
        }
        db.exec("INSERT INTO words VALUES (\"" + word + "\",\"https://sjp.pl/"
          + url + "\"," + comments + "," + date +")");
        startpoint = site.find("\"/" + url + "\"", startpoint) + 1;
      }
      if (verbose){
        cout << "Info: Finished indexing page " <<  i << " ("
             << i - startPage + 1 << "/" << endPage - startPage + 1<< ").\n";
      }
    }
    if (verbose){
      cout << "Info: Saving words..." << endl;
    }
    transaction.commit();
    if (verbose){
      cout << "Info: Indexing complete!" << endl;
    }
  }
  catch(exception & e){
    cerr << "Error: Exception at page " << i << ", word " << j << ": " << e.what() << endl;
    exit(EXIT_FAILURE);
  }
}
