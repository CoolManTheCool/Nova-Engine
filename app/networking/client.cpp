#include "client.hpp"
#include <boost/asio.hpp>

Client::Client(const std::string& ip, int port)
    : socket_(io_context_, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0)) {
    std::cout << "Sending message to: " << ip << ", on port: " << port << "!" << std::endl;

    // Resolve server endpoint
    boost::asio::ip::udp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(boost::asio::ip::udp::v4(), ip, std::to_string(port));
    server_endpoint_ = *endpoints.begin();

    send_message("Hello from client!");

    io_context_.run();
}

void Client::send_message(const std::string& message) {
    socket_.async_send_to(
        boost::asio::buffer(message), server_endpoint_,
        [this](boost::system::error_code ec, std::size_t /*bytes_sent*/) {
            if (!ec) {
                std::cout << "Message sent, waiting for reply..." << std::endl;
                receive_message();
            } else {
                std::cerr << "Send failed: " << ec.message() << std::endl;
            }
        });
}

void Client::receive_message() {
    socket_.async_receive_from(
        boost::asio::buffer(data_, max_length), server_endpoint_,
        [this](boost::system::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0) {
                std::cout << "Received from server: " << std::string(data_, bytes_recvd) << std::endl;
            } else {
                std::cerr << "Receive failed: " << ec.message() << std::endl;
            }
        });
}
