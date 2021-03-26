#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <map>
#include <iterator>
#include <string>
#include <ctime>

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
  int asciiVal = 97 + (rand() % (123 - 97 +1));
  char asciiChar = asciiVal;
  string s;
  s += asciiChar;
  return s;
}

int main(int argc, char **argv)
{
  srand(time(0));
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
    cout << serverData[generateRandomLetter()] << endl;
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
