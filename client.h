//
// Created by ellaine on 2020-02-26.
//

#ifndef MSGQ_PRIORITY_CLIENT_H
#define MSGQ_PRIORITY_CLIENT_H
#include "msq.h"
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
class client {
public:
    client(msq* Msq) : Msq(Msq){};
    void run_client(int priority, std::string filename) {
        pid_t my_pid = getpid();
        std::string initMessage = std::to_string(priority) + std::to_string(my_pid) + ":" + filename;
        std::cout << initMessage << std::endl;
        if (Msq->send_message(1, initMessage) == -1) {
            perror("msgsend fail");
            return;
        };
        while(true) {
            std::string read = Msq->read_message(my_pid);
            if(read.size() < 1) {
                return;
            }
            std::cout << read;
        }
    }
private:
    msq* Msq;
};
#endif //MSGQ_PRIORITY_CLIENT_H
