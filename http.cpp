#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <map>
#include <iterator>
#include <string>
#include <ctime>
#include <vector>
#include <deque>
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

  int rank, size, requestType, requestReceived, urlType;
  int server = 0;
  char request;
  int response[2];
  unsigned long startTime, endTime;
  deque<vector<int>> requestList;
  map<char, int> serverData;
  vector<int> req;

  // Each process records how long it takes for a request to be completed : will be averaged later
  vector<int> averageRequestTime;

  // Process 0 records average time to process request : we could do this per request type : will be averaged later
  vector<int> averageProcessingTime;

  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MCW, &rank);
  MPI_Comm_size(MCW, &size);
  int maxRequestCount = size / 2;
  int maxRequestPerProcess = size / 4;
  int requestsPerProcess[size];

  if (!rank)
  {
    cout << "Server Data Initializing..." << endl;
    serverData = initializeServerData();
    cout << "Server Data Initialized." << endl;

    for (int i = 0; i < size; i++) {
      requestsPerProcess[i] = 0;
    }
  }

  // Process 0 is the server
  if (!rank)
  {
    // TODO: implement conditon for when the server should stop
    while (true)
    {
      MPI_Iprobe(MPI_ANY_SOURCE, 0, MCW, &requestReceived, &status);

      while(requestReceived)
      {
        MPI_Iprobe(MPI_ANY_SOURCE, 0, MCW, &requestReceived, &status);
        MPI_Recv(&request, 1, MPI_CHAR, MPI_ANY_SOURCE, 0, MCW, &status);

        req.push_back(status.MPI_SOURCE);
        req.push_back(request);
        requestList.push_back(req);
        requestsPerProcess[status.MPI_SOURCE]++;
        int numOfRequests = requestsPerProcess[status.MPI_SOURCE];

        // 507 - Insufficient Storage
        if (requestList.size() > maxRequestCount)
        {
          response[0] = 507;
          response[1] = 0;
          MPI_Send(response, 2, MPI_INT, status.MPI_SOURCE, 0, MCW);
          requestList.pop_back();
          requestsPerProcess[status.MPI_SOURCE]--;
          break;
        }
        else if (numOfRequests > maxRequestPerProcess)
        {
          response[0] = 429;
          response[1] = 0;
          MPI_Send(response, 2, MPI_INT, status.MPI_SOURCE, 0, MCW);
          requestList.pop_back();
          requestsPerProcess[status.MPI_SOURCE]--;
        }
        else
        {
          // 102 - Request Received
          response[0] = 102;
          response[1] = 0;
          MPI_Send(response, 2, MPI_INT, status.MPI_SOURCE, 0, MCW);
        }
      }

      while (requestList.size() > 0)
      {
        // In a request the 0th element is the source
        // the 1st element is the 'url'
        req = requestList.front();
        requestList.pop_front();
        if (isalpha(req[1]))
        {
          // 200 - OK Request
          response[0] = 200;
          response[1] = serverData[req[1]];
          MPI_Send(response, 2, MPI_INT, req[0], 0, MCW);
        }
        else
        {
          // 404 - Request Not Found
          response[0] = 404;
          response[1] = 0;
          MPI_Send(response, 2, MPI_INT, req[0], 0, MCW);
        }
        requestsPerProcess[req[0]]--;
      }
    }
  }
  // All other processes request from the server
  else
  {
    // TODO: implement condition for when the processes should stop
    while(true)
    // for (int i = 0; i < maxRequestCount; i++)
    {
      startTime = getTime();
      // Increase the number by one for each url type implemented
      urlType = rand() % 2;
      switch(urlType)
      {
        case 0:
          request = generateRandomLetter();
          break;
        case 1:
          request = '-';
          break;
      }

      MPI_Send(&request, 1, MPI_CHAR, server, 0, MCW);
      // TODO: see how long process waits if time > 10 seconds then 408
      MPI_Recv(response, 2, MPI_INT, server, 0, MCW, MPI_STATUS_IGNORE);
      // cout << "Rank " << rank << " Status: " << response[0] << endl;

      if (response[0] == 102) {
        MPI_Recv(response, 2, MPI_INT, server, 0, MCW, MPI_STATUS_IGNORE);
        // TODO: if repsonse is 200 then do something with the data received
      } else if (response[0] == 507) {
        // 507 means insufficient storage with the server
        // Send the request again
        MPI_Send(&request, 1, MPI_CHAR, server, 0, MCW);
      } else if (response[0] == 429) {
        // TODO: keep track of metrics
      }

      endTime = getTime();
      averageRequestTime.push_back(endTime - startTime);
      cout << "Rank " << rank << " Status: " << response[0] << endl;
    }
  }

  // TODO: Average time values and display that along with other interesting data

  MPI_Finalize();

  return 0;
}
