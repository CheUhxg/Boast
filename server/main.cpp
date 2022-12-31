#include <iostream>

void server(const char* port) {

}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "Usage: <prog> <port>" << std::endl;
        return 1;
    }
    server(argv[1]);
    return 0;
}
