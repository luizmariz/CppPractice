#include <cstdio>
#include <cstdlib>

struct Node {
  struct Node* esq;
  struct Node* dir;
  int key;
  bool color;
};

class _RBTree {
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

    Node* addNodeToTree( Node *pt, int key) {

      if ( pt == NULL ) {
        return this->createNode(key);
      }

      if ( key < pt->key ) pt->esq = this->addNodeToTree(pt->esq, key);
      if ( key >= pt->key ) pt->dir = this->addNodeToTree(pt->dir, key);

      return this->balance(pt);
    }

    int checkBHeight(Node *pt) {
      if (pt == NULL)
        return 0;
      else {
        int leftBlackHeight = checkBHeight(pt->esq) + (!this->isRed(pt->esq) ? 1 : 0);
        int rightBlackHeight = checkBHeight(pt->dir) + (!this->isRed(pt->dir) ? 1 : 0);

        if (leftBlackHeight != rightBlackHeight) return -1;
        return leftBlackHeight;
      }
    }

    void printPreorder( Node *pt ) {
      if ( pt != NULL ) {
        //std::printf("%d%c -- esq:%d, dir:%d\n", pt->key, pt->color ? 'R':'N', pt->esq == NULL ? 0 : pt->esq->key,pt->dir == NULL ? 0 : pt->dir->key);
        std::printf("%d%c\n", pt->key, pt->color ? 'R':'N');
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

    bool isRed( Node *pt ) {
      if (pt == NULL) return false;
      return pt->color;
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
      Node* newRoot = pt;

      if ( this->isRed(pt->dir) && !this->isRed(pt->esq) && this->isRed(pt) ) {
        //std::printf("vou girar pra esquerda, %d\n", pt->key);
        newRoot = this->rotateLeft(pt);
      }

      if ( this->isRed(pt->esq)  &&  this->isRed(pt->esq->esq) ) {
        //std::printf("vou girar pra direita, %d\n", pt->key);
        newRoot = this->rotateRight(pt);
      }

      if ( this->isRed(pt->esq) && this->isRed(pt->dir) ) {
        //std::printf("vou trocar a cor, %d\n", pt->key);
        this->flipColors(pt);
      }

      return newRoot;
    }

  public:

    _RBTree(): root( NULL ) {}

    void push(int key) {
      //std::printf("inseri %d\n\n", key);
      this->root = this->addNodeToTree(this->root, key);
      this->root->color = false;
    }

    void print() {
      this->printPreorder(this->root);
    }

    ~_RBTree() {
      this->freeTree(this->root);
    }
};

int main() {
  _RBTree tree;
  int n;

  while (std::scanf("%d", &n) == 1) {
    tree.push(n);
  }

  tree.print();

  return 0;
}