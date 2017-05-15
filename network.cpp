#include "network.h"
#include <sstream>
#include "catch.hpp"
#include <iostream>



Net_Connection::Net_Connection() :
        socket_(io_service_),
        server(boost::asio::ip::address::from_string("127.0.0.1"), 8000) // Those numbers, such magic
{
        boost::system::error_code error;
        socket_.connect(server, error);
        if(error){
                throw boost::system::system_error(error); // oops: This should be smarter
        }
}

// Net_Connection::Net_Connection(boost::asio::io_service io_service_,
//                                boost::asio::ip::tcp::socket socket_,
//                                boost::asio::ip::tcp::socket otherside) :
//         socket_(io_service_),
//         server(otherside){}


std::string make_string(boost::asio::streambuf& streambuf){
        return {boost::asio::buffers_begin(streambuf.data()),
                        boost::asio::buffers_end(streambuf.data())};
}

std::string Net_Connection::read_response(){
        boost::asio::streambuf b;

        boost::asio::read_until(socket_, b, '\n');

        return make_string(b);
}

void Net_Connection::shovel_bits(std::string msg){
        socket_.write_some(boost::asio::buffer(msg));
}

// Net_Connection_Server::Net_Connection_Server(boost::asio::io_service io_service_,
//                                              boost::asio::ip::tcp::socket socket_,
//                                              boost::asio::ip::tcp::socket client) :
//         io_service(io_service_),
// {}


TEST_CASE("send a messsage", "[.]"){

        //std::cout << "press enter once you've started the netcat server" << std::endl;

        char c;
        //std::cin >> c;

        Net_Connection a_portal_to_a_magical_place;
        a_portal_to_a_magical_place.shovel_bits("Hi Mike!\n");
}

// TEST_CASE("receive a message"){

//         Net_Connection_Client a_portal_to_a_magical_place;
//         REQUIRE(a_portal_to_a_magical_place.read_response() == "Hi Mike!\n");
// }
