//
// Created by ellaine on 2020-02-26.
//

#ifndef MSGQ_PRIORITY_CLIENT_H
#define MSGQ_PRIORITY_CLIENT_H
#include "msq.h"
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <stdio.h>

typedef struct {
    msq* Msq;
    pid_t pid;
} client_thread_info;
class client {
public:
    client(msq* Msq) : Msq(Msq){};
    void run_client(int priority, std::string filename);
private:
    msq* Msq;
    static void* readMessages(void* Msq);
};
#endif //MSGQ_PRIORITY_CLIENT_H
