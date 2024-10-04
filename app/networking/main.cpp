#include <iostream>
#include <string>

#include "server.hpp"
#include "client.hpp"

int main(int argc, char **argv) {
    std::cout << "Argument Count: " << argc << " Arguments: ";
    for (int i = 0; i < argc; ++i) {
        std::cout << "\n" << argv[i];
    };
    std::cout << std::endl;
    if (argc < 4) {
        std::cout << "Usage: [--server address port], [--client address port]" << std::endl;
        return 1;
    }
    const std::string& address = argv[2];
    const int& port = std::stoi(argv[3]);

    if (std::string(argv[1]) == "--server") {
        Server server(address, port);
        server.start();
    } else if (std::string(argv[1]) == "--client") {
        Client client(address, port);
        
        /*
        char input[1024];
        std::cin >> input;
        while (std::cin.getline(input, sizeof(input))) {
            client.send_message(input);
            client.receive_message();
        }
        */
    } else {
        std::cout << "Invalid command line argument. Use --server or --client." << std::endl;
    }
    
    return 1;
} 