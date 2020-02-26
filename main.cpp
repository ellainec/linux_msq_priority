#include <iostream>

/*--------- showmsg.c: Show message queue details ----*/
#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include "msq.h"
#include "client.h"
#define KEY_ID 2222

int main (int argc , char *argv[])
{

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
    delete Msq;

    // Remove the message queue
//    if (msgctl (msq_id, IPC_RMID, 0) < 0)
//    {
//        perror ("msgctl (remove queue) failed!");
//        exit (3);
//    }
    exit(0);
}


