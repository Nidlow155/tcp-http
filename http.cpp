#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <map>
#include <iterator>

#define MCW MPI_COMM_WORLD

using namespace std;

map<string, int> initializeServerData()
{
  map<string, int> serverData;
  serverData["a"] = 1;
  serverData["b"] = 2;
  serverData["c"] = 3;
  serverData["d"] = 4;
  serverData["e"] = 5;
  serverData["f"] = 6;
  serverData["g"] = 7;
  serverData["h"] = 8;
  serverData["i"] = 9;
  serverData["j"] = 10;
  serverData["k"] = 11;
  serverData["l"] = 12;
  serverData["m"] = 13;
  serverData["n"] = 14;
  serverData["o"] = 15;
  serverData["p"] = 16;
  serverData["q"] = 17;
  serverData["r"] = 18;
  serverData["s"] = 19;
  serverData["t"] = 20;
  serverData["u"] = 21;
  serverData["v"] = 22;
  serverData["w"] = 23;
  serverData["x"] = 24;
  serverData["y"] = 25;
  serverData["z"] = 26;
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
