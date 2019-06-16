#include <cstdio>
#include <cstdlib>

#define MAX 100

class _UnionFind {
  private:

    int* data;
    int* size;
    int len;

  public:

    _UnionFind( int length ): data(new int[sizeof(int)*length+1]) , size(new int[sizeof(int)*length+1]), len(length+1) {
      for ( size_t i = 0; i < this->len; ++i ) this->data[i] = -1;
      for ( size_t i = 0; i < this->len; ++i ) this->size[i] = 1;
    }

    int _find( int node ) {
      while (this->data[node] != -1) node = this->data[node];
      return node;
    }

    void _union(int node1, int node2) {
      node1 = this->_find(node1);
      node2 = this->_find(node2);

      if ( node1 != node2 ) {
        this->data[node1] = node2;
        this->size[node2] = this->size[node2] + this->size[node1];
      }
    }

    int maxGroupLen() {
      int maxLen = 0;

      for ( size_t i = 0; i < this->len; ++i ) {
        if (this->data[i] == -1)
          maxLen = this->size[i] > maxLen ? this->size[i] : maxLen;
      }

      return maxLen;
    }

    void print() {
      for ( size_t i = 0; i < this->len; ++i ) std::printf("%d ", this->data[i]);
      std::printf("\n");
      for ( size_t i = 0; i < this->len; ++i ) std::printf("%d ", this->size[i]);
      std::printf("\n");
    }

    ~_UnionFind() {
      delete[] this->data;
    }

};

int main() {

  int n, n1, n2;
  char buff[MAX];

  std::fgets(buff, MAX, stdin);
  std::sscanf(buff, "%d", &n);

  _UnionFind set(n);

  while (std::fgets(buff, MAX, stdin)) {
    if ( buff[0] == '?' ) std::printf("%d\n", set.maxGroupLen());
    else {
      std::sscanf(buff, "%d %d", &n1, &n2);
      set._union(n1,n2);
    }
  }

  return 0;
}