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


// A4 Debug
// Michael Giancola
// 400372137

/// Code adapted from Textbook starts here ///

using namespace std;

typedef string Elem;                                                        //base element type

class LinkedBinaryTree {
    protected:
        struct Node {                                                       //a node in the tree
            Elem elt;                                                   //element value of node
            Node* par;                                                   //parent of node
            Node* left;                                                     //left child of node
            Node* right;                                                    //right child of node
            Node() : elt(), par(NULL), left(NULL), right(NULL) {}    //constructor
        };
    public:
        class Position {                                                    //position of node in the tree
            private:
                Node* v;                                                    //a pointer to the node
            public:
                Position(Node* _v = NULL) : v(_v) {}                        //constructor
                Elem& operator*()                                           //returns element of node
                { return v->elt; }
                Position left() const                                       //returns left child of node
                { return Position(v->left); }
                Position right() const                                      //returns right child of node
                { return Position(v->right); }
                Position parent() const                                     //reutrns parent of node 
                { return Position(v->par); }
                bool isRoot() const                                         //checks if the node is a root 
                { return v->par == NULL; }
                bool isExternal() const                                     //checks if the node is external (end of branch of tree)
                { return v->left == NULL && v->right == NULL; }
                friend class LinkedBinaryTree;                              //'friend class' gives tree access
        };
        typedef std::list<Position> PositionList;                           //list of positions in tree
    public:
        LinkedBinaryTree();                                                 //constructor
        int size() const;                                                   //number of nodes in tree
        bool empty() const;                                                 //checks if tree is empty
        Node* root() const;                                              //returns root of tree
        PositionList positions() const;                                     //lists the positions of nodes
        void addRoot();                                                     //adds root to tree
        void expandExternal(const Position& p);                             //adds nodes to external node (expands tree)
        Position removeAboveExternal(const Position& p);                    //removes node at position p and parent 
        friend LinkedBinaryTree createExpressionTree(string postfix);      
        void printExpression();                                             //prints expression tree
        void printHelp(Node* cur);                                          //helper for print function
        double evaluateExpression(double a, double b);                      //evaluates the expression tree
        double evaluateHelp(double a, double b, Node* cur);                 //helper for the evaluate function
        double getScore()const;                                             //gets score of the tree
        void setScore(double s);                                            //sets the score of the tree
        bool operator<(const LinkedBinaryTree& other) const {
            return score < other.score;
        }
        void destructorHelper(Node* root_node);
        LinkedBinaryTree(const LinkedBinaryTree& t);
        ~LinkedBinaryTree();
        LinkedBinaryTree& operator=(const LinkedBinaryTree& t);

    protected:                                                              //local utilities
        void preorder(Node* v, PositionList& pl) const; 
        Node* copyPreOrder(const Node* root);
    private:
        Node* _root;                                                       //pointer to the root of tree
        int n;                                                              //number of nodes in tree
        double score;                                                       //score of tree
};

// copy assignment operator
LinkedBinaryTree& LinkedBinaryTree::operator=(const LinkedBinaryTree& t) {
    if (this != &t) {
      // If tree already contains data, delete it
      if (_root != NULL)
      {
        destructorHelper(_root);
      }
      _root = copyPreOrder(t.root());
      score = t.getScore();
    }
    return *this;
  }

LinkedBinaryTree::LinkedBinaryTree()                                        //constructor
: _root(NULL), n(0) { }

  // copy constructor
  LinkedBinaryTree::LinkedBinaryTree(const LinkedBinaryTree& t) {
    _root = copyPreOrder(t.root());
    score = t.getScore();
  }

LinkedBinaryTree::~LinkedBinaryTree() { //destructor
    destructorHelper(_root);
}

void LinkedBinaryTree:: destructorHelper (LinkedBinaryTree:: Node* node){
    if (node == NULL)
    {
        return;
    }

    destructorHelper(node ->left);
    destructorHelper(node->right);
    delete node;
}

LinkedBinaryTree::Node* LinkedBinaryTree::copyPreOrder(const Node* root) {
  if (root == NULL) return NULL;
  Node* nn = new Node;
  nn->elt= root->elt;
  nn->left = copyPreOrder(root->left);
  if (nn->left != NULL) nn->left->par = nn;
  nn->right = copyPreOrder(root->right);
  if (nn->right != NULL) nn->right->par = nn;
  return nn;
}

int LinkedBinaryTree::size() const                                          //number of nodes in tree
{ return n; }

bool LinkedBinaryTree::empty() const                                        //checks if tree is empty
{ return size() == 0; }                                                     //if size equal 0 tree is empty

LinkedBinaryTree::Node* LinkedBinaryTree::root() const                   //get the root of tree
{ return _root; }

void LinkedBinaryTree::addRoot()                                            //add root to empty tree
{ _root = new Node; n = 1; }


void LinkedBinaryTree::expandExternal(const Position& p) {
    Node* v = p.v;                                                          //node at position p
    v->left = new Node;                                                     //add a new left child to node
    v->left->par = v;                                                    //v is the parent 
    v->right = new Node;                                                    //add a new right child to node
    v->right->par = v;                                                   //v is its parent
    n += 2;                                                                 // add two more nodes to size of tree
}

LinkedBinaryTree::Position LinkedBinaryTree::removeAboveExternal(const Position& p) {   //remove position p and parent
    Node* w = p.v; Node* v = w->par;                                                 //get node p and its parent
    Node* sibling = (w == v->left ? v->right : v->left);
    if (v == _root) 
    {                                                                                   //check if child of root
        _root = sibling;                                                                //make a sibling root
        sibling ->par = NULL;
    }
    else 
    {
        Node* gparent = v->par;                                                      //v's parent is w's grandparent

        if (v == gparent->left) 
        {
            gparent->left = sibling;                                                    //replace parent as a sibling
        }
        else 
        {
            gparent->right = sibling;
            sibling->par = gparent;
        }
    }
    delete w;                                                                           //delete removede nodes
    delete v;                                                                         
    n -= 2;                                                                             //tree becomes two notes smaller
    return Position(sibling);
}

LinkedBinaryTree::PositionList LinkedBinaryTree::positions() const {
    PositionList plist;
    preorder(_root, plist);                                                             //preorder traversal
    return PositionList(plist);                                                         //return resulting list
}

void LinkedBinaryTree::preorder(Node* v, PositionList& plist) const {
    plist.push_back(Position(v));                                                       //add this node to tree
    if (v->left != NULL)                                                                //traverse left subtree           
        preorder(v->left, plist);
    if (v->right != NULL)                                                               //traverse right subtree
        preorder(v->right, plist);
}

/// Code adapted from textbook ends here /// 


LinkedBinaryTree createExpressionTree(string postfix) {       //creates trees to be put into a vector
    stack<LinkedBinaryTree::Node*> stack;                        //stack of nodes to hold roots of tree
    stringstream ss(postfix);
    string tok;
    LinkedBinaryTree::Node *holder;                                      //temporary variable to hold the new tree every iteration
    int count = 0;                                             //a variable to count number of nodes in tree

    while (getline(ss, tok, ' '))                             //tokenizes string input for every space seperation
    {                                                           
        holder = new LinkedBinaryTree::Node();                          //creates new tree
        holder->elt = tok;
        holder->left = NULL;
        holder->right = NULL;
        count++;

        if ((isdigit(tok[0]) || isdigit(tok[1])) || (isalpha(tok[0]) && tok != "abs")) //if token is number or a/b, do nothing and push to stack
        {  
        }
        else if (tok == "abs")                                //the right node holds the other node in the stack if token is abs 
        {                                                  
            holder->right = stack.top();                   
            holder->right->par = holder;
            stack.pop();
            
        }
        else                                                    //if operator takes two most recent stack elements and makes them the children of the root 
        {                                               
            holder->right = stack.top();                   //take the top of the stake and make right and left pointers to then pop the stack element 
            holder->right->par = holder;
            stack.pop();
            holder->left = stack.top();
            holder->left->par = holder;
            stack.pop();
            
        } 
        stack.push(holder);                                //push the root of the tree to the stack
        //delete temp;                    
    }
    LinkedBinaryTree result;                               //by this point their should only be only thing left if the stack 
    result._root = stack.top();                            //create a new tree and set the stack element to the root of the new tree
    result.n = count;
    return result;
}

void LinkedBinaryTree::printExpression() {                      //helps to print expression
    printHelp(_root);
}

void LinkedBinaryTree::printHelp(Node *cur) {
    if (cur->elt == "abs")                                  //checks if element of current node is abs
    { 
        cout << "abs(";                                         //if abs recursively call helper for right child
        printHelp(cur->right);
        cout << ")";
    }
    else if(cur->left == NULL && cur->right == NULL)            //checks if current node is leaf node (external) 
    {       
        cout << cur->elt;                                   //if true prints element of current
    }
    else                                                        //executes when there are both left and right nodes 
    {                                                      
        cout << "(";
        printHelp(cur->left);
        cout << " " << cur->elt << " ";
        printHelp(cur->right);
        cout << ")";
    }
}

double LinkedBinaryTree::evaluateExpression(double a, double b) {               //evalutates expression
    return evaluateHelp(a,b, _root);
}

double LinkedBinaryTree::evaluateHelp(double a, double b, Node* cur) {          //functions that perform operations on tree
    if (cur->elt == "-") 
    {
        return evaluateHelp(a,b,cur->left) - evaluateHelp(a,b,cur->right);
    }
    else if (cur->elt == "+") 
    {
        return evaluateHelp(a,b,cur->left) + evaluateHelp(a,b,cur->right);
    }
    else if (cur->elt == "/") 
    {
        return evaluateHelp(a,b,cur->left) / evaluateHelp(a,b,cur->right);
    }
    else if (cur->elt == "*") 
    {
        return evaluateHelp(a,b,cur->left) * evaluateHelp(a,b,cur->right);
    }
    else if (cur->elt == "abs") 
    {
        return fabs(evaluateHelp(a,b,cur->right));              //fabs is a function used to calculate absolute value of floating point argument
    }
    else if (cur->elt == ">") 
    {
        if (evaluateHelp(a,b,cur->left) > evaluateHelp(a,b,cur->right)) return 1;
        else return -1;
    }
    else if (cur->elt == "b") 
    {
        return b;
    }
    else if (cur->elt == "a") 
    {
        return a;
    }
    else {
        return stod(cur->elt);
    }
}

double LinkedBinaryTree::getScore() const {
    return score;
}

void LinkedBinaryTree::setScore(double s) {
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

    //Sort trees by their score
    sort(trees.begin(), trees.end());
    for (auto& t : trees) {
        cout << "Exp ";
        t.printExpression();
        cout << " Score " << t.getScore() << endl;
    }
}