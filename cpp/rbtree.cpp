#include <iostream>
using namespace std;

#define RED     0
#define BLACK   1

template <class T>
struct Node
{
    T       val;
    bool    color;  /* 0-RED 1-BLACK */
    Node<T> *parent;
    Node<T> *left;
    Node<T> *right;

    Node(T val, bool color, Node<T> *parent)
    {
        this->val       = val;
        this->color     = color;
        this->parent    = parent;
        this->left      = NULL;
        this->right     = NULL;
    }
};

template <class T>
class RbTree
{
    Node<T> *root;

    RbTree()
    {
        root = NULL;
    }

    void rotateL(Node<T> *p)
    {
        Node<T> *par   = p->parent;
        Node<T> *gp    = par->parent;
        Node<T> *child = p->left;

        if (gp->left == par) { gp->left = p; }
        else { gp->right = p; }
        p->parent = gp;

        p->left = par;
        par->parent = p;

        par->right = child;
        child->parent = par;
    }

    void rotateR(Node<T> *p)
    {
        Node<T> *par   = p->parent;
        Node<T> *gp    = par->parent;
        Node<T> *child = p->right;

        if (gp->left == par) gp->left = p;
        else gp->right = p;
        p->parent = gp;

        p->right = par;
        par->parent = p;

        par->left = child;
        child->parent = par;
    }

    Node<T> *sibling(Node<T> *p)
    {
        if (p->parent->left == p) return p->parent->right;
        return p->parent->left;
    }

    bool find(Node<T> *p, T val)
    {
        if (p == NULL) return false;
        if (p->val == val) return true;
        if (p->val < val) return find(p->right, val);
        return find(p->left, val);
    }

    void rb_insert(Node<T> *p, T val)
    {
        if (p == NULL) {
            p = new Node<T>(val, BLACK, NULL);
            return;
        }

        Node<T> *insert = NULL;
        while (true) {
            T v = p->val;
            if (v == val) return;
            else if (v < val) {
                if (p->right != NULL) {
                    p = p->right;
                } else {
                    insert = p->right = new Node<T>(val, RED, p);
                    break;
                }
            }
            else {
                if (p->left != NULL) {
                    p = p->left;
                } else {
                    insert = p->left = new Node<T>(val, RED, p);
                    break;
                }
            }
        }

        insert_change(insert);
    }

    void insert_change(Node<T> *p)
    {
        if (p->parent == NULL) {
            p->color = BLACK;
            return;
        }

        if (p->parent->color == BLACK) return;

        /* parent is RED */
        if (sibling(p->parent)->color == RED) {
            sibling(p->parent)->color = BLACK;
            p->parent->color = BLACK;
            p->parent->parent->color = RED;
            insert_change(p->parent->parent);
        }

        // sibling is BLACK
        if (p == p->parent->left && p->parent == p->parent->parent->left) {
            rotateR(p->parent);
            p->parent->color = BLACK;
            sibling(p)->color = RED;
        }

        if (p == p->parent->left && p->parent == p->parent->parent->right) {
            rotateR(p);
            rotateL(p);
            p->color = BLACK;
            p->left->color = RED;
        }

        if (p == p->parent->right && p->parent == p->parent->parent->right) {
            rotateL(p->parent);
            p->parent->color = BLACK;
            p->parent->left->color = RED;
        }

        if (p == p->parent->right && p->parent == p->parent->parent->left) {
            rotateL(p);
            rotateR(p);
            p->color = BLACK;
            p->right->color = RED;
        }
    }

    Node<T> *findLeftMost(Node<T> *p)
    {
        if (p == NULL) {
            cout << "p is NULL" << endl;
            return NULL;
        }

        while (p->left != NULL) p = p->left;
        return p;
    }

    bool rb_delete(Node<T> *p, T val)
    {
        while (p != NULL) {
            if (p->val == val) {
                if (p->right != NULL) {
                    Node<T> *leftMost = findLeftMost(p->right);
                    p->val = leftMost->val;
                    rb_delete_one(leftMost);
                } else {
                    rb_delete_one(p);
                }
                delete p;
                p = NULL;
                return true;
            } else if (p->val < val) {
                p = p->right;
            } else {
                p = p->left;
            }
        }

        return false;
    }

    void rb_delete_one(Node<T> *p)
    {
        if (p->parent == NULL) {
            if (p->left == NULL && p->right == NULL) {
                root = NULL;
            } else if (p->left == NULL && p->right != NULL) {
                root = p->right;
                root->color = BLACK;
            } else if (p->right == NULL && p->left != NULL) {
                root = p->left;
                root->color = BLACK;
            }
            return;
        }

        /* if p is not root */
        Node<T> *child = p->left == NULL ? p->right : p->left;
        /* if child is NULL */
        //

        if (p == p->parent->left) p->parent->left = child;
        else if (p == p->parent->right) p->parent->right = child;

        if (child != NULL) child->parent = p->parent;

        if (p->color == BLACK) {
            if (child->color == RED) {
                child->color = BLACK;
            } else if (child->color == BLACK) {
                /* if child is NULL? */
                rb_delete_adjust(child);
            }
        }
    }

    void rb_delete_adjust(Node<T> *p)
    {
        if (p->parent == NULL) {
            p->color = BLACK;
            return;
        }

        if (sibling(p)->color == RED) {
            if (p == p->parent->left) {
                rotateL(sibling(p));
                p->parent->color = RED;
                p->parent->parent->color = BLACK;
            } else if (p == p->parent->right) {
                rotateR(sibling(p));
                p->parent->color = RED;
                p->parent->parent->color = BLACK;
            }
        }

        /* All black */
        if (p->parent->color == BLACK && sibling(p)->color == BLACK && sibling(p)->left->color == BLACK && sibling(p)->right->color == BLACK) {
            sibling(p)->color = RED;
            rb_delete_adjust(p->parent);
            return;
        }

        if (p->parent->color == RED && sibling(p)->color == BLACK && sibling(p)->left->color == BLACK && sibling(p)->right->color == BLACK) {
            p->parent->color = BLACK;
            sibling(p)->color = RED;
            return;
        }


        /* move RED Node<T> to the outside */
        if (p == p->parent->left && sibling(p)->left->color == RED && sibling(p)->right->color == BLACK) {
            rotateR(sibling(p)->left);
            p->parent->right->color = BLACK;
            p->parent->right->right->color = RED;
        } else if (p == p->parent->right && sibling(p)->left->color == BLACK && sibling(p)->right->color == RED) {
            rotateL(sibling(p)->right);
            p->parent->left->color = BLACK;
            p->parent->left->left->color = RED;
        }

        if (p == p->parent->left) {
            sibling(p)->right->color = BLACK;
            sibling(p)->color = p->parent->color;
            p->parent->color = BLACK;
            rotateL(sibling(p));
        } else if (p == p->parent->right) {
            sibling(p)->left->color = BLACK;
            sibling(p)->color = p->parent->color;
            p->parent->color = BLACK;
            rotateR(sibling(p));
        }
    }
};


int main()
{
    // test case
    return 0;
}

