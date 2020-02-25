#include <iostream>

/*--------- showmsg.c: Show message queue details ----*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "filemanager.h"
#include <errno.h>
#include <cstring>
#include "mesg.h"
#define KEY_ID 2222

/*--- Function prototypes -----------------*/

typedef struct {
    long mtype;
    char mtext[100];
} my_msg;
void mqstat_print (key_t mkey, int mqid, struct msqid_ds *mstat);
int send_message(int msg_qid, int type,  std::string message);
std::string read_message(int msg_qid, long type);
void run_client(int priority, std::string filename);
void run_server();


// move later
int msq_id;

int main (int argc , char *argv[])
{

    struct msqid_ds   msq_status;
    key_t mkey = KEY_ID;
    long msg_type = 0;

    if (argc < 2)
    {
        fprintf (stderr, "Usage: msg_queues <server or client> <priority (1 to 3, 1 being highest)> <filename> \n");
        exit(1);
    }
    if ((msq_id = msgget (mkey, IPC_CREAT | 0666)) < 0)
    {
        perror ("msgget failed!");
        exit(2);
    }
    std::string arg1 = argv[1];
    if (arg1.compare("s") == 0) {
        run_server();
    } else {
        if (argc != 4) {
            fprintf (stderr, "Usage: msg_queues <server or client> <priority (1 to 3, 1 being highest)> <filename> \n");
            exit(1);
        }
        int priority = atoi(argv[2]);
        std::string filename = argv[3];
        std::cout << "you picked client with priority: " << priority << "and filename: " << filename <<  std::endl;

        run_client(priority, filename);
    }

    // Remove the message queue
//    if (msgctl (msq_id, IPC_RMID, 0) < 0)
//    {
//        perror ("msgctl (remove queue) failed!");
//        exit (3);
//    }
    exit(0);
}

void run_client(int priority, std::string filename) {
    std::string initMessage = std::to_string(priority) + ":" + filename;
    if (send_message(msq_id, 1, initMessage) == -1) {
        perror("msgsend fail");
        return;
    };
    std::string read = read_message(msq_id, 2);
    std::cout << "Read from file: " << read << std::endl;


}

void run_server() {
    std::string msgRead = read_message(msq_id, 1);
    //std::cout << "read: " << msgRead << std::endl;
    int index = msgRead.find(':');
    int priority = atoi(msgRead.substr(0, index).c_str());
    std::string filename = msgRead.substr(index + 1, msgRead.size());
    std::cout << "priority: " << priority << ", filename: " << filename << std::endl;

    filemanager fm;
    int errno;
    int filelength = fm.openFile(filename);
    if (filelength < 0) {
        printf("error: %s", strerror(errno));
    }
    char buffer[100];
    fm.read(buffer, filelength);
    if (send_message(msq_id, 2, buffer)) {
        perror("msgsend fail");
    }

    //open file
    //send file
}

/*--------- status info print function ---------*/
void mqstat_print (key_t mkey, int mqid, struct msqid_ds *mstat)
{
    /*-- call the library function ctime ----*/
    //char *ctime();
    time_t rawtime;

    printf ("\nKey %d, msg_qid %d\n\n", mkey, mqid);
    printf ("%d messages on queue\n\n", (int)mstat->msg_qnum);
    time(&rawtime);
    printf ("Last send by proc %d at %s\n",
            mstat->msg_lspid, ctime(&rawtime));
    printf ("Last recv by proc %d at %s\n",
            mstat->msg_lrpid, ctime(&rawtime));
}

int send_message(int msg_qid, int type,  std::string message) {
    my_msg msg;
    msg.mtype = type;
    strcpy(msg.mtext, message.c_str());
    int result, length;
    length = sizeof(my_msg) - sizeof(long);
    if ( (result = msgsnd(msg_qid, &msg, 26, 0)) == -1) {

        return (-1);
    }
    return (result);
}

std::string read_message(int msg_qid, long type) {
    my_msg msg;
    int result, length;
    length = sizeof(my_msg) - sizeof(long);
    if ( (result = msgrcv(msg_qid, &msg, length, type, 0)) == -1) {
        perror("read failed");
    } else {
        std::cout << "read bytes = " << result << std::endl;
    }
    return msg.mtext;
}
