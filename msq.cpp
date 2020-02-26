//
// Created by ellaine on 2020-02-26.
//

#include "msq.h"
int msq::id = -1;

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

std::string msq::read_message(long type) {
    int result, length;
    Mesg rcvmsg;
    length = sizeof(Mesg) - sizeof(long) - sizeof(int);
    if ( (result = msgrcv(id, &rcvmsg, length, type, 0)) == -1) {
        perror("read failed");
    }
    return rcvmsg.mesg_data;
}

bool msq::getmsq(key_t key) {
    if ((id = msgget (key, IPC_CREAT | 0666)) < 0) {
        perror("msgget failed!");
        return false;
    }
    return true;
}

void msq::deleteMsq() {
    if (msgctl (id, IPC_RMID, 0) < 0)
    {
        perror ("msgctl (remove queue) failed!");
    }
}
