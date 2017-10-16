#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "IconvString.h"
#include <iconv.h>
#include <sqlite3.h>
#include <curl/curl.h>
#include <SQLiteCpp/SQLiteCpp.h>

using namespace std;

int countInString(const string & sourceString, const string & stringToFind){
  int i{ 0 }, size{ static_cast<int>(stringToFind.size()) }, count{ 0 };
  for (char c : sourceString){
    if (c == stringToFind[i]){
      i++;
    }
    else
      i = 0;
    if (i == size){
      count++;
      i = 0;
    }
  }
  return count;
}

string substrBetween(const string & lhsstring, const string & sourceString, const string & rhsstring, size_t beggining){
  size_t startPos, endPos;
  try {
    startPos = sourceString.find(lhsstring, beggining) + lhsstring.size();
    endPos = sourceString.find(rhsstring, startPos);
    return sourceString.substr(startPos, endPos - startPos);
  }
  catch(exception & e){
    cerr << "Error: Exception in function substrBetween: " << e.what() << endl;
    cout << "Arguments:\nlhsstring: " << lhsstring << "\nsourceString: "
	 << sourceString << "\nrhsstring: " << rhsstring << "\nbeggining: " << beggining << endl;
    cout << "Size of sourceString: " << sourceString.size() << endl;
    exit(EXIT_FAILURE);
  }
}

class cURLCpp{
  CURL* curl;
  IconvString IcStr;
  string _URL;

  static size_t write_data(void *contents, size_t size, size_t nmemb, IconvString & IS){
    try{
      string asdf = string((char*)contents,nmemb);
      cout << asdf << endl;
      IS.append(asdf);
    }
    catch (exception & e){
      cerr << "Error: in function write_data: " << e.what() << endl;
      exit(EXIT_FAILURE);
    }
    return size * nmemb;
  }

public:

  cURLCpp(){
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    _URL = "";
    if (!curl){
      cerr << "Erorr: Curl initialization failed" << endl;
      exit(EXIT_FAILURE);
    }
  }

  bool getPage(const string & URL){
    const int MAX_ATTEMPTS = 3;
    int attempt{MAX_ATTEMPTS};
    do {
      if(attempt < MAX_ATTEMPTS){
	if (attempt < 0){
	  cerr << "Error: Couldn't download " << URL << endl;
	  return false;
	}
	cerr << "Warning: site " << URL << " is empty. Attempting re-download (" << MAX_ATTEMPTS - attempt << " of " << MAX_ATTEMPTS << ")." << endl;
      }
      attempt--;
      IcStr.clear();
      curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &IcStr);
      curl_easy_perform(curl);
    } while (IcStr.size() < 1);
    if (countInString(IcStr,"charset=") == 0){
      cerr << "Warning: charset on site " << URL << " not found. Assuming UTF-8." << endl;
      _URL = URL;
      return true;
    }
    else{
      string charset = substrBetween("charset=",IcStr,"\"",0);
      IcStr.reencode(charset,"UTF-8");
      _URL = URL;
      return true;
    }
  }

  string currentURL(){
    return _URL;
  }

  string toStr(){
    return IcStr.str();
  }

  ~cURLCpp(){
    curl_easy_cleanup(curl);
    curl_global_cleanup();
  }
};

class SJPCrawler{
  cURLCpp cupp;

public:

  int getCommentsNmb(const string & URL){
    if (URL != cupp.currentURL())
      cupp.getPage(URL);
    return countInString(cupp.toStr(), "\">#</a>");
  }

  string whenLastComment(const string & URL){
    if (URL != cupp.currentURL())
      cupp.getPage(URL);
    string site = cupp.toStr();
    return substrBetween(">#</a> ",site,"</span>",site.rfind(">#</a> "));
  }

  int getWordsNmb(){
    string URL{"https://sjp.pl/slownik/lp.phtml"};
    if (URL != cupp.currentURL())
      cupp.getPage(URL);
    return atoi(substrBetween("&nbsp;<tt><b>", cupp.toStr(), "</b></tt>",0).c_str());
  }

  size_t getPageCount(){
    string URL{"https://sjp.pl/slownik/lp.phtml"};
    if (URL != cupp.currentURL())
      cupp.getPage(URL);
    string site = cupp.toStr();
    return atoi(substrBetween("\">", site , "</a>", site.find("...")).c_str());
  }

  void indexWords(size_t startPage, size_t endPage, bool verbose){
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
	if(i == endPage)
	  wordsOnPage = countInString(site, "<tr><td><a href=\"/");
	startpoint = 0;
	for (j = 0; j < wordsOnPage; j++){
	  url = substrBetween("<tr><td><a href=\"/", site, "\">", startpoint);
	  word = substrBetween("\"/" + url + "\">", site, "</a>", startpoint);
	  comments = to_string(getCommentsNmb("https://sjp.pl/" + url));
	  if (atoi(comments.c_str()) > 0)
	    date = "\"" + whenLastComment("https://sjp.pl/" + url) + "\"";
	  else
	    date = "NULL";
	  //cout << "Info: Executing: INSERT INTO words VALUES (\"" + word + "\",\"https://sjp.pl/"
	  //    + url + "\"," + comments + "," + date +")" << endl;
	  db.exec("INSERT INTO words VALUES (\"" + word + "\",\"https://sjp.pl/"
		  + url + "\"," + comments + "," + date +")");
	  startpoint = site.find("\"/" + url + "\"", startpoint) + 1;
	}
	if (verbose)
	  cout << "Info: Finished indexing page " <<  i << " ("
	       << i - startPage + 1 << "/" << endPage - startPage + 1<< ").\n";
      }
      if (verbose)
	{
	  cout << "Info: Saving words..." << endl;
	}
      transaction.commit();
      if (verbose)
	cout << "Info: Indexing complete!" << endl;
    }
    catch(exception & e){
      cerr << "Error: Exception at page " << i << ", word " << j << ": " << e.what() << endl;
      exit(EXIT_FAILURE);
    }
  }

};

int main(void){
  //SJPCrawler SJPC;
  //ofstream file;
  //file.open("test.html");
  //cout << SJPC.getCommentsNmb("https://sjp.pl/zds") << endl;
  //cout << "#" <<SJPC.whenLastComment("https://sjp.pl/zds") << "#" << endl;
  //cout << SJPC.getWordsNmb() << endl;
  //SJPC.indexWords(1,SJPC.getPageCount(),true);

  //cout << SJPC.getWordsNmb() << endl;

  //string site = SJPC.site2string("https://sjp.pl/%B1%E6%EA%B3%F1%F3%B6%BF%BC");
  //file.close();
  cURLCpp cupp;
  cupp.getPage("https://sjp.pl/zds");
  //cout << cupp.toStr() << endl;
  //file.close();
  system("read -n 1");
  return 0;
}
