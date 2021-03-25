# TCP Handshake and HTTP Data Transfer

## Introduction

With this project, we would recreate the TCP 3-Way Handshake and then mimic the Hypertext Transfer Protocol (HTTP) for data transfer. We would gain a strong background knowledge in the HTTP and TCP protocols while using message passing to deepen our understanding and practice with open_mpi.

## Description

In this program, process 0 will act as the server and the rest of the processes will act as clients in our TCP and HTTP simulation. The messages we will be passing may not be exactly be like the HTTP but they will mimic closely the process of sending data from one server to many clients with TCP and HTTP. As we reach the end of the project we hope to implement specific messages that the client will receive from the server such as 404 Not Found or 200 OK.

In place of more bulky and arguably less necessary realities of the TCP Handshake and HTTP data transfer will may create simplified versions. For example, we may use a process's rank to indicate IP addresses instead of an actual IP address or we may use regular strings instead of byte strings in HTTP.

## Tests

For process 0 which will simulate the server in our program we will test the priority queue. Once we determine the priority of sending requested data and re-sending requested data, we will test to make sure that the queue of data that needs to be sent is working. First we will have a test where all of the data received won't be damaged and nothing will be resent. Then we will run a test where one of the processes received data is intentionally damaged and see how the server (process 0) handles the sorting of this.

Another test we will run is to verify that the checksum functionality for all of the receiving processes works. This will be implemented between just 2 processes where the server will send good data to a process which evaluate this through the checksum functionality and detect no errors. Then the server will intentionally send data with errors that will be caught by the checksum functionality. A 406 (not acceptable) response will be sent from the server when data with errors is sent to the server.

We will have a test for 404 responses where the server (process 0) will have a domain of valid strings it can accept and if it isn't one of these then it returns a 404 response which the receiving process must handle. This will be implemented by a process intentionally sending an invalid string to process 0.

% Another test we will run is a 429 (too many request) where process 0 will keep track of how many requests it has received from each process. Once the number of requests from a process has exceeded the max limit of requests then the server will remove all of that process's requests from its queue and respond with a 429.

Another test we will run is a 507 (insufficient storage) that will mean that process 0 has a set amount of requests it can handle at a time. When a request comes in when the max number of requests has been reached and before the serve processes that request then a 507 response is immediately sent to that process.

% We will run a test that the server response with a 102 (processing) response. This will be sent to the process once the server is working on their request. This will enable the receiving process to wait for another response from the server which will let them know whether or not they need to resend their data.

We will send a 200 response to the process from the server when the server receives the request.

% We will have a 408 (time out) test which will be sent to the process which never sends any data and has an 'idle connection' with the server. The last process will always be chosen as the one that never sends a request to the server and will time out after 10 seconds. Once the 408 response is received then that process will finalize.

A final test we will run is determine the average wait time for a process to receive a response from the server.

## Anticipated Results

The expected results are going to be much like the 'Too Many Chefs' homework assignment where we will narrate the requests and responses among the server and the processes.

For example:

Process 1 request ('string url') from the server.
The server responded with a 200 (OK).
Process 1 received the 200 (OK) response from the server.
