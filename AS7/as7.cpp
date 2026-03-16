#include <iostream>

#include "client.hpp"
#include "server.hpp"

int main(int argc, char** argv) {
    // ./as7 client
    // ./as7 server
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [client/server]" << std::endl;
        return -1;
    }

    std::string_view command = argv[1];
    if(command == "server") {
        server_main();
    } else if(command == "client") {
        // Homework: How do we let the user choose the IP address to connect to?
        client_main("127.0.0.1");
    } else {
        std::cerr << "Usage: " << argv[0] << " [client/server]" << std::endl;
        return -1;
    }
}