//
// Created by ellaine on 2020-02-26.
//

#include "msq.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: msq.cpp
--
-- PROGRAM: Msq
--
-- FUNCTIONS:
-- int send_message(long type,  std::string message)
-- std::string read_message(long type)
-- bool getmsq(key_t key)
-- void deleteMsq()
--
-- DATE: March 2, 2020
--
-- DESIGNER: Ellaine Chan
--
-- PROGRAMMER: Ellaine Chan
--
-- NOTES:
-- The msq class encapsulates all the function calls related to Linux message queues.
----------------------------------------------------------------------------------------------------------------------*/

int msq::id = -1;

/*--------------------------------------------------------------------------------
-- FUNCTION: send_message
--
-- DATE:    March 2, 2020
--
-- DESIGNER: Ellaine Chan
--
-- PROGRAMMER: Ellaine Chan
--
-- PARAMETERS: long type, std::string message
--
-- RETURNS: int
--
-- NOTES:
-- Sends a message with msgsnd to the message queue with the message type equal
-- to the type argument passed in, and the message contents equal to the message
-- argument passed in.
-- Returns the result of the msgsnd call.
-------------------------------------------------------------------------------- */
int msq::send_message(long type,  std::string message) {
    Mesg sendmsg;
    memset(&sendmsg, 0, sizeof(Mesg));
    sendmsg.mesg_type = type;
    sendmsg.mesg_len= message.size();
    strcpy(sendmsg.mesg_data, message.c_str());
    int result, length;
    length = sizeof(Mesg) - sizeof(long) - sizeof(int);
    if ( (result = msgsnd(id, &sendmsg, length, 0)) == -1) {

        return (-1);
    }
    return (result);
}


/*--------------------------------------------------------------------------------
-- FUNCTION: read_message
--
-- DATE:    March 2, 2020
--
-- DESIGNER: Ellaine Chan
--
-- PROGRAMMER: Ellaine Chan
--
-- PARAMETERS: long type
--
-- RETURNS: std::string message read
--
-- NOTES:
-- Calls msgrcv on the message queue with the type equal to the type argument
-- passed in. Returns the message data read, which may be empty if the call fails.
-------------------------------------------------------------------------------- */
std::string msq::read_message(long type) {
    int result, length;
    Mesg rcvmsg;
    length = sizeof(Mesg) - sizeof(long) - sizeof(int);
    if ( (result = msgrcv(id, &rcvmsg, length, type, 0)) == -1) {
        perror("read failed");
    }
    return rcvmsg.mesg_data;
}

/*--------------------------------------------------------------------------------
-- FUNCTION: getmsq
--
-- DATE:    March 2, 2020
--
-- DESIGNER: Ellaine Chan
--
-- PROGRAMMER: Ellaine Chan
--
-- PARAMETERS: key_t key
--
-- RETURNS: bool true if succesful, else false
--
-- NOTES:
-- Calls msgget to create the message queue to be used for both server and client
-------------------------------------------------------------------------------- */
bool msq::getmsq(key_t key) {
    if ((id = msgget (key, IPC_CREAT | 0666)) < 0) {
        perror("msgget failed!");
        return false;
    }
    return true;
}

/*--------------------------------------------------------------------------------
-- FUNCTION: deleteMsq
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
-- Removes the message queue.
-------------------------------------------------------------------------------- */
void msq::deleteMsq() {
    if (msgctl (id, IPC_RMID, 0) < 0)
    {
        perror ("msgctl (remove queue) failed!");
    }
}
