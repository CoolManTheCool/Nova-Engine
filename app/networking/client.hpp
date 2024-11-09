#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <boost/asio.hpp>

class Client {
public:
    Client(const std::string& IP, const int Port);
    void send_message(const std::string& message);
    void receive_message();

private:

    boost::asio::io_context io_context_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint server_endpoint_;
    enum { max_length = 2048 };
    char data_[max_length];
};

#endif
