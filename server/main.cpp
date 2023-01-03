#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <arpa/inet.h>
#include <vector>
#include <string>
#include <unistd.h>
#include <mutex>
#include "../boast_msg.h"

struct BoastNode {
    int fd = -1;
    std::string ip;
};

struct QuestionLock {
    bool hasQuestion = false;
    BoastNode* node = nullptr;
    std::mutex mu;
}question_lock;

std::vector<BoastNode> clients;

void get_msg(int cli_fd, BoastMessage& msg, ssize_t& res) {
    ssize_t msg_len = 0;

    res = recv(cli_fd, &msg_len, sizeof(ssize_t), 0);
    if(res == -1) {
        return;
    }

    res = recv(cli_fd, &msg, msg_len, 0);
    if(res == -1) {
        return;
    }
}

void register_client(int cli_fd, const std::string& ip) {
    bool isExist = false;

    for(const BoastNode& node : clients) {
        if(node.ip == ip)isExist = true;
    }
    if(!isExist)clients.push_back({cli_fd, ip});
}

void question_client(const std::string& ip) {
    for(int i = 0; i < clients.size(); ++i) {
        if(clients[i].ip == ip) {
            question_lock.mu.lock();
            question_lock.hasQuestion = true;
            question_lock.node = &clients[i];
            question_lock.mu.unlock();
        }
    }
}

void handler(int cli_fd, const struct sockaddr_in cli_addr) {
    ssize_t res = -1;
    BoastMessage msg;
    std::string ip(inet_ntoa(cli_addr.sin_addr));

    get_msg(cli_fd, msg, res);
    if(res == -1) {
        close(cli_fd);
        return;
    }

    switch (msg.msg_type) {
        case Register:
            register_client(cli_fd, ip);
            break;
        case Question:
            question_client(ip);
            break;
        case Send:
            break;
    }
}

void game() {
    char isStart = 'n';
    std::thread t_cnt_num([](){
        size_t size = -1;

        while(clients.size() != size) {
            printf("\n[*] There is/are %lu client(s). Shell we start the game?(y/n)",
                   clients.size());
            size = clients.size();
        }
    });

    while(isStart == 'n') {
        scanf("%c", &isStart);
    }
}

[[noreturn]] void server(const int port) {
    int sock_fd = -1, res = -1, cli_fd = -1;
    struct sockaddr_in srv_addr = {0};
    struct sockaddr_in cli_addr = {0};
    socklen_t len;
    std::vector<std::thread> thread_pool;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd == -1) {
        perror("socket");
        exit(1);
    }

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = INADDR_ANY;
    srv_addr.sin_port = htons(port);

    res = bind(sock_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    if(res == -1) {
        perror("bind");
        exit(1);
    }

    res = listen(sock_fd, 0);
    if(res == -1) {
        perror("listen");
        exit(1);
    }

    while(true) {
        len = sizeof(cli_addr);
        cli_fd = accept(sock_fd, (struct sockaddr*)&cli_addr, &len);
        if(cli_fd == -1) {
            perror("accept");
            continue;
        }

        thread_pool.emplace_back(handler, cli_fd, cli_addr);
    }
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "Usage: <prog> <port>" << std::endl;
        return 1;
    }
    server(atoi(argv[1]));
}
