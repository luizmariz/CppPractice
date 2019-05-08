#include <cstdio>
#include <cstdlib>

struct Node {
  struct Node* esq;
  struct Node* dir;
  int key;
};

class _AVLTree {
  private:

    Node* root;

    Node* createNode( int key ) {
      Node* pt = new Node;

      pt->esq = NULL;
      pt->dir = NULL;
      pt->key = key;

      return pt;
    }

    void addNodeToTree( Node *pt, int key) {

      if ( key < pt->key ) {
        if ( pt->esq == NULL ) pt->esq = this->createNode(key);
        else this->addNodeToTree(pt->esq, key);
      }

      if ( key >= pt->key ) {
        if ( pt->dir == NULL ) pt->dir = this->createNode(key);
        else this->addNodeToTree(pt->dir, key);
      }

    }

    void printPreorder( Node *pt ) {
      if ( pt != NULL ) {
        std::printf("%d\n", pt->key);
        this->printPreorder( pt->esq );
        this->printPreorder( pt->dir );
      }
    }

    void freeTree( Node *&pt ) {
      if ( pt != NULL ) {
        this->freeTree(pt->esq);
        this->freeTree(pt->dir);

        delete pt;
      }
    }

    int getMaxLevel( Node *pt ) {
      if ( pt != NULL && pt->esq == NULL && pt->dir == NULL ) {
        return 0;
      }

      if ( pt == NULL ) {
        return -1;
      }

      int levelEsq = 1+getMaxLevel(pt->esq), levelDir = 1+getMaxLevel(pt->dir);
      return levelEsq >= levelDir ? levelEsq : levelDir;
    }

    int getBalanceK( Node *pt ) {
      return pt == NULL ? -1 : this->getMaxLevel(pt->esq) - this->getMaxLevel(pt->dir);
    }

    Node* rotateLeft( Node *pt ) {
      Node* newParent;

      newParent = pt->dir;
      pt->dir = newParent->esq != NULL ? newParent->esq : NULL;
      newParent->esq = pt;

      return newParent;
    }

    Node* rotateRight( Node *pt ) {
      Node* newParent;

      newParent = pt->esq;
      pt->esq = newParent->dir != NULL ? newParent->dir : NULL;
      newParent->dir = pt;

      return newParent;
    }

    Node* balance( Node *pt ) {
      Node* newRoot = NULL;

      if ( pt != NULL ) {
        pt->esq = this->balance(pt->esq);
        pt->dir = this->balance(pt->dir);
      }

      int bk = this->getBalanceK(pt);

      if ( bk <= 1 && bk >= -1 ) newRoot = pt;

      if ( bk <= -2 ) {
        if ( this->getBalanceK(pt->dir) < 1 ) newRoot = this->rotateLeft(pt);
        else {
          pt->dir = this->rotateRight(pt->dir);
          newRoot = this->rotateLeft(pt);
        }
      }

      if ( bk >= 2 ) {
        if ( this->getBalanceK(pt->esq) < 1 ) newRoot = this->rotateRight(pt);
        else {
          pt->esq = this->rotateLeft(pt->esq);
          newRoot = this->rotateRight(pt);
        }
      }

      return newRoot;
    }

  public:

    _AVLTree(): root( NULL ) {}

    void push(int key) {
      if ( this->root == NULL ) this->root = createNode(key);
      else this->addNodeToTree(this->root, key);

      this->root = this->balance(this->root);
    }

    void print() {
      this->printPreorder(this->root);
    }

    int height() {
      return this->getMaxLevel(this->root);
    }

    ~_AVLTree() {
      this->freeTree(this->root);
    }
};

int main() {
  _AVLTree tree;
  int n;

  while (std::scanf("%d", &n) == 1) {
    tree.push(n);
  }

  tree.print();

  return 0;
}