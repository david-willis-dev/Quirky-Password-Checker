// REF: Understanding of red black tree via: https://www.programiz.com/dsa/red-black-tree
#include <iostream>
#include <string>
// #include <queue>
using namespace std;
#define MAX_LIMIT 10

// Red-Black Tree class:
class RBTree {
    // Each Node will be a Key/Value Pair, the tree will Search/Store based on the Key
    struct Node {
        float jaroVal; // The Key is the Jaro-Value of the Password
        string pass; // The Value is the given Password
        int origin; // The original position in the passwords text file
        bool color; // Red = true, Black = false
        Node *parent, *left, *right; // Keep track of Left, Right, and Parent nodes

        // Null node initializer
        Node()
        {
            jaroVal = -1;
            pass = "";
            // Null nodes are always black
            color = false;
            parent = left = right = nullptr;
        }
        Node(float jaroVal, string pass, int origin)
        {
            this->jaroVal = jaroVal;
            this->pass = pass;
            this->origin = origin;
            // Always Insert as a Red Node
            color = true;
            parent = nullptr;
        }
    };
    Node* root;
    Node* nullNode; // Attached to nodes with 0/1 children: Helps keep track of leaf nodes!
    unsigned int size = 0; // Keeps track of current size of tree, initialize to 0

public:
    RBTree();
    ~RBTree();
    void deleteTree(Node* n); // Deconstructor Helper Function
    // Inserts new node into tree
    void insert(const float& jaroVal, const string& pass, const int& origin);
    // Balances tree after insertion
    void treeUpkeep(Node* n);
    void leftRotate(Node* n);
    void rightRotate(Node* n);
    // Returns password and its origin index for quirk score calculations
    void getTop(string& pass, int& origin)
    {
        pass = root->pass;
        origin = root->origin;
    }

    // Prints reverse inorder traversal of tree
    void reverseInOrder();
    // Helper function for reverseInOrder()
    void reverseInOrderHelper(Node* n, int& count, int& limit);
};

// Default Constructor
RBTree::RBTree()
{
    nullNode = new Node;
    root = nullNode; // Assigns root as a null node
    size = 0; // Initializes size of tree to 0
}

// Deletes each node in the tree
RBTree::~RBTree()
{
    deleteTree(root);
}

// Deletes tree in post order traversal and only delete when size > 0
void RBTree::deleteTree(Node* n) {
    if(size > 0) {
        // If tree left doesn't exist (or is null node), don't traverse
        if(n->left != nullNode && n->left)
            deleteTree(n->left);
        // If tree right doesn't exist (or is null node), don't traverse
        if(n->right != nullNode && n->right)
            deleteTree(n->right);
        // Decrease size of tree and delete current node
        --size;
        delete n;
    }
}

// Insert a new node into the tree, and upkeep (balance) as needed
void RBTree::insert(const float& jaroVal, const string& pass, const int& origin)
{
    ++size;
    Node* n = new Node(jaroVal, pass, origin); // Create new leaf node
    n->left = n->right = nullNode; // Since it's a leaf: children are nullNode

    // If root doesn't exist, assign n as new root
    if(root == nullNode)
    {
        // Root must be Black!
        n->color = false;
        root = n;
        return;
    }

    Node* temp = root; // Temp node to navigate our tree and insert n in the correct spot
    Node* parent; // Parent keeps track of the node before any child nodes

    // While we haven't reached the end of our tree
    while(temp != nullNode)
    {
        parent = temp;
        // If temp's current value is greater than n's, traverse Left
        if(n->jaroVal < temp->jaroVal)
            temp = temp->left;
        else
            temp = temp->right;
    }
    // Once we've reached our destination, update n's parent
    n->parent = parent;

    // If n's value is less than its parent's, it's the left child
    if(n->jaroVal < parent->jaroVal)
        parent->left = n;
    else parent->right = n;

    // Current Node needs a grandparent for RBTree Balancing
    if(n->parent->parent) treeUpkeep(n);
}

// Upkeep the tree: (self-balancing and recoloring)
// Starting at inserted node, balance tree, moving up each iteration until the top (root) of the tree is reached.
void RBTree::treeUpkeep(Node* n)
{
    Node* unc;
    // Balance only if two consecutive Red nodes
    while (n->parent->color) {
        // Check whether uncle node is to left or right of grandparent
        if(n->parent == n->parent->parent->right)
        {
            unc = n->parent->parent->left;
            // If uncle is red => flip colors
            if(unc->color)
            {
                // Flipping colors of uncle, parent, and grandparent nodes
                n->parent->color = false;
                unc->color = false;
                n->parent->parent->color = true;
                // Uncle and Parent have been fixed, so move up to grandparent
                n = n->parent->parent;
            }
            // Else uncle is black => rotate
            else {
                if(n == n->parent->left)
                {
                    // move pointer to its parent and perform a
                    // right rotation at the node
                    n = n->parent;
                    rightRotate(n);
                }

                // recolor the parent and grandparent nodes
                n->parent->color = false;
                n->parent->parent->color = true;
                // perform a left rotate on n
                leftRotate(n->parent->parent);
            }
        }
        // Do same for the right:
        else {
            unc = n->parent->parent->right;
            // If uncle is red => flip colors
            if(unc->color)
            {
                // Flipping colors of uncle, parent, and grandparent nodes
                n->parent->color = false;
                unc->color = false;
                n->parent->parent->color = true;
                // Uncle and Parent have been fixed, so move up to grandparent
                n = n->parent->parent;
            }
            // Else uncle is black => rotate
            else {
                if(n == n->parent->right)
                {
                    // move pointer to its parent and perform a
                    // left rotation at the node
                    n = n->parent;
                    leftRotate(n);
                }

                // Recolor the parent and grandparent nodes
                n->parent->color = false;
                n->parent->parent->color = true;
                // Perform a right rotate on n
                rightRotate(n->parent->parent);
            }
        }
        if(n == root) break;
    }
    // Ensure root is still colored black
    root->color = false;
}

// Left Rotate:
void RBTree::leftRotate(Node* n)
{
    // Store right child node of n
    Node* child = n->right;
    // Reassign child node's parent as n's parent (grandparent node)
    child->parent = n->parent;
    // If nullptr, then reassign root as child
    if(n->parent == nullptr)
        root = child;
    // If n is left child of its parent, parent's left child is now the child node
    else if(n->parent->left == n)
        n->parent->left = child;
    // If n is right child of its parent, parent's right child is now the child node
    else n->parent->right = child;
    // n's parent is now child
    n->parent = child;
    // If it wasn't empty, the left subtree's parent is now n
    if(child->left != nullNode)
        child->left->parent = n;
    // n's right subtree is now child's left
    n->right = child->left;
    // Finally, child's left subtree is now n
    child->left = n;
}

// Right Rotate:
void RBTree::rightRotate(Node* n)
{
    // Store left child node of n
    Node* child = n->left;
    // Reassign child node's parent as n's parent (grandparent node)
    child->parent = n->parent;
    if(n->parent == nullptr)
        root = child;
    // If n is right child of its parent, parent's right child is swapped
    else if(n->parent->right == n)
        n->parent->right = child;
    // Otherwise swap left and child
    else
        n->parent->left = child;
    // n's parent is now child
    n->parent = child;
    // If it wasn't empty, the right subtree's parent is now n
    if(child->right != nullNode)
        child->right->parent = n;
    // n's left subtree is now child's right
    n->left = child->right;
    // Finally, child's right subtree is now n
    child->right = n;
}

// Print In Order Traversal of tree:
void RBTree::reverseInOrder()
{
    // Initializing values to be passed into helper print function
    int count = 0, limit = MAX_LIMIT;
    reverseInOrderHelper(root, count, limit);
}

// Helper function for inOrder Print: print from the largest to smallest jaro values
void RBTree::reverseInOrderHelper(Node* n, int& count, int& limit)
{
    // If right child is not a null node (empty tree), traverse right: traverse the number of nodes we need to (limit)
    if(n->right != nullNode && count <= limit)
        reverseInOrderHelper(n->right, count, limit);
    // Print nodes current count, password, and jaro value: print as many as specified by limit
    if(++count <= limit)
        cout << count << ": " << n->pass << ", " << n->jaroVal << "\n";
    // If left child is not a null node (empty tree), traverse left: traverse the number of nodes we need to (limit)
    if(n->left != nullNode && count <= limit)
        reverseInOrderHelper(n->left, count, limit);
}

/*
// Main (Was used to test tree efficiency, comment out cause not needed!)
int main() {
    // Set precision to 5 decimal points to ensure no duplicate jaro's
    RBTree myTree;
    myTree.insert(5.5, "a", -1);
    myTree.insert(4.0, "b", -1);
    myTree.insert(3.5, "c", -1);
    myTree.insert(3.0, "d", -1);
    myTree.insert(2.5, "e", -1);
    myTree.insert(2.0, "f", -1);
    // myTree.levelPrint();
    myTree.reverseInOrder();
    myTree.~RBTree();
    return 0;
}
*/
