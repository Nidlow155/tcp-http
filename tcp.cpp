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
    return std::chrono::system_clock::now().time_since_epoch() / std::chrono::seconds(1);
}

int main(int argc, char **argv)
{
    srand(time(0));
    map<string, int> initializeServerData();
    string generateRandomLetter();
    unsigned long getTime();

    int rank, size, data;
    unsigned long startTime, endTime;
    int serverData[2] = {};
    int receivedData[2] = {};
    vector<int> receivedBytes(22);
    MPI_Status mystatus;
    MPI_Request request;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);
    srand((unsigned)time(NULL) + rank * size);

    // We assume rank 0 is server and rank n != 0 is client
    if (rank == 0)
    {
        int counter = size - 1;
        while (counter > 0)
        {
            // Acknowledge first packet received
            MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MCW, &mystatus);
            int server_seq = rand() % 100;
            int ack = data + 1;
            serverData[0] = server_seq;
            serverData[1] = ack;
            // Send back server ACK and SEQ from client
            cout << "Step 2: Server: Packet received \"SEQ(" << data << ")\" from Client " << mystatus.MPI_SOURCE << "." << endl;
            cout << "Step 2 Cont: Server: Sending back ACK(" << ack << ") and the server's SEQ(" << server_seq << ") to Client " << mystatus.MPI_SOURCE << "." << endl;
            sleep(1);
            MPI_Send(&serverData, 2, MPI_INT, mystatus.MPI_SOURCE, 0, MCW);

            // Receive ack back from client with client_seq + 1
            MPI_Recv(&receivedData, 2, MPI_INT, mystatus.MPI_SOURCE, MPI_ANY_TAG, MCW, &mystatus);
            cout << "Step 4: Server: Received SEQ(" << receivedData[0] << ") and ACK(" << receivedData[1] << ") from Client " << mystatus.MPI_SOURCE << "." << endl;
            cout << "Server and Client " << mystatus.MPI_SOURCE << ": Connection ESTABLISHED." << endl;
            cout << "---Sending Data from Server to Client " << mystatus.MPI_SOURCE << "---" << endl;
            int dataList[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            MPI_Isend(&dataList, 8, MPI_INT, mystatus.MPI_SOURCE, 0, MCW, &request);
            ack = ack + 8;
            MPI_Recv(&data, 1, MPI_INT, mystatus.MPI_SOURCE, MPI_ANY_TAG, MCW, &mystatus);
            if (data == ack)
            {
                cout << "Server: Client " << mystatus.MPI_SOURCE << ": properly received all data." << endl;
                data = ack;
                // Sending FIN
                cout << "Server: Sending FIN with seq(" << ack << ") to Client " << mystatus.MPI_SOURCE << "." << endl;
                sleep(0.25);
                MPI_Send(&data, 1, MPI_INT, mystatus.MPI_SOURCE, 0, MCW);

                // Sending ACK
                MPI_Recv(&receivedData, 2, MPI_INT, mystatus.MPI_SOURCE, MPI_ANY_TAG, MCW, &mystatus);
                cout << "Server: Received FIN-ACK from Client " << mystatus.MPI_SOURCE << "." << endl;
                cout << "Server: Sending final ACK to close connection to Client " << mystatus.MPI_SOURCE << "." << endl;
                data = receivedData[1] + 1;
                sleep(0.25);
                MPI_Send(&data, 1, MPI_INT, mystatus.MPI_SOURCE, 0, MCW);
            }
            else if (data != ack)
            {
                cout << "Server: Client " << mystatus.MPI_SOURCE << ": did not properly received all data. Try again." << endl;
            }
            counter -= 1;
        }
    }
    else
    {
        //First send packet to client
        int client_seq = rand() % 100;
        cout << "Step 1: Client " << rank << ": sent SEQ(" << client_seq << ") to server." << endl;
        sleep(1);
        MPI_Send(&client_seq, 1, MPI_INT, 0, 0, MCW);

        // Recv serverData (server_seq, ACK) from Server
        MPI_Recv(&receivedData, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MCW, &mystatus);
        cout << "Step 3: Client " << rank << ": received ACK(" << receivedData[1] << ") and SEQ(" << receivedData[0] << ") from Server." << endl;
        // int ack = receivedData[1];

        // Send back ACK to acknowledge the same acknowledgement along with client_seq + 1 to
        // indicate that it receive the packet and is ready to continue
        serverData[0] = receivedData[0] + 1;
        serverData[1] = receivedData[1];
        cout << "Step 3 Cont: Client " << rank << ": Sent SEQ(" << serverData[0] << ") and ACK(" << serverData[1] << ") to server." << endl;
        sleep(1);
        MPI_Send(&serverData, 2, MPI_INT, 0, 0, MCW);

        // Counting how many bytes were received
        // wait 2 seconds, then timeout
        auto initialTime = getTime();
        while (getTime() - initialTime < 2)
        {
            //MPI_Wait(&request, &mystatus);
            MPI_Recv(&data, 8, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MCW, &mystatus);
        }
        auto sizeOfData = sizeof(data) * 2;
        auto ack = sizeOfData + serverData[1];
        cout << "Client " << rank << ": Data received." << endl;
        cout << "Client " << rank << ": Sent ACK(" << ack << ") to server." << endl;
        sleep(1);
        int data = ack;
        MPI_Send(&data, 1, MPI_INT, 0, 0, MCW);

        // MPI_Recv Fin
        cout << "Client " << rank << ": received FIN from server." << endl;
        cout << "Client " << rank << ": returning FIN-ACK to server." << endl;
        sleep(0.25);
        MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MCW, &mystatus);
        serverData[0] = data + 1;
        serverData[1] = rand() % 100;
        MPI_Send(&serverData, 2, MPI_INT, 0, 0, MCW);

        //Final ACK to close connection
        MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MCW, &mystatus);
        cout << "Server and Client " << rank << ": Connection CLOSED." << endl;
    }

    MPI_Finalize();

    return 0;
}
