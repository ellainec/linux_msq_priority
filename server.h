//
// Created by ellaine on 2020-02-26.
//

#ifndef MSGQ_PRIORITY_SERVER_H
#define MSGQ_PRIORITY_SERVER_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "filemanager.h"
#include "msq.h"

class server {
public:
    server(msq* Msq) : Msq(Msq){};
    void start();
private:
    msq* Msq;
    int getBufferSize(int priority);
    void send_file(int priority, std::string filename, std::string pid);
};


#endif //MSGQ_PRIORITY_SERVER_H
