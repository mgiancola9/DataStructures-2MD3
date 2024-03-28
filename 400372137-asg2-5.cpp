#include <cstdlib>
#include <iostream>

typedef std::string Elem;
using namespace std;

class DNode {  // doubly linked list node
 private:
  Elem elem;                 // node element value
  int score;                 // variable to keep track of score
  DNode* prev;               // previous node in list
  DNode* next;               // next node in list
  friend class DLinkedList;  // allow DLinkedList access
};

class DLinkedList {  // doubly linked list
 public:
  DLinkedList();                 // constructor
  ~DLinkedList();                // destructor
  bool empty() const;            // is list empty?
  const Elem& front() const;     // get front element
  const Elem& back() const;      // get back element
  void addFront(const Elem& e);  // add to front of list
  void addBack(const Elem& e);   // add to back of list
  void removeFront();            // remove from front
  void removeBack();             // remove from back

  //New code
  int Size() const;              //gets the number of elements in 'scores' list
  void AddScoreInOrder(std::string name, int score);
  void RemoveScore(int index);
  bool UpdateScore(std::string name, int score);
  void Print();
  DLinkedList(const DLinkedList &passing);
  void operator = (const DLinkedList &passing);

 private:                        // local type definitions
  DNode* header;                 // list sentinels
  DNode* trailer;

 protected:                           // local utilities
  void add(DNode* v, const Elem& e);  // insert new node before v
  void remove(DNode* v);              // remove node v
};

DLinkedList::DLinkedList() {  // constructor
  header = new DNode;         // create sentinels
  trailer = new DNode;
  header->next = trailer;  // have them point to each other
  trailer->prev = header;
  header->prev = NULL;
  trailer->next = NULL;
}

DLinkedList::~DLinkedList() {  // destructor
  while (!empty()) {
    removeFront();  //  remove all but sentinels
  }
  delete header;  //  remove the sentinels
  delete trailer;
}

bool DLinkedList::empty() const {  // is list empty?
  return (header->next == trailer);
}

const Elem& DLinkedList::front() const {  // get front element
  return header->next->elem;
}

const Elem& DLinkedList::back() const {  // get back element
  return trailer->prev->elem;
}

void DLinkedList::add(DNode* x, const Elem& value) {
  DNode* newNode = new DNode;
  newNode->elem = value;
  newNode->next = x->next;
  newNode->prev = x;
  x->next->prev = newNode;
  x->next = newNode;
}

void DLinkedList::addFront(const Elem& e) {  // add to front of list
  add(header, e);
}

void DLinkedList::addBack(const Elem& e) {  // add to back of list
  add(trailer->prev, e);
}

void DLinkedList::remove(DNode* v) {  // remove node v
  DNode* u = v->prev;                 // predecessor
  DNode* w = v->next;                 // successor
  u->next = w;                        // unlink v from list
  w->prev = u;
  delete v;
}

void DLinkedList::removeFront() {  // remove from font
  remove(header->next);
}

void DLinkedList::removeBack() {  // remove from back
  remove(trailer->prev);
}

void listReverse(DLinkedList& L) {  // reverse a list
  DLinkedList T;                    // temporary list
  while (!L.empty()) {              // reverse L into T
    Elem s = L.front();
    L.removeFront();
    T.addFront(s);
  }
  while (!T.empty()) {  // copy T back to L
    Elem s = T.front();
    T.removeFront();
    L.addBack(s);
  }
}




//New code
int DLinkedList::Size() const{  // remove front item
  static DNode* cur = header->next;
  static int count = 0;
  //if next element is end of list, return the count
  if (cur->next == trailer) 
  {
    return count;
  } 
  else 
  //if its not the end of the list, increment count and set the cureent value to the next value, recursively calling the size function
  {
    count++;
    cur = cur->next;
    Size();
    return count + 1;
  }
}

void DLinkedList::AddScoreInOrder(std::string name, int score) {
  // Make new node to connect to DLL and give it attributes of score and name
  DNode* node = new DNode;
  node->elem = name;
  node->score = score;

  //Make 2 pointers to move through DLL
  DNode* prev = header;
  DNode* cur = header->next;

  //only want to execute loop if in DLL
  while (cur!=trailer->next){
    if (cur == trailer || cur->score <= node->score){
      //putting new node into DLL
      node->next = cur;
      node->prev = prev;
      prev->next = node;
      cur->prev = node;
      return;
    }
    //move to next node in DLL
    prev = cur;
    cur = cur->next;
  }
  return;
}

void DLinkedList:: RemoveScore(int index) {
    int size = Size();
    int i;
    DNode* cur;
    //split the list into two halves to increase efficiency
    if (index <= size/2) { //this is the half with the best scores
      cur = header->next; //start at the best score and work up the list
      i = 0;

      while (i != index) {
        cur = cur->next;
        i++;
      }

      (cur->prev)->next = cur->next;
      (cur->next)->prev = cur->prev;
    }
    else { //this is the half with the worse scores
      cur = trailer->prev; //start at the worst score and work down the list
      i = size;

      while (i != index) {
        cur = cur->prev;
        i--;
      }

      (cur->prev)->next = cur->next; //the next pointer of the node behind will skip over the removed node
      (cur->next)->prev = cur->prev; //the prev pointer of the node infront will do the same
    }
    delete cur; //delete the node once list pointers have been manipulated
}

//updates score of existing entry
bool DLinkedList::UpdateScore(string name, int score) {
    DNode* curFront = header->next;
    DNode* curBack = trailer->prev;  
    
    //using same idea where we traverse through front and back of list until we find correct node, then change value
    while (curFront != trailer || curBack != header) {
        if (curFront->elem == name) {
            curFront->score = score;
            return true;
        }
        else if (curBack->elem == name) {
            curBack->score = score;
            return true;
        }
        curFront = curFront->next;
        curBack = curBack->prev;
    }
    return false;
}

void DLinkedList::Print() {
  DNode* cur = header->next;
  while (cur->next != trailer) {
    cout << "{" << cur->elem << "," << cur->score << "}->";
    cur = cur->next;
  }
  cout << "{" << cur->elem << "," << cur->score << "}" << endl;
  return;
}

//copy an existing list
DLinkedList::DLinkedList(const DLinkedList &passing) {
  //setting header and trailers
  header = new DNode;
  trailer = new DNode;
  header->prev = NULL;
  trailer->next = NULL;

  //setting nodes for node we are copying, and node we are copying to
  DNode* curPassing = (passing.header)->next;
  DNode* curNew = header;

  //traverse and copy until we hit end of existing list
  while (curPassing->next != NULL) {
    curNew->next = new DNode;           //allocate new memory, and set pointers of new node
    (curNew->next)->prev = curNew; 
    curNew = curNew->next;

    curNew->elem = curPassing->elem;             //copy values for new node
    curNew->score = curPassing->score;

    curPassing = curPassing->next;            //go to next node and repeat
  }
    curNew->next = trailer;             //set trailer of list and finish!
    trailer->prev = curNew;
}

//overloading equal operator
void DLinkedList::operator = (const DLinkedList &passing) {
  DLinkedList copy(passing);               //call copy constructor
}

int main() {
DLinkedList scores;
scores.AddScoreInOrder("Jeff", 7);
scores.AddScoreInOrder("Jen", 9);
scores.AddScoreInOrder("Ilya", 3);
scores.AddScoreInOrder("Sara", 10);
scores.AddScoreInOrder("Sam", 11);
// Test size function
cout << "Number of scores is " << scores.Size() << endl;
scores.Print();
// Test remove function
scores.RemoveScore(4);
cout << "Number of scores is now " << scores.Size() << endl;
scores.Print();
// Test update function
if (scores.UpdateScore("Jeff",7))
scores.Print();
// Test copy construcor
DLinkedList scores_copy_1(scores);
scores.UpdateScore("Jen",5);
scores.Print();
scores_copy_1.Print();
// Test assignment operator overload
DLinkedList scores_copy_2 = scores_copy_1;
scores_copy_1.UpdateScore("Jen",5);
scores_copy_1.Print();
scores_copy_2.Print();
}