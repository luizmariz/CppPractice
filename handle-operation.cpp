#include <cstdio>
#include <stdexcept>

template <typename T>
class _Stack {
  private:

    T* data;
    size_t capacity;
    size_t length;

  public:

    _Stack(): data(NULL), capacity(0), length(0) {}

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

    T pop() {
      if (this->length > 0) return this->data[--this->length];
      else throw std::underflow_error("Stack underflow exception");
    }

    T value() {
      if (this->length > 0) return this->data[this->length-1];
      else throw std::runtime_error("Can't get value of an empty stack");
    }

    void print() {
      std::printf("[ ");
      for (size_t i = 0; i < this->length; ++i) std::printf("%d ", int(this->data[i]));
      std::printf("]\n");
    }

    ~_Stack() {
      delete[] this->data;
    }
};

template <typename T>
struct List {
  T* content;
  size_t size;
};

int main() {

  _Stack<float> s;
  char* buff = new char[100];

  while (std::fgets(buff, 100, stdin)) {

    if (std::isdigit(buff[0])) s.push(std::atof(buff));
    else {

      float res, x1, x2;

      x2 = s.pop();
      x1 = s.pop();

      switch (buff[0]) {
        case '+': res = x1 + x2; break;
        case '-': res = x1 - x2; break;
        case '*': res = x1 * x2; break;
        case '/': res = x1 / x2; break;
        default : throw std::runtime_error("Illegal operator");
      }

      s.push(res);
    }
  }

  s.print();

  delete[] buff;
  return 0;
}
