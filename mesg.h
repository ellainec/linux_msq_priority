//
// Created by ellaine on 2020-02-25.
//

#ifndef MSGQ_PRIORITY_MESG_H
#define MSGQ_PRIORITY_MESG_H

#define MAXMESSAGEDATA (4096-16) /* don't want sizeof(Mesg) > 4096 */
#define MESGHDRSIZE (sizeof(Mesg) -  MAXMESGDATA)/* length of mesg_len and mesg_type */

typedef struct {
    int mesg_len;  /* #bytes in mesg_data */
    long mesg_type; /* message type */
    char mesg_data [MAXMESSAGEDATA];
} Mesg;
#endif //MSGQ_PRIORITY_MESG_H
