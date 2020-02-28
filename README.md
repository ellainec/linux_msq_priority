A client/server application that demonstrates the use of System V message queues. Clients will send a message to the server to request for a text file 
and if the server can successfully open the file it will send the contents over the same message queue. A simple priority system is implemented 
by changing message length.

## Design overview:
The server will read the queue for messages with type 1 (client to server) and fork a new process to serve that client.
The message will contain the client's PID, a filename, and a priority. The server will attempt to open the file and read
the contents. The server will send the contents of the file to the client with messages, using the client's PID as the type.
The length of the message will depend on the client's priority. Once it has finished reading the contents of the file, or if the file could
not be opened, a message with the length field equal to 0 will be sent. The client will check for the length field and upon reading 0, 
will exit.

## To run: 
This project was created with CLION. On a terminal, cd into cmake-build-release folder and execute MsgQ_Priority.

##### To run as server: 
```./MsgQ_priority s```

##### To run as client: 
```./MsgQ_priority c <1-4 (priority)> <filename>```

Client example: ```./MsqQ_priority c 1 data.txt```

## To test:
Open up 5 terminals. Prepare to run the the application on all 5 terminals, with the command arguments of one as the server and 4 as clients with priorities 1 through 4 
with the same file that should be at least 2MB in size. Smaller files will work but the priority system will not be demonstrated.
Start the server first, and then the client with priority 4, 3, 2 and then 1 last. As each client is reading its messages it will be printed 
to the screen and the client with priority 1 should be the first to finish even though it was started last.
