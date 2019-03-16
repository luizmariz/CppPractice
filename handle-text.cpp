#include <iostream>

struct FoundIndex {
  size_t index = 0;
  bool found = false;
};

template <typename T> class _Array {
  private:

    T* data;
    size_t capacity;
    size_t length;

  public:

    _Array(): data(nullptr), capacity(0), length(0) {}

    size_t len() { return this->length; }

    void push(const T &&element) {
      if (this->data == nullptr) {
        this->data = new T[1];
        ++this->capacity;

      } else if (this->length == this->capacity) {
        T* new_data = new T[this->capacity*2];

        for (size_t i = 0; i != this->capacity; ++i) {
          new_data[i] = std::move(this->data[i]);
        }

        delete[] this->data;
        this->data = new_data;
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

    void remove_swap(size_t index) {
      this->data[index] = this->data[--this->length];
    }

    void remove(size_t index) {
      for (size_t i = index; i <= this->length; ++i) {
        this->data[i] = std::move(this->data[i+1]);
      }
      --this->length;
    }

    FoundIndex indexOf(const T &&element) {
      size_t index = 0;
      bool found = false;

      for (size_t i = 0; i != this->length; ++i) {
        if (element == this->data[i]) {
          index = i;
          found = true;
          break;
        }
      }
      return FoundIndex{index, found};
    }

    ~_Array() {
      delete[] this->data;
    }
};

class _String {
  private:

    unsigned int length;
    char* data;

    void str_copy( char* p_dest, char* p_src, int len) {
      for (size_t i = 0; i < len; ++i ) {
        p_dest[i] = std::move(p_src[i]);
      }
    }

    int str_len(char* p) {
      if (p == nullptr) {
        return 0;
      }

      int _len = 0;

      while (*p != '\0') {
        ++_len;
        ++p;
      }
      return _len;
    }

  public:

    _String(): length(0), data(nullptr) {}

    _String(char* text): length( str_len(text) ), data( new char[length] ) {
      this->str_copy( this->data, text, this->length);
    }

    int len() { return this->length; }

    _String& operator= (const _String& other_str) {
      if ( this != &other_str ) {
        delete[] this->data;
        this->length = other_str.length;
        this->data = new char[this->length];
        this->str_copy( this->data, other_str.data, this->length);
      }
    }

    friend std::istream& operator>> (std::istream& is, _String& s) {
      char* buff = new char[1024];
      is.getline(buff, 1024);
      s = buff;
      return is;
    }

    friend std::ostream& operator<< (std::ostream& os, _String& s) {
      for (size_t i = 0; i != s.length; ++i) {
        os.put(s.data[i]);
      }
      return os;
    }

    ~_String() {
      delete[] this->data;
    }
};

int main() {
  _String str = "Hello World";
  std::cout << str << std::endl;
  std::cout << "Digite um texto:";
  std::cin >> str;
  std::cout << str << std::endl;
  std::cout << str.len() << std::endl;
  _Array<int> a;
  a.push(5);
  a.push(4);
  a.push(3);
  a.print();
  a.remove(a.indexOf(5).index);
  a.print();
  std::cout << a.len() << std::endl;
  return 0;
}