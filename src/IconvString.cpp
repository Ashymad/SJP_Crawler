#include "IconvString.h"
#include "stringhelpers.h"

using namespace std;

std::ostream & operator<<(std::ostream & os, const IconvString & ic){
    for (const char c : ic)
        os << c;
    return os;
}

//{class IconvString

std::string IconvString::substr(size_t startPos, size_t length) const{
    return std::string(str_1 + startPos,length);
}

size_t IconvString::find(const std::string & str, size_t pos) const{
    size_t i{0}, length{str.size()};
    IconvString::iterator it;
    for (it = begin() + pos; it != end(); it++){
        if(*it == str[i])
            i++;
        else
            i = 0;
        if (i == length)
            return (it - i + 1) - begin();
    }
    return _size + 1;
}

void IconvString::swapstr(){
    char * tmpp;
    size_t tmpc;
    tmpp = str_1;
    str_1 = str_2;
    str_2 = tmpp;
    tmpc = _capacity;
    _capacity = _capacity_2;
    _capacity_2 = tmpc;
}

IconvString::IconvString(){
    str_1 = nullptr;
    str_2 = nullptr;
    _size = 0;
    _capacity_2 = 0;
    _capacity = 0;
}

IconvString::IconvString(const std::string & sourceString){
    _size = sourceString.size();
    _capacity = _size;
    str_1 = new char[_size];
    str_2 = nullptr;
    _capacity_2 = 0;
    for (size_t i{0}; i < _size; i++){
        str_1[i] = sourceString[i];
    }
}

size_t IconvString::size() const{
    return _size;
}

IconvString& IconvString::operator=(const std::string & rhs){
    _size = rhs.size();
    if(_size > _capacity){
        swapstr();
    }
    if(_size > _capacity){
        delete[] str_1;
        str_1 = nullptr;
        _capacity = _size;
        str_1 = new char[_size];
    }
    for (size_t i{0}; i < _size; i++){
        str_1[i] = rhs[i];
    }
    return *this;
}

void IconvString::append(const std::string & rhs){
    size_t oldsize = _size;
    _size += rhs.size();
    if(_size > _capacity){
        if(_size > _capacity_2){
            delete[] str_2;
            str_2 = nullptr;
            _capacity_2 = _size*2;
            str_2 = new char[_size*2];
        }

        for (size_t i{0}; i < oldsize; i++){
            str_2[i] = str_1[i];
        }
        swapstr();
    }
    for (size_t i = oldsize; i < _size; i++){
        str_1[i] = rhs[i - oldsize];
    }
}

std::string IconvString::operator=(const IconvString & other) const{
    return other.str();
}

char IconvString::operator[](size_t index) const{
    return str_1[index];
}

IconvString::operator std::string() const{
    return str();
}

void IconvString::clear(){
    _size = 0;
}

void IconvString::reencode(const std::string & source_encoding, const std::string & target_encoding){
    if (target_encoding != source_encoding){
        if (_capacity_2 < _size*2){
            delete[] str_2;
            str_2 = nullptr;
            str_2 = new char[_size*2];
            _capacity_2 = _size*2;
        }
        char * pIn = str_1;
        char * pOut = str_2;
        size_t inBytesLeft = _size;
        size_t outBytesleft = _capacity_2;
        conv = iconv_open(target_encoding.c_str(),source_encoding.c_str());
        iconv(conv, &pIn, &inBytesLeft, &pOut, &outBytesleft);
        _size = _capacity_2 - outBytesleft;
        iconv_close(conv);
        swapstr();
    }
}

std::string IconvString::str() const{
    return std::string(str_1,_size);
}

IconvString::~IconvString(){
    delete[] str_1;
    delete[] str_2;
    str_1 = nullptr;
    str_2 = nullptr;
}

//}class IconvString

//{class IconvString::iterator

IconvString::iterator::iterator(char * p) : ptr(p) {}

IconvString::iterator::iterator(const IconvString::iterator & it) :
    ptr(it.ptr) {}

IconvString::iterator & IconvString::iterator::operator++() {
    ++ptr;
    return *this;
}

IconvString::iterator IconvString::iterator::operator++(int){
    IconvString::iterator tmp(*this);
    ++ptr;
    return tmp;
}

IconvString::iterator & IconvString::iterator::operator=(const IconvString::iterator & other){
    ptr = other.ptr;
    return *this;
}

IconvString::iterator IconvString::iterator::operator+(int rhs) const{
    return IconvString::iterator(ptr + rhs);
}

IconvString::iterator IconvString::iterator::operator-(int rhs) const{
    return IconvString::iterator(ptr - rhs);
}

int IconvString::iterator::operator-(const IconvString::iterator & rhs) const{
    return static_cast<int>(ptr - rhs.ptr);
}

bool IconvString::iterator::operator==(const IconvString::iterator & rhs) const{
    return ptr == rhs.ptr;
}

bool IconvString::iterator::operator!=(const IconvString::iterator & rhs) const{
    return ptr != rhs.ptr;
}

char IconvString::iterator::operator*() const{
    return *ptr;
}

IconvString::iterator IconvString::begin() const{
    return IconvString::iterator(str_1);
}

IconvString::iterator IconvString::end() const{
    return IconvString::iterator(str_1 + _size);
}

//}class IconvString::iterator
