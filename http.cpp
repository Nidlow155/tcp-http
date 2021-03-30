#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <map>
#include <iterator>
#include <string>
#include <ctime>
#include <vector>
#include <chrono>

#define MCW MPI_COMM_WORLD

using namespace std;

unsigned long getTime()
{
  return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
}

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

// TODO: implement method
void request_102()
{

}

// TODO: implement method
void request_200()
{

}

// TODO: implement method
void request_404()
{

}

// TODO: implement method
void request_406()
{

}

// TODO: implement method
void request_408()
{

}

// TODO: implement method
void request_429()
{

}

// TODO: impelment method
void request_507()
{

}

int main(int argc, char **argv)
{
  srand(time(0));
  map<string, int> initializeServerData();
  string generateRandomLetter();
  unsigned long getTime();
  void request_102();
  void request_200();
  void request_404();
  void request_406();
  void request_408();
  void request_429();
  void request_507();

  int rank, size, request;
  unsigned long startTime, endTime;
  map<string, int> serverData;
  // Each process records how long it takes for a request to be completed : will be averaged later
  vector<int> averageRequestTime;
  // Process 0 records average time to process request : we could do this per request type : will be averaged later
  vector<int> averageProcessingTime;
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
    // TODO: implement methods above
    request = rand() % 7;
    switch(request)
    {
      case 0:
        startTime = getTime();
        request_102();
        endTime = getTime();
        averageRequestTime.push_back(endTime - startTime);
        break;
      case 1:
        startTime = getTime();
        request_200();
        endTime = getTime();
        averageRequestTime(endTime - startTime);
        break;
      case 2:
        startTime = getTime();
        request_404();
        endTime = getTime();
        averageRequestTime(endTime - startTime);
        break;
      case 3:
        startTime = getTime();
        request_406();
        endTime = getTime();
        averageRequestTime(endTime - startTime);
        break;
      case 4:
        startTime = getTime();
        request_408();
        endTime = getTime();
        averageRequestTime(endTime - startTime);
        break;
      case 5:
        startTime = getTime();
        request_429();
        endTime = getTime();
        averageRequestTime(endTime - startTime);
        break;
      case 6:
        startTime = getTime();
        request_507();
        endTime = getTime();
        averageRequestTime(endTime - startTime);
        break;
    }

  }

  // TODO: Average time values and display that along with other interesting data

  MPI_Finalize();

  return 0;
}
