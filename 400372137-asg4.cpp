#include <iostream>
#include <stack>
#include <vector>
#include <fstream>
#include <cmath>
#include <list>
#include <algorithm>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

typedef string Elem;                               // base element type

class LinkedBinaryTree {
    protected:
        struct Node {                              // a node of the tree
            Elem elt;                              // element value
            Node* par;                             // parent
            Node* left;                            // left child
            Node* right;                           // right child
            Node() : elt(), par(NULL), left(NULL), right(NULL) {} // constructor
        };
    public:
        class Position {                            // position in the tree
            private:
                Node* v;                                 // pointer to the node
            public:
                Position(Node* _v = NULL) : v(_v) {}     // constructor
                Elem& operator*()                          // get element
                { return v->elt; }
                Position left() const                     // get left child
                { return Position(v->left); }
                Position right() const                    // get right child
                { return Position(v->right); }
                Position parent() const                   // get parent
                { return Position(v->par); }
                bool isRoot() const                       // root of the tree?
                { return v->par == NULL; }
                bool isExternal() const                   // an external node?
                { return v->left == NULL && v->right == NULL; }
                friend class LinkedBinaryTree;            // give tree access
        };
        typedef std::list<Position> PositionList;     // list of positions
    public:
        LinkedBinaryTree();                           // constructor
        int size() const;                             // number of nodes
        bool empty() const;                           // is tree empty?
        Position root() const;                        // get the root
        PositionList positions() const;               // list of nodes
        void addRoot();                               // add root to empty tree
        void expandExternal(const Position& p);       // expand external node
        Position removeAboveExternal(const Position& p); // remove p and parent
        friend LinkedBinaryTree createExpressionTree(string postfix);
        void printExpression();                       // print expression tree
        void printHelp(Node* cur);                    //helper for print function
        double evaluateExpression(double a, double b); // evaluate expression tree
        double evaluateHelp(double a, double b, Node* cur); //helper for evaluate function
        double getScore();                      // get score
        void setScore(double s);                      // set score
        bool operator<(const LinkedBinaryTree& other) const { //compares scores overloading > operator
            return score < other.score;
        }
    protected:                                      // local utilities
        void preorder(Node* v, PositionList& pl) const; // preorder utility
    private:
        Node* _root;                                // pointer to the root
        int n;                                      // number of nodes
        double score;                               // score of tree
};

LinkedBinaryTree::LinkedBinaryTree() // constructor
: _root(NULL), n(0) { }

int LinkedBinaryTree::size() const // number of nodes
{ return n; }

bool LinkedBinaryTree::empty() const // is tree empty?
{ return size() == 0; }

LinkedBinaryTree::Position LinkedBinaryTree::root() const // get the root
{ return Position(_root); }

void LinkedBinaryTree::addRoot() // add root to empty tree
{ _root = new Node; n = 1; }

void LinkedBinaryTree::expandExternal(const Position& p) {
    Node* v = p.v;          // p’s node
    v->left = new Node;     // add a new left child
    v->left->par = v;       // v is its parent
    v->right = new Node;    // and a new right child
    v->right->par = v;      // v is its parent
    n += 2;                 // two more nodes
}

// remove p and parent
LinkedBinaryTree::Position LinkedBinaryTree::removeAboveExternal(const Position& p) {
    Node* w = p.v; Node* v = w->par; // get p’s node and parent
    Node* sib = (w == v->left ? v->right : v->left);
    if (v == _root) { // child of root?
        _root = sib; // . . .make sibling root
        sib->par = NULL;
    }
    else {
        Node* gpar = v->par; // w’s grandparent

        if (v == gpar->left) gpar->left = sib; // replace parent by sib
        else gpar->right = sib;
        sib->par = gpar;
    }
    delete w; delete v; // delete removed nodes
    n -= 2; // two fewer nodes
    return Position(sib);
}

LinkedBinaryTree::PositionList LinkedBinaryTree::positions() const {
    PositionList pl;
    preorder(_root, pl); // preorder traversal
    return PositionList(pl); // return resulting list
}

void LinkedBinaryTree::preorder(Node* v, PositionList& pl) const {
    pl.push_back(Position(v)); // add this node
    if (v->left != NULL) // traverse left subtree
        preorder(v->left, pl);
    if (v->right != NULL) // traverse right subtree
        preorder(v->right, pl);
}

//New code

LinkedBinaryTree createExpressionTree(string postfix) {       //creates trees to put into vector
    stack<LinkedBinaryTree::Node*> stack;                     //stack of nodes to hold roots
    stringstream ss(postfix);
    string tok;
    LinkedBinaryTree *holder;                                 //holder variable to hold the new tree every iteration
    int count = 0;                                            //variable to count number of nodes in tree

    while (getline(ss, tok, ' ')) {                           //tokenizes string for every space seperation
        holder = new LinkedBinaryTree();                      //creates new tree and initializes root
        holder->addRoot();
        holder->_root->elt = tok;
        holder->_root->left = NULL;
        holder->_root->right = NULL;
        count++;

        if ((isdigit(tok[0]) || isdigit(tok[1])) || (isalpha(tok[0]) && tok != "abs")) {  //if tok is number or a/b, do nothing and push to stack
        }
        else if (tok == "abs") {                           //if tok is abs then make the right node hold the other node in the stack
            holder->_root->right = stack.top();            //this is its own if statement as it works with only one child node
            holder->_root->right->par = holder->_root;
            stack.pop();
        }
        else {                                             //if operator take recent two stack elements and make them children of root
            holder->_root->right = stack.top();            //take stack top and set up the right and parent pointers then pop the stack element
            holder->_root->right->par = holder->_root;
            stack.pop();
            holder->_root->left = stack.top();
            holder->_root->left->par = holder->_root;
            stack.pop();
        } 
        stack.push(holder->_root);                         //push root of tree to the stack
    }
    LinkedBinaryTree result;                                  //should only be one element in the stack left
    result._root = stack.top();                               //create new tree and set stack element to root of tree, then return tree
    result.n = count;
    return result;
}

void LinkedBinaryTree::printExpression() {
    printHelp(_root);
}

void LinkedBinaryTree::printHelp(Node *cur) {
    if (cur->elt == "abs") { //checks if element of current node is 'abs'
        cout << "abs(";      //if true print 'abs'and recursively call helper for right child
        printHelp(cur->right);
        cout << ")";
    }
    else if(cur->left == NULL && cur->right == NULL) {    //checks if current node is leaf node    
        cout << cur->elt;
    }
    else {                                              //when current node has both left and right children
        cout << "(";
        printHelp(cur->left);
        cout << " " << cur->elt << " ";
        printHelp(cur->right);
        cout << ")";
    }
}

double LinkedBinaryTree::evaluateExpression(double a, double b) {
    return evaluateHelp(a,b, _root);
}

double LinkedBinaryTree::evaluateHelp(double a, double b, Node* cur) {  //checks the element value in the current node
    if (cur->elt == "+") {                                                  
        return evaluateHelp(a,b,cur->left) + evaluateHelp(a,b,cur->right); //if value is main binary operator, recursively evaluate left and right subtrees
    }
    else if (cur->elt == "-") {
        return evaluateHelp(a,b,cur->left) - evaluateHelp(a,b,cur->right);
    }
    else if (cur->elt == "*") {
        return evaluateHelp(a,b,cur->left) * evaluateHelp(a,b,cur->right);
    }
    else if (cur->elt == "/") {
        return evaluateHelp(a,b,cur->left) / evaluateHelp(a,b,cur->right);
    }
    else if (cur->elt == "abs") {                   //if abs, function evaluates right subtree
        return fabs(evaluateHelp(a,b,cur->right));  //fabs used to calculate absolute value of floating point argument
    }
    else if (cur->elt == ">") {
        if (evaluateHelp(a,b,cur->left) > evaluateHelp(a,b,cur->right)) return 1;
        else return -1;
    }
    else if (cur->elt == "a") {                     //if element is a or b, return value of corresponding variable
        return a;
    }
    else if (cur->elt == "b") {
        return b;
    }
    else {                                          //if element is none of the above, convert string to double and return
        return stod(cur->elt);
    }
}

double LinkedBinaryTree::getScore() { //returns score
    return score;
}

void LinkedBinaryTree::setScore(double s) { //set score to s
    score = s;
}

int main() {
    // Read postfix expressions into vector
    vector<LinkedBinaryTree> trees;
    ifstream exp_file("expressions.txt");
    string line;
    while (getline(exp_file, line)) {
        trees.push_back(createExpressionTree(line));
    }
    // Read input data into 2D vector
    vector<vector<double>> inputs;
    ifstream input_file("input.txt");
    while (getline(input_file, line)) {
        vector<double> ab_input;
        stringstream ss(line);
        string str;
        while (getline(ss, str, ' ')) {
            ab_input.push_back(stod(str));
        }
        inputs.push_back(ab_input);
    }
    // Evaluate expressions on input data
    for (auto& t : trees) {
        double sum = 0;
        for (auto& i : inputs) {
            sum += t.evaluateExpression(i[0], i[1]);
        }
        t.setScore(sum/inputs.size());
    }
    //trees[3].printExpression();
    //cout << trees[6].evaluateExpression(100, 300) << endl;
    //Sort trees by their score
    sort(trees.begin(), trees.end());
    for (auto& t : trees) {
        cout << "Exp ";
        t.printExpression();
        cout << " Score " << t.getScore() << endl;
    }
}
