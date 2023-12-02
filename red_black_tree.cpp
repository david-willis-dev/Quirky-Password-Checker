// REF: Understanding of red black tree via: https://www.programiz.com/dsa/red-black-tree and via lecture slides (COP3530 University of Florida Fall 2023)
#include <iostream>
#include <string>
#include <queue>
using namespace std;

class RBTree {
    struct Node {
        // Each Node will be a Key/Value Pair
        // The Tree will Search/Store based on the Key
        // The Key is the Jaro-Value of the Password
        float jaroVal;
        // The Value is the given Password
        string pass;
        // Red = true, Black = false
        bool color;
        // Keep track of Left, Right, and Parent nodes
        Node *parent, *left, *right;
        // Null node initializer
        Node() {
            jaroVal = -1;
            pass = "";
            // Null nodes are always black
            color = false;
            parent = left = right = nullptr;
        }
        Node(float jaroVal, string pass) {
            this->jaroVal = jaroVal;
            this->pass = pass;
            // Always Insert as a Red Node
            color = true;
            parent = nullptr;
        }
    };
    Node* root;
    // Helper node to ensure invalid addresses aren't accessed in opertations
    // Attached to nodes with 0/1 children
    Node* nullNode;
    public:
    // Default Constructor
    RBTree();
    void insert(const float& jaroVal, const string& pass);
    void treeUpkeep(Node* n);
    void leftRotate(Node* n);
    void rightRotate(Node* n);
    // Silly Function
    void levelPrint();
    void postOrder();
    void postOrderHelper(Node* n);
};

RBTree::RBTree() {
    nullNode = new Node;
    root = nullNode;
}

// Insert a new node into the tree, and upkeep (balance) as needed
void RBTree::insert(const float& jaroVal, const string& pass) {
    // Create new leaf node
    Node* n = new Node(jaroVal, pass);
    // Assign children as null node
    n->left = n->right = nullNode;
    // If root doesn't exist, assign n as new root
    if(root == nullNode) {
        // Root must be Black
        n->color = false;
        root = n;
        return;
    }
    Node* temp = root;
    Node* parent;
    while(temp != nullNode) {
        parent = temp;
        if(n->jaroVal < temp->jaroVal)
            temp = temp->left;
        else temp = temp->right;
    }
    n->parent = parent;
    if(n->jaroVal < parent->jaroVal) 
        parent->left = n;
    else parent->right = n;
    // Current Node needs a grandparent for RBTree Balancing
    if(n->parent->parent) treeUpkeep(n);
}

// Upkeep the tree: (self balancing and recoloring)
// Starting at inserted node, balance tree, moving up each iteration
// until the top (root) of the tree is reached.
void RBTree::treeUpkeep(Node* n) {
    Node* unc;
    // Balance only if two consecutive Red nodes
    while (n->parent->color) {
        // Check whether uncle node is to left or right of grandparent
        if(n->parent == n->parent->parent->right) {
            // Uncle is left child of grandparent
            unc = n->parent->parent->left;
            // If uncle is red => flip colors
            if(unc->color) {
                // Flipping colors of uncle, parent, and grandparent nodes
                n->parent->color = false;
                unc->color = false;
                n->parent->parent->color = true;
                // Uncle and Parent have been fixed, so move up to grandparent
                n = n->parent->parent;
            }
            // Else uncle is black => rotate
            else {
                if(n == n->parent->left) {
                    n = n->parent;
                    rightRotate(n);
                }
                n->parent->color = false;
                n->parent->parent->color = true;
                leftRotate(n->parent->parent);
            }
        }
        else {
            // Uncle is right child of grandparent
            unc = n->parent->parent->right;
            if(unc->color) {
                n->parent->color = false;
                unc->color = false;
                n->parent->parent->color = true;
                n = n->parent->parent;
            }
            else {
                if(n == n->parent->right) {
                    n = n->parent;
                    leftRotate(n);
                }
                n->parent->color = false;
                n->parent->parent->color = true;
                rightRotate(n->parent->parent);
            }
        }
        // If we've reached the top of the tree (root) stop
        if(n == root) break;
    }
    root->color = false;
}

// Left Rotate:
void RBTree::leftRotate(Node* n) 
{
    // Store right child node of n
    Node* child = n->right;
    // Reassign child node's parent as n's parent (grandparent node)
    child->parent = n->parent;
    // Find n's parent
    // If nullptr, then reassign root as child
    if(n->parent == nullptr) root = child;
    // If n is left child of its parent, parent's left child is now the child node
    else if(n->parent->left == n) n->parent->left = child;
    // If n is right child of its parent, parent's right child is now the child node
    else n->parent->right = child;
    // n's parent is now child
    n->parent = child;
    // if child's left subtree was empty, do nothing
    // if it wasn't empty, the left subtree's parent is now n
    if(child->left != nullNode) child->left->parent = n;
    // n's right subtree is now child's left
    n->right = child->left;
    // finally, child's left subtree is now n
    child->left = n;
}

// Right Rotate:
void RBTree::rightRotate(Node* n)
{
    // Store left child node of n
    Node* child = n->left;
    // Reassign child node's parent as n's parent (grandparent node)
    child->parent = n->parent;
    // Find n's parent
    // If nullptr, then reassign root as child
    if(n->parent == nullptr) root = child;
    // If n is right child of its parent, parent's right child is swapped
    else if(n->parent->right == n) n->parent->right = child;
    // If n is left child of its parent, parent's left child is swapped
    else n->parent->left = child;
    // n's parent is now child
    n->parent = child;
    // if child's right subtree was empty, do nothing
    // if it wasn't empty, the right subtree's parent is now n
    if(child->right != nullNode) child->right->parent = n;
    // n's left subtree is now child's right
    n->left = child->right;
    // finally, child's right subtree is now n
    child->right = n;
}

// Print level order of tree:
void RBTree::levelPrint() {
    queue<Node*> q;
    Node* temp;
    q.push(root);
    while(!q.empty()) {
        temp = q.front();
        q.pop();
        cout << temp->pass << " ";
        if(temp->left != nullNode) 
            q.push(temp->left);
        if(temp->right != nullNode) 
            q.push(temp->right);
    }
    cout << endl;
}

// Print In Order Traversal of tree:
void RBTree::postOrder() {
    postOrderHelper(root);
}
// Helper function for inOrder Print
void RBTree::postOrderHelper(Node* n) {
    if(n->right != nullNode) postOrderHelper(n->right);
    cout << n->jaroVal << " ";
    if(n->left != nullNode) postOrderHelper(n->left);
}

// Main (Was used to test tree efficiency, comment out cause not needed!)
int main() {
    // Set precision to 5 decimal points to ensure no duplicate jaro's
    RBTree myTree;
    myTree.insert(5.5, "a");
    myTree.insert(4.0, "b");
    myTree.insert(3.5, "c");
    myTree.insert(3.0, "d");
    myTree.insert(2.5, "e");
    myTree.insert(2.0, "f");
    myTree.levelPrint();
    myTree.postOrder();
    return 0;
}
