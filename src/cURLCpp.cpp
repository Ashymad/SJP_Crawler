#include <iostream>
#include "cURLCpp.h"
#include "stringhelpers.h"

using namespace std;

size_t cURLCpp::write_data(void *contents, size_t size, size_t nmemb, IconvString & IS){
  try{
    IS.append(string((char*)contents,nmemb));
  }
  catch (exception & e){
    cerr << "Error: in function write_data: " << e.what() << endl;
    exit(EXIT_FAILURE);
  }
  return size * nmemb;
}

cURLCpp::cURLCpp(){
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  _URL = "";
  if (!curl){
    cerr << "Erorr: Curl initialization failed" << endl;
    exit(EXIT_FAILURE);
  }
}

bool cURLCpp::getPage(const string & URL){
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
    string charset = substrBetween("charset=\"",IcStr,"\"",0);
    IcStr.reencode(charset,"UTF-8");
    _URL = URL;
    return true;
  }
}

string cURLCpp::currentURL(){
  return _URL;
}

string cURLCpp::toStr(){
  return IcStr.str();
}

cURLCpp::~cURLCpp(){
  curl_easy_cleanup(curl);
  curl_global_cleanup();
}
