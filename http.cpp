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

map<char, int> initializeServerData()
{
  map<char, int> serverData;
  int startingLetter = 97;
  for (int i = 0; i < 26; i++)
  {
    char asciiChar = startingLetter + i;
    serverData[asciiChar] = i + 1;
  }
  return serverData;
}

char generateRandomLetter()
{
  int asciiVal = 97 + (rand() % (123 - 97 +1));
  char asciiChar = asciiVal;
  return asciiChar;
}

int main(int argc, char **argv)
{
  srand(time(0));
  map<char, int> initializeServerData();
  char generateRandomLetter();
  unsigned long getTime();
  void request_102();
  void request_200();
  void request_404();
  void request_406();
  void request_408();
  void request_429();
  void request_507();

  int rank, size, requestType, requestReceived;
  int server = 0;
  char request;
  int response[2];
  unsigned long startTime, endTime;
  map<char, int> serverData;
  // Each process records how long it takes for a request to be completed : will be averaged later
  vector<int> averageRequestTime;
  // Process 0 records average time to process request : we could do this per request type : will be averaged later
  vector<int> averageProcessingTime;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank);
  MPI_Comm_size(MCW, &size);

  if (!rank)
  {
    cout << "Server Data Initializing..." << endl;
    serverData = initializeServerData();
    cout << "Server Data Initialized." << endl;
  }

  // Process 0 is the server
  if (!rank)
  {
    MPI_Recv(&request, 1, MPI_CHAR, MPI_ANY_SOURCE, 0, MCW, &status);

    response[0] = 102;
    response[1] = 0;
    MPI_Send(response, 2, MPI_INT, status.MPI_SOURCE, 0, MCW);

    if (isalpha(request)) {
      response[0] = 200;
      response[1] = serverData[request];
      MPI_Send(response, 2, MPI_INT, status.MPI_SOURCE, 0, MCW);
    } else {
      response[0] = 404;
      response[1] = 0;
      MPI_Send(response, 2, MPI_INT, status.MPI_SOURCE, 0, MCW);
    }

  }
  // All other processes request from the server
  else
  {
    startTime = getTime();
    request = generateRandomLetter();
    // request = '-';
    MPI_Send(&request, 1, MPI_CHAR, server, 0, MCW);
    MPI_Recv(response, 2, MPI_INT, server, 0, MCW, MPI_STATUS_IGNORE);
    cout << "Rank " << rank << " Status: " << response[0] << endl;

    if (response[0] == 102) {
      MPI_Recv(response, 2, MPI_INT, server, 0, MCW, MPI_STATUS_IGNORE);
    }

    endTime = getTime();
    averageRequestTime.push_back(endTime - startTime);
    cout << "Rank " << rank << " Status: " << response[0] << endl;
  }

  // TODO: Average time values and display that along with other interesting data

  MPI_Finalize();

  return 0;
}
