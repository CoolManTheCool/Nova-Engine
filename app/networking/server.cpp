#include "server.hpp"

Server::Server(const std::string& ip, int port)
    : socket_(io_context_, boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(ip), port)) {
    std::cout << "Binding to: " << ip << ", on port: " << port << "!" << std::endl;
}

void Server::start() {
    std::cout << "UDP Server started, waiting for messages..." << std::endl;
    do_receive();
    io_context_.run();
}

void Server::do_receive() {
    socket_.async_receive_from(
        boost::asio::buffer(data_, MAX_LENGTH), sender_endpoint_,
        [this](boost::system::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0) {
                std::cout << "Received: " << std::string(data_, bytes_recvd) << std::endl;
            } else {
                do_receive();
            }
        });
}

void Server::do_send(char* _data) {
    socket_.async_send_to(
        boost::asio::buffer(_data, sizeof(_data)), sender_endpoint_,
        [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {
            do_receive(); // Wait for the next message
        });
}
