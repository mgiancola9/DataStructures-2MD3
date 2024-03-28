#include <iostream>
#include <random>
using namespace std;

void birthday(int n, double e) 
{
  int c = 0;
  srand(time(0)); // generates random number each run through program (seeding)
  for (int i = 0; i < e; i++) 
  {
    bool birthdays[365] = {0}; // array to keep track of birthdays
    int counter = 0;
    for (int j = 0; j < n; j++) 
    {
      int birthday = rand() % 365 + 1; // randomly generate birthday
      if (birthdays[birthday] == 1) // checks if birthday already exists
      {
        c++; // increment number of experiments with at least 2 people with same birthday
        break;
      } 
      else 
      {
        birthdays[birthday] = 1; // mark the birthday as taken
        counter++;
      }
    }
  }
  double measuredProb = (double)c / (double)e;
  std::cout << n << "," << c << "," << measuredProb << endl;
}

int main() {
  for (int i = 5; i <= 100; i += 5) 
  {
    birthday(i, 100);
  }
  return 0;
}

