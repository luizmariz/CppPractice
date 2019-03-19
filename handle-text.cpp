#include <iostream>
#include <cstring>
#include <cstdlib>

struct FoundIndex {
  size_t index;
  bool found;
};

template <typename T>
struct List {
  T* content;
  size_t size;
};

class _String {
  private:

    unsigned int strLength;
    char* strData;

    void str_copy( char* pDest, char* pSrc, size_t len) {
      for (size_t i = 0; i < len; ++i ) {
        pDest[i] = pSrc[i];
      }
    }

    int str_len(char* p) {
      if (p == NULL) return 0;

      int _len = 0;

      while (*p != '\0') {
        ++_len;
        ++p;
      }
      return _len;
    }

  public:

    _String(): strLength(0), strData(NULL) {}

    _String(char* text): strLength( str_len(text) ), strData( new char[this->strLength] ) {
      this->str_copy( this->strData, text, this->strLength);
    }

    _String(char c): strLength(1), strData( new char[1]) {
      this->strData[0] = c;
    }

    int len() { return this->strLength; }
    char* value() { return this->strData; }

    List<char*> split(const char& c) {
      size_t itemCounter = 0, splitPosition = 0, listIndex = 0;

      for ( size_t i = 0; i < this->strLength+1; ++i ) {
        if ( this->strData[i] == c || this->strData[i] == 0 ) ++itemCounter;
      }

      char** list = new char*[sizeof(char*)*itemCounter];

      for ( size_t i = 0; i < this->strLength+1; ++i ) {
        if ( this->strData[i] == c || this->strData[i] == 0 ) {

          list[listIndex] = new char[splitPosition+i+1];

          for ( size_t j = 0; j < i - splitPosition; ++j ) {
            list[listIndex][j] = this->strData[splitPosition + j];
          }

          list[listIndex][i-splitPosition] = '\0';

          ++listIndex;
          splitPosition = i+1;
        }

      }

      List<char*> l;
      l.content = list;
      l.size = itemCounter;

      return l;
    }

    void operator= (const _String& otherStr) {
      if ( this != &otherStr ) {
        delete[] this->strData;
        this->strLength = otherStr.strLength;
        this->strData = new char[this->strLength];
        this->str_copy( this->strData, otherStr.strData, this->strLength);
      }
    }

    char* operator+ (const _String& otherStr) {
      size_t newLength = (this->strLength + otherStr.strLength);
      char newStr[newLength+1];

      for (size_t i = 0; i < this->strLength; ++i) {
        newStr[i] = this->strData[i];
      }

      for (size_t i = this->strLength; i < newLength; ++i) {
        newStr[i] = otherStr.strData[i - this->strLength];
      }

      return newStr;
    }

    char& operator[] (int index ) {
      if( index < 0 || index > this->strLength ) {
	      std::cerr << "Invalid index";
	      exit(-1);
      }
      return this->strData[index];
    }

    friend std::istream& operator>> (std::istream& is, _String& s) {
      char* buff = new char[1024];

      while(scanf("%[^\n]%*c", buff)==1){
        s = s + "oi ";
      }
      //is.getline(buff, 1024, 0);
      //s = buff;

      for ( size_t i = 0; i < s.len(); ++i ) {
        if ( s[i] < 32 ) s[i] = ' ';
      }

      delete[] buff;
      return is;
    }

    friend std::ostream& operator<< (std::ostream& os, _String& s) {
      for (size_t i = 0; i != s.strLength; ++i) {
        os.put(s.strData[i]);
      }
      return os;
    }

    ~_String() {
      delete[] this->strData;
    }
};

template <typename T>
class _Array {
  private:

    T* data;
    size_t capacity;
    size_t length;

    void arrayCopy( T* pDest, T* pSrc, size_t len) {
      for (size_t i = 0; i < len; ++i ) {
        pDest[i] = pSrc[i];
      }
    }

  public:

    _Array(): data(NULL), capacity(0), length(0) {}

    _Array( int len, T element ): data( new T[sizeof(T)*len]), capacity(len), length(len) {
      for ( size_t i = 0; i < len; ++i) this->data[i] = element;
    }

    _Array( List<T> list ): data(new T[sizeof(T)*list.size]), capacity(list.size), length(list.size) {
      this->arrayCopy(this->data, list.content, this->length);
    }

    size_t len() { return this->length; }

    void push(const T element) {
      if (this->data == NULL) {
        this->data = new T[1];
        ++this->capacity;

      } else if (this->length == this->capacity) {
        T* newData = new T[sizeof(this->capacity*2)];

        for (size_t i = 0; i != this->capacity; ++i) {
          newData[i] = this->data[i];
        }

        delete[] this->data;
        this->data = newData;
        this->capacity *= 2;

      }

      this->data[this->length++] = element;

    }

    void print() {
      std::cout << "[ ";
      for (size_t i = 0; i != this->length; ++i) {
        std::cout << this->data[i] << (i != this->length -1 ?  ", " : " ]");
      }
      std::cout << std::endl;
    }

    void removeSwap(size_t index) {
      this->data[index] = this->data[--this->length];
    }

    void remove(size_t index) {
      for (size_t i = index; i <= this->length; ++i) {
        this->data[i] = this->data[i+1];
      }
      --this->length;
    }

    FoundIndex indexOf(const T element) {
      size_t index = 0;
      bool found = false;

      for (size_t i = 0; i != this->length; ++i) {
        if (element == this->data[i]) {
          index = i;
          found = true;
          break;
        }
      }

      FoundIndex f;
      f.index = index;
      f.found = found;
      return f;
    }

    void alphabeticSort() {
      bool changed;
      char* aux;

      do {
        changed = false;

        for (size_t i = 0; i < this->length-1; ++i) {
          if ( std::strcmp(this->data[i], this->data[i+1]) == 1) {
            aux = this->data[i];
            this->data[i] = this->data[i+1];
            this->data[i+1] = aux;

            changed = true;
          }
        }

      } while (changed);
    }

    _Array& operator= (const _Array& otherArray) {
      if ( this != &otherArray ) {
        delete[] this->data;
        this->length = otherArray.length;
        this->capacity = otherArray.capacity;
        this->data = new T[this->length];
        this->arrayCopy( this->data, otherArray.data, this->length);
      }
    }

    T& operator[] (int index ) {
      if( index < 0 || index > this->length ) {
	      std::cerr << "Invalid index";
	      exit(-1);
      }
      return this->data[index];
    }

    ~_Array() {
      delete[] this->data;
    }
};

int main() {

  _String str;

  std::cin >> str;
  std::cout << std::endl;

  _Array<char*> a = str.split(' ');
  _Array<int> b(a.len(), 1);
  a.alphabeticSort();

  for ( size_t i = 0 ; i < a.len(); ++i) {
    for (size_t j = i+1; j < a.len(); ++j) {
      if ( b[j] != 0 && std::strcmp(a[i], a[j]) == 0 ) {
        b[i] += 1;
        b[j] = 0;
      }
    }
  }

  a.print();
  b.print();
  for ( size_t i = 0; i < a.len(); ++i ) {
    if ( b[i] != 0 && a[i][0] > 32) std::cout << a[i] << ' ' << b[i] << std::endl;
  }

  return 0;
}




