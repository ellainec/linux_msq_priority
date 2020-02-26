#include <iostream>

/*--------- showmsg.c: Show message queue details ----*/
#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include "msq.h"
#include "client.h"
#include <signal.h>

#define KEY_ID 2222

void catch_int(int signo)
{
    if (msq::id != -1) {
        if (msgctl (msq::id, IPC_RMID, 0) < 0)
        {
            perror ("msgctl (remove queue) failed!");
        }
    }
    exit(0);
}

int main (int argc , char *argv[])
{
    signal(SIGINT, catch_int);
    if (argc < 2)
    {
        fprintf (stderr, "Usage: msg_queues <server or client> <priority (1 to 4, 1 being highest)> <filename> \n");
        exit(1);
    }

    msq *Msq = new msq();

    if (Msq->getmsq(KEY_ID) == false) {
        delete Msq;
        exit(2);
    };

    std::string arg1 = argv[1];

    if (arg1.compare("s") == 0) {
        server *s = new server(Msq);
        s->start();
        delete s;
    } else {
        if (argc != 4) {
            fprintf (stderr, "Usage: msg_queues <server or client> <priority (1 to 3, 1 being highest)> <filename> \n");
            exit(1);
        }
        int priority = atoi(argv[2]);
        std::string filename = argv[3];
        client *c = new client(Msq);
        c->run_client(priority, filename);
        delete c;
    }
    //Msq->deleteMsq();
    //delete Msq;
    exit(0);
}


