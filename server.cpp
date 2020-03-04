#include "server.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: server.cpp
--
-- PROGRAM: Server
--
-- FUNCTIONS:
-- void start()
-- void send_file(int priority, std::string filename, std::string pid)
-- int getBufferSize(int priority)
--
-- DATE: March 2, 2020
--
-- DESIGNER: Ellaine Chan
--
-- PROGRAMMER: Ellaine Chan
--
-- NOTES:
-- Server listens to the message queue for requests from clients, and interfaces with the FileManager class
-- to fulfill the request by sending the requested file.
----------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
-- FUNCTION: start
--
-- DATE:    March 2, 2020
--
-- DESIGNER: Ellaine Chan
--
-- PROGRAMMER: Ellaine Chan
--
-- PARAMETERS: void
--
-- RETURNS: void
--
-- NOTES:
-- Starts the server. Waits for a message with type CLIENT_TO_SERVER_TYPE and
-- once received parses the message for the client's PID, priority and requested
-- filename. A new process is created to serve that client. The server then waits
-- again for another message indefinitely until SIGINT signal is received which
-- will remove the message queue and exit the application.
-------------------------------------------------------------------------------- */
void server::start() {
    while (true) {

        std::string msgRead = Msq->read_message(CLIENT_TO_SERVER_TYPE);
        if(msgRead.size() > 0) {
            //message format: <priority><pid>:<filename>
            int priority = atoi(msgRead.substr(0, 1).c_str());
            //find pid
            int seperater = msgRead.find(':');
            std::string client_pid = msgRead.substr(1, seperater-1);
            std::string filename = msgRead.substr(seperater+1, msgRead.size());

            if (fork() == 0) {
                send_file(priority, filename, client_pid);
                return;
            }
        }
    }
}

/*--------------------------------------------------------------------------------
-- FUNCTION: send_file
--
-- DATE:    March 2, 2020
--
-- DESIGNER: Ellaine Chan
--
-- PROGRAMMER: Ellaine Chan
--
-- PARAMETERS: int priority, std::string filename, std::string pid
--
-- RETURNS: void
--
-- NOTES:
-- Function that runs after the server forks a new process to serve each client.
-- Gets the buffer size based on the client's priority, and attempts to open
-- the file requested. If the file can not be opened, the error message is sent to
-- the client followed by a message with length 0 to signify that the client should
-- exit. If the file can be opened, the file contents will be written to the message
-- queue with the message length based on the buffer size.
-- The message type used is the client's PID.
-- A status message will print out to the terminal prior to the first message sent,
-- and another one after the last message sent.
-------------------------------------------------------------------------------- */
void server::send_file(int priority, std::string filename, std::string pid) {
    int buffersize = getBufferSize(priority);
    char buffer[buffersize];
    memset(buffer, 0, sizeof(buffer));
    long clientpid = atoi(pid.c_str());
    filemanager fm;
    int errno;
    int filelength = fm.openFile(filename);
    if (filelength < 0) {
        sprintf(buffer, "Unable to open file %s: %s", filename.c_str(), strerror(errno));
        std::cout << "Client " << pid << " requested file " << filename << ". Error: " << strerror(errno) << std:: endl;
        if (Msq->send_message(clientpid, buffer)) {
            perror("msgsend fail");
        }
        //send empty buffer to close client
        memset(buffer, 0, sizeof(buffer));
        if (Msq->send_message(clientpid, buffer)) {
            perror("msgsend fail");
        }
        return;
    }

    std::cout << "Sending to client " << pid << " with priority " << priority << ", filename: " << filename << std::endl;

    while(filelength > 0) {
        int length = buffersize - 1;
        if (filelength < (buffersize - 1)) {
            length = filelength;
        }
        fm.read(buffer, length);
        if (Msq->send_message(clientpid, buffer)) {
            perror("msgsend fail");
            break;
        }
        memset(buffer, 0, sizeof(buffer));
        filelength -= length;
    }
    memset(buffer, 0, sizeof(buffer));
    //send last message with size 0 to signal end of transfer
    if (Msq->send_message(clientpid, buffer)) {
        perror("msgsend fail");
    }
    std::cout << "Finished sending to client " << pid << " with priority " << priority <<std::endl;
}

/*--------------------------------------------------------------------------------
-- FUNCTION: getBufferSize
--
-- DATE:    March 2, 2020
--
-- DESIGNER: Ellaine Chan
--
-- PROGRAMMER: Ellaine Chan
--
-- PARAMETERS: int priority
--
-- RETURNS: int
--
-- NOTES:
-- Returns an int to represent the buffer size that should be used when sending
-- a message to the client based on the client's priority
-------------------------------------------------------------------------------- */
int server::getBufferSize(int priority) {
    switch (priority) {
        case 1: return 4000;
        case 2: return 1600;
        case 3: return 400;
        case 4: return 100;
        default: return -1;
    }
}

server::~server() {

}