#include "client.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: client.cpp
--
-- PROGRAM: Client
--
-- FUNCTIONS:
-- void run_client(int priority, std::string filename)
-- void* readMessages(void *info_p)
--
--
-- DATE: March 2, 2020
--
-- DESIGNER: Ellaine Chan
--
-- PROGRAMMER: Ellaine Chan
--
-- NOTES:
-- Client writes to the message queue to request a file from the server. A new thread is created
-- to continuously read from the message queue. When a message of length 0 receives, the client
-- exits.
----------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------
-- FUNCTION: run_client
--
-- DATE:    March 2, 2020
--
-- DESIGNER: Ellaine Chan
--
-- PROGRAMMER: Ellaine Chan
--
-- PARAMETERS: int priority, std::string filename
--
-- RETURNS: void
--
-- NOTES:
-- Starts the client. Makes the request to the server by sending a message with
-- the client's priority, the filename it wants, and the client's PID. A new thread
-- is created to read the server's response.
-------------------------------------------------------------------------------- */
void client::run_client(int priority, std::string filename) {
    pid_t my_pid = getpid();
    std::string initMessage = std::to_string(priority) + std::to_string(my_pid) + ":" + filename;
    if (Msq->send_message(1, initMessage) == -1) {
        perror("msgsend fail");
        return;
    };
    client_thread_info *info = new client_thread_info;
    info->Msq = Msq;
    info->pid = my_pid;
    pthread_t readThread;
    pthread_create(&readThread, NULL, readMessages, (void*)info);
    pthread_join(readThread, NULL);
    std::cout << std::endl;

}

/*--------------------------------------------------------------------------------
-- FUNCTION: readMessages
--
-- DATE:    March 2, 2020
--
-- DESIGNER: Ellaine Chan
--
-- PROGRAMMER: Ellaine Chan
--
-- PARAMETERS: void *info_p
--
-- RETURNS: void
--
-- NOTES:
-- Read thread function to loop reading messages from the message queue. It
-- calls the Msq read method with it's PID, since it only wants messages with type
-- equal to it's PID. If a message with length 0 is received, this signifies that
-- the server has finished sending to the client, so the application exits.
-------------------------------------------------------------------------------- */
void* client::readMessages(void *info_p) {
    client_thread_info *info = (client_thread_info*)info_p;
    while(true) {
        std::string read = info->Msq->read_message(info->pid);
        if(read.size() < 1) {
            return NULL;
        }
        std::cout << read;
    }
}