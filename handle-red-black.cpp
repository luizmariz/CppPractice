#include <cstdio>
#include <cstdlib>

struct Node {
  struct Node* esq;
  struct Node* dir;
  int key;
  bool color;
};

class _AVLTree {
  private:

    Node* root;

    Node* createNode( int key ) {
      Node* pt = new Node;

      pt->esq = NULL;
      pt->dir = NULL;
      pt->key = key;
      pt->color = true;

      return pt;
    }

    Node* create1stNode( int key ) {
      Node* pt = new Node;

      pt->esq = NULL;
      pt->dir = NULL;
      pt->key = key;
      pt->color = false;

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
        std::printf("%d-- e:%d d:%d\n", pt->key, pt->esq? pt->esq->key:100, pt->dir ? pt->dir->key:100);
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

    Node* rotateLeft( Node *pt ) {
      Node* newParent;

      newParent = pt->dir;
      pt->dir = newParent->esq != NULL ? newParent->esq : NULL;
      newParent->esq = pt;

      newParent->color = pt->color;
      pt->color = true;

      return newParent;
    }

    Node* rotateRight( Node *pt ) {
      Node* newParent;

      newParent = pt->esq;
      pt->esq = newParent->dir != NULL ? newParent->dir : NULL;
      newParent->dir = pt;

      newParent->color = pt->color;
      pt->color = true;

      return newParent;
    }

    void flipColors( Node *pt ) {
      pt->color = true;
      pt->esq->color = false;
      pt->dir->color = false;
    }

    Node* balance( Node *pt ) {
      Node* newRoot = NULL;

      if ( pt != NULL ) {
        pt->esq = this->balance(pt->esq);
        pt->dir = this->balance(pt->dir);
      }

      if ( pt->dir->color && !pt->esq->color )  newParent = this->rotateLeft(pt);
      if ( pt->esq->color  &&  pt->esq->esq->color ) newParent = this->rotateRight(pt);
      if ( pt->esq->color && pt->dir->color ) this->flipColors(pt);

      return newRoot;
    }

  public:

    _AVLTree(): root( NULL ) {}

    void push(int key) {
      if ( this->root == NULL ) this->root = create1stNode(key);
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

  tree.push(1);
  tree.push(0);
  tree.push(2);
  tree.push(2);
  tree.push(3);
  tree.push(2);
  tree.push(2);
  tree.push(5);
  tree.push(17);
  tree.push(2);
  tree.print();

  std::printf("\n%d\n", tree.height());

  return 0;
}