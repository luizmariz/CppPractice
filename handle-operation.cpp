#include <cstdio>
#include <cstdlib>
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
      for (size_t i = 0; i < this->length; ++i) std::printf("%d ", int(this->data[i]));
    }

    ~_Stack() {
      delete[] this->data;
    }
};

template <typename T>
class _Queue {
  private:

    T* data;
    size_t capacity;
    size_t length;
    size_t out;

  public:

    _Queue(): data(NULL), capacity(0), length(0), out(0) {}

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
      if (this->out < this->length) return this->data[this->out++];
      else throw std::underflow_error("Queue overflow exception");
      --this->length;
    }

    T value() {
      if (this->out < this->length) return this->data[this->out];
      else throw std::runtime_error("Can't get value of an empty Queue");
    }

    void print() {
      for (size_t i = this->out; i < this->length; ++i) std::printf("%c ", (char)this->data[i]);
    }

    ~_Queue() {
      delete[] this->data;
    }
};

void expToPolishNotation(char *buff, _Stack<char> &s, _Queue<char> &q);
void solveAndPrintPoslih( _Queue<char> &q);

int main() {

  _Stack<char> s;
  _Queue<char> q;

  char* buff = new char[sizeof(char)*50];

  while (std::fgets(buff, 50, stdin)) {

    expToPolishNotation(buff, s, q);
    solveAndPrintPoslih(q);

  }

  delete[] buff;
  return 0;
}

void solveAndPrintPoslih( _Queue<char> &q) {

  _Stack<int> s;

  for ( size_t i = 0; i < q.len(); ++i ) {
    if (std::isdigit(q.value())) s.push((int)(q.value() - '0'));
    else {

      float res, x1, x2;

      x2 = s.pop();
      x1 = s.pop();

      switch (q.value()) {
        case '+': res = x1 + x2; break;
        case '-': res = x1 - x2; break;
        case '*': res = x1 * x2; break;
        case '/': res = x1 / x2; break;
        default : throw std::runtime_error("Illegal operator");
      }

      s.push(res);
    }
    q.pop();
  }

  s.print();
}

void expToPolishNotation(char *buff, _Stack<char> &s, _Queue<char> &q) {
  for ( size_t i = 0; i < 50; ++i ) {

    if ( buff[i] == '\0') {
      break;
    }

    if (std::isdigit(buff[i])) q.push(buff[i]);
    else {

      if (s.len() == 0) s.push(buff[i]);
      else {

        switch (buff[i]){
          case '(': break;

          case ')':
            while (s.value() != '(') {
              q.push(s.pop());
            }
            s.pop();
            break;

          case '*':
            while (s.value() == '*' || s.value() == '/') {
              q.push(s.pop());
            }
            break;

          case '/':
            while (s.value() == '*' || s.value() == '/') {
              q.push(s.pop());
            }
            break;

          default:
            while (s.value() == '*' || s.value() == '/' || s.value() == '+' || s.value() == '-') {
              q.push(s.pop());
            }
            break;
        }

        if (buff[i] != ')') s.push(buff[i]);
      }
    }
  }
}