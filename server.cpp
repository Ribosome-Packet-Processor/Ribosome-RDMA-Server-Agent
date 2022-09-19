#include "server/Server.h"

volatile sig_atomic_t flag;

void handler(int) {
    flag = 1;
}

int main(int argc, char **argv) {
    if (argc < 7) {
        std::cout << "Usage: server <device> <idx> <numa> <qps> <min-timer> <max-timer>" << std::endl;

        return 1;
    }

    signal(SIGINT, &handler);

    Server *server = nullptr;
    try {
        server = new Server(std::string(argv[1]), std::stoi(argv[2]),
                            std::stoi(argv[3]), std::stoi(argv[4]),
                            std::stoi(argv[5]), std::stoi(argv[6]));
        if (server->init() == 0) {
            for (;;)
                if (flag) throw (int) flag;
        }
    } catch (int x) {
        delete (server);
    }

    return 0;
}

