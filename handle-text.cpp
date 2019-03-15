#include <iostream>

struct FoundIndex {
  size_t index = 0;
  bool found = false;
};

template <typename T> class Vector {

  T* data = nullptr;
  size_t capacity = 0;
  size_t length = 0;

  public:

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
      for (size_t i = 0; i != this->length; ++i) {
        std::cout << this->data[i] << ", ";
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

  ~Vector() {
    delete[] this->data;
  }
};

int main() {
  //std::cout << "Digite um texto:";

  Vector<int> a;
  a.push(5);
  a.push(4);
  a.push(3);
  a.print();
  a.remove(a.indexOf(5).index);
  a.print();
  std::cout << a.len();
  return 0;
}