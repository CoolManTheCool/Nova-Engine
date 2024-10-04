#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <boost/asio.hpp>

class Server {
public:
    Server(const std::string& IP, const int Port);
    void start();
    void do_receive();
    void do_send(char* _data);

private:

    boost::asio::io_context io_context_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint sender_endpoint_;
    enum { MAX_LENGTH = 2048 };
    char data_[MAX_LENGTH];
};

#endif
