/*--------- showmsg.c: Show message queue details ----*/
#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include "msq.h"
#include "client.h"
#include <signal.h>

#define KEY_ID 2222


/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: main.cpp
--
-- PROGRAM: main
--
-- FUNCTIONS:
-- in main (int argc, char *argv[])
-- void catch_int (int signo)
--
-- DATE: March 2, 2020
--
-- DESIGNER: Ellaine Chan
--
-- PROGRAMMER: Ellaine Chan
--
-- NOTES:
-- Entry point of the program. Depending on user arguments a server or client will be started.
----------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------
-- FUNCTION: catch_int
--
-- DATE:    March 2, 2020
--
-- DESIGNER: Ellaine Chan
--
-- PROGRAMMER: Ellaine Chan
--
-- PARAMETERS: signo
--
-- RETURNS: void
--
-- NOTES:
-- Handler for a SIGINT signal to remove the created
-- message queue before exiting the program.
-------------------------------------------------------------------------------- */
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



/*--------------------------------------------------------------------------------
-- FUNCTION: main
--
-- DATE:    March 2, 2020
--
-- DESIGNER: Ellaine Chan
--
-- PROGRAMMER: Ellaine Chan
--
-- PARAMETERS: int argc, char *argv[]
--
-- RETURNS: int
--
-- NOTES:
-- Entry point of program. Sets up the signal handler for SIGINT and reads
-- the command line arguments. If argv[1] is "s", a server application is started.
-- Otherwise, a client application is assumed and priority and filename arguments
-- are read.
-------------------------------------------------------------------------------- */
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
    exit(0);
}


