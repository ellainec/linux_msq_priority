//
// Created by ellaine on 2020-02-26.
//

#include "client.h"

void client::run_client(int priority, std::string filename) {
    pid_t my_pid = getpid();
    std::string initMessage = std::to_string(priority) + std::to_string(my_pid) + ":" + filename;
    if (Msq->send_message(1, initMessage) == -1) {
        perror("msgsend fail");
        return;
    };
    client_thread_info *info = new client_thread_info;
    info->Msq = Msq;
    info->pid = my_pid;
    pthread_t readThread;
    pthread_create(&readThread, NULL, readMessages, (void*)info);
    pthread_join(readThread, NULL);
    std::cout << std::endl;

}

void* client::readMessages(void *info_p) {
    client_thread_info *info = (client_thread_info*)info_p;
    while(true) {
        std::string read = info->Msq->read_message(info->pid);
        if(read.size() < 1) {
            return NULL;
        }
        std::cout << read;
    }
}