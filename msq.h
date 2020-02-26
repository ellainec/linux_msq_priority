//
// Created by ellaine on 2020-02-26.
//

#ifndef MSGQ_PRIORITY_MSQ_H
#define MSGQ_PRIORITY_MSQ_H

#define MAXMESSAGEDATA (4096-16) /* don't want sizeof(Mesg) > 4096 */
#define MESGHDRSIZE (sizeof(Mesg) -  MAXMESGDATA)/* length of mesg_len and mesg_type */
#define CLIENT_TO_SERVER_TYPE 1

#include <cstring>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct {
    long mesg_type; /* message type */
    int mesg_len;  /* #bytes in mesg_data */
    char mesg_data [MAXMESSAGEDATA];
} Mesg;

class msq {
public:
    msq(){};
    ~msq() {};
    static int id;
    int send_message(long type,  std::string message);
    std::string read_message(long type);
    bool getmsq(key_t key);
    void deleteMsq();

};


#endif //MSGQ_PRIORITY_MSQ_H
