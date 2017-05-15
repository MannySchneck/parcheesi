#pragma once
#include <boost/asio.hpp>


class Net_Connection{
public:
        Net_Connection();
        Net_Connection(boost::asio::io_service io_service_,
                       boost::asio::ip::tcp::socket socket_,
                       boost::asio::ip::tcp::socket otherside); // gotta take it on the otherside, after all

        std::string read_response();

        void shovel_bits(std::string msg);
protected:
        boost::asio::io_service io_service_;
        boost::asio::ip::tcp::socket socket_;
        boost::asio::ip::tcp::endpoint server;
};

class Net_Connection_Client :
        public Net_Connection{
public:
};

class Net_Connection_Server :
        public Net_Connection{
        Net_Connection_Server(boost::asio::io_service io_service_,
                              boost::asio::ip::tcp::socket socket_,
                              boost::asio::ip::tcp::socket client);

        void accept_connection();
};
