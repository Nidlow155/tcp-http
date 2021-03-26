#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <map>
#include <iterator>
#include <string>

#define MCW MPI_COMM_WORLD

using namespace std;

map<string, int> initializeServerData()
{
  map<string, int> serverData;
  int startingLetter = 97;
  for (int i = 0; i < 26; i++)
  {
    char asciiChar = startingLetter + i;
    string s;
    s += asciiChar;
    serverData[s] = i + 1;
  }
  return serverData;
}

string generateRandomLetter()
{
  string letters[26] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};

  return letters[rand() % 26];
}

int main(int argc, char **argv)
{
  map<string, int> initializeServerData();
  string generateRandomLetter();

  int rank, size;
  map<string, int> serverData;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank);
  MPI_Comm_size(MCW, &size);

  if (!rank)
  {
    serverData = initializeServerData();
    cout << serverData["a"] << endl;
  }

  // Process 0 is the server
  if (!rank)
  {

  }
  // All other processes request from the server
  else
  {

  }

  MPI_Finalize();

  return 0;
}
