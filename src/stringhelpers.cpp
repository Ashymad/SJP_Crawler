#include "stringhelpers.h"

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

int countInString(const IconvString & sourceString, const std::string & stringToFind){
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

std::string substrBetween(const std::string & lhsstring, const IconvString & sourceString, const std::string & rhsstring, size_t beggining){
    size_t startPos, endPos;

    startPos = sourceString.find(lhsstring, beggining) + lhsstring.size();
    endPos = sourceString.find(rhsstring, startPos);
    return sourceString.substr(startPos, endPos - startPos);

}
