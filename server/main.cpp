#include <iostream>

void server(const char* ip, const char* port) {

}

int main(int argc, char* argv[]) {
    if(argc != 3) {
        std::cout << "Usage: <prog> <ip> <port>" << std::endl;
        return 1;
    }
    server(argv[1], argv[2]);
    return 0;
}
