#ifndef BOAST_BOAST_MSG_H
#define BOAST_BOAST_MSG_H

enum MessageType {
    Register,
    Question,
    Send
};

struct BoastMessage {
    MessageType msg_type = Register;
};

#endif //BOAST_BOAST_MSG_H
