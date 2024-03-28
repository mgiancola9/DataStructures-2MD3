#include <cstdlib>
#include <iostream>
using namespace std;

class Progression {  //  a generic progression
 public:
  Progression(long f = 0)  // constructor
      : first(f), cur(f) {}
  virtual ~Progression(){};      // destructor
  void printProgression(int n);  // print the first n values
 protected:
  virtual long firstValue();  // reset
  virtual long nextValue();   // advance
 protected:
  long first;  // first value
  long cur;    // current value
};

void Progression::printProgression(int n) {  // print n values
  cout << firstValue();                      // print the first
  for (int i = 2; i <= n; i++)               // print 2 through n
    cout << ' ' << nextValue();
  cout << endl;
}

long Progression::firstValue() {  //  reset
  cur = first;
  return cur;
}
long Progression::nextValue() {  // advance
  return ++cur;
}

class AbsProgression : public Progression {  // absolute progression
 public:
  AbsProgression(long b = 2, long a = 200);  // constructor
 protected:
  virtual long nextValue();  // advance
 protected:
  long base;  // base value
  long end; //other value
};

AbsProgression::AbsProgression(long b, long a)  // constructor
    : Progression(a), base(b), end(a) {}

long AbsProgression::nextValue() {  // advance by finding the difference
  long temp = abs(base - end);
  base = end;
  end = temp;
  return end;
}

int main() {
  Progression* prog;
  // test AbsProgression
  cout << "Absolute progression with default increment:\n";
  prog = new AbsProgression();
  prog->printProgression(10);
  cout << "Absolute progression with increment 3:\n";
  prog = new AbsProgression(3, 300);
  prog->printProgression(10);
  return 0;
}
