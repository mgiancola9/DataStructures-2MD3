#include <cstdlib>
#include <iostream>
using namespace std;

template <typename E>  // forward declaration of SLinkedList
class SLinkedList;

template <typename E>
class SNode {  // singly linked list node
 private:
  E elem;                       // linked list element value
  SNode<E>* next;               // next item in the list
  friend class SLinkedList<E>;  // provide SLinkedList access
};

template <typename E>
class SLinkedList {  // a singly linked list
 public:
  SLinkedList();              // empty list constructor
  ~SLinkedList();             // destructor
  bool empty() const;         // is list empty?
  const E& front() const;     // return front element
  void addFront(const E& e);  // add to front of list
  void removeFront();         //  remove front of list

  //New code
  int numberOfNodes();         //counts the number of nodes
  void print();                //prints list in order
  SNode<E>* getHead();         //get pointer to head of list
  void append(SNode<E>* list); //appends new list to original one
  void reverse(SNode<E>* prev);              //reverses list
 private:
  SNode<E>* head;  // head of the list
};

template <typename E>
SLinkedList<E>::SLinkedList()  // constructor
    : head(NULL) {}

template <typename E>
bool SLinkedList<E>::empty() const {  // is list empty?
  return head == NULL;
}

template <typename E>
const E& SLinkedList<E>::front() const {  // return front element
  return head->elem;
}

template <typename E>
SLinkedList<E>::~SLinkedList() {  // destructor
  while (!empty()) removeFront();
}

template <typename E>
void SLinkedList<E>::addFront(const E& e) {  // add to front of list
  SNode<E>* v = new SNode<E>;                // create new node
  v->elem = e;                               // store data
  v->next = head;                            // head now follows v
  head = v;                                  // v is now the head
}

template <typename E>
void SLinkedList<E>::removeFront() {  // remove front item
  if (head == NULL) return;
  SNode<E>* old = head;  // save current head
  head = old->next;      // skip over old head
  delete old;            // delete the old head
}

//New code
template <typename E>
int SLinkedList<E>::numberOfNodes() {
    static int count = 0;                 //set initial count and current node
    static SNode<E>* cur = head;

    if (cur == NULL) 
    {                    //if end of list is reached return the current count
        return count;
    }
    else 
    {                                //if end was not reached increment count and move to next node to check again
        count++;
        cur = cur->next;
        numberOfNodes();
        return count;
    }
}

template <typename E>
void SLinkedList<E>::print() {
    SNode<E>* cur = head;                

    while (cur->next != NULL)          //prints until the current points to null
    {
        cout << cur->elem << "->";
        cur = cur->next;
    }
    cout << cur->elem << endl;         //when it points to null, print the last element and terminate
}

template <typename E>
SNode<E>* SLinkedList<E>::getHead() {
    return head;                       //returns address of head
}

template <typename E>
void SLinkedList<E>::append(SNode<E>* list) {
    SNode<E>* cur = head;
    while (cur->next != NULL)          //searches for last node in list
    {                    
        cur = cur->next;
    }

    cur->next = list;                  //sets last node to point to head of second list
}

template <typename E>
void SLinkedList<E>::reverse(SNode<E>* prev) {

    static SNode<E>* OldHead = head;          //so we know when we reach the last node and point it to NULL 
    static SNode<E>* cur = head->next;
    if (cur->next == NULL) 
    {
        head = cur;                          //set new head
        cur->next = prev;                    //start first reverse
        cur = cur->next;                     //take a reverse step then we move to it and go back to last call
        return;
    }
    else 
    {
        cur = cur->next;                     
        reverse(prev->next);                //recursively call with new prev and cur
        cur->next = prev;                   //point cur to prev node
        cur = cur->next;                    //move to next node and go back to last call
        if (cur == OldHead)                 //checks if we've reached last node and makes it point to null
        {               
            cur->next = NULL;             
        }
        return;
    }
}

int main() {
    SLinkedList<string> List1;
    List1.addFront("four");
    List1.addFront("three");
    List1.addFront("two");
    List1.addFront("one");
    cout << "List 1 has 4 nodes:" << endl;
    List1.print();

    SLinkedList<string> List2;
    List2.addFront("seven");
    List2.addFront("six");
    List2.addFront("five"); 
    cout << "List 2 has 3 nodes:" << endl;
    List2.print();

    List1.append(List2.getHead()); 
    cout << "After appending list 2 to list 1, list 1 has 7 nodes:" << endl;
    List1.print();

    List1.reverse(List1.getHead());
    cout << "After reversing list 1, it looks like this:" << endl;
    List1.print();
}


