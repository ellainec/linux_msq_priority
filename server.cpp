//
// Created by ellaine on 2020-02-26.
//

#include "server.h"
//function prototypes
std::string read_message(int msg_qid, long type);
int send_message(int msg_qid, int type,  std::string message);

void server::start() {
    while (true) {

        std::string msgRead = Msq->read_message(CLIENT_TO_SERVER_TYPE);
        if(msgRead.size() > 0) {
            //message format: <priority><pid>:<filename>
            int priority = atoi(msgRead.substr(0, 1).c_str());
            //find pid
            int seperater = msgRead.find(':');
            std::string client_pid = msgRead.substr(1, seperater-1);
            std::string filename = msgRead.substr(seperater+1, msgRead.size());

            if (fork() == 0) {
                send_file(priority, filename, client_pid);
                return;
            }
        }
    }
}

void server::send_file(int priority, std::string filename, std::string pid) {
    int buffersize = getBufferSize(priority);
    char buffer[buffersize];
    memset(buffer, 0, sizeof(buffer));
    long clientpid = atoi(pid.c_str());
    filemanager fm;
    int errno;
    int filelength = fm.openFile(filename);
    if (filelength < 0) {
        sprintf(buffer, "Unable to open file %s: %s", filename.c_str(), strerror(errno));
        std::cout << "Client " << pid << " requested file " << filename << ". Error: " << strerror(errno) << std:: endl;
        if (Msq->send_message(clientpid, buffer)) {
            perror("msgsend fail");
        }
        //send empty buffer to close client
        memset(buffer, 0, sizeof(buffer));
        if (Msq->send_message(clientpid, buffer)) {
            perror("msgsend fail");
        }
        return;
    }

    std::cout << "Sending to client " << pid << " with priority " << priority << ", filename: " << filename << std::endl;

    while(filelength > 0) {
        int length = buffersize - 1;
        if (filelength < (buffersize - 1)) {
            length = filelength;
        }
        fm.read(buffer, length);
        if (Msq->send_message(clientpid, buffer)) {
            perror("msgsend fail");
            break;
        }
        memset(buffer, 0, sizeof(buffer));
        filelength -= length;
    }
    memset(buffer, 0, sizeof(buffer));
    //send last message with size 0 to signal end of transfer
    if (Msq->send_message(clientpid, buffer)) {
        perror("msgsend fail");
    }
    std::cout << "Finished sending to client " << pid << " with priority " << priority <<std::endl;
}

int server::getBufferSize(int priority) {
    switch (priority) {
        case 1: return 4000;
        case 2: return 1600;
        case 3: return 400;
        case 4: return 100;
        default: return -1;
    }
}

server::~server() {

}