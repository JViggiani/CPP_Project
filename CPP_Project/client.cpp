#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "order.h"
#include <cstdlib>
using boost::asio::ip::tcp;

std::string clientid;
int orderId = 0;
std::string instruments[]{ "VOD.L","HSBA.L" };
size_t sizes[]{ 1763,2024 };
float benchmarkPrices[]{ 1.23,2.76 }; //TODO change all floats to doubles

void sendNewOrder(tcp::socket& socket) {
	boost::system::error_code ignored_error;
	int instIndex = orderId % 2;
	Order newOrder( 
		{ 
			clientid, orderId, instruments[instIndex], Order::Buy, sizes[instIndex], benchmarkPrices[instIndex] 
		}
	); //TODO make the numbers random - or better make it test values which are recreatable
	newOrder.setClientId(clientid);
	std::cout << "Sending order " << orderId++ << " " << newOrder.toString() << "\n";
	//TASK change the protocol to include the order id
	//TASK change the protocol to FIX - this is a text based protocol! Be aware of architectural differences between Linux and Windows.. text is universal
	boost::asio::write(socket, boost::asio::buffer("NEW_ORDER" + newOrder.serialise()), ignored_error);
}

int main(int argc, char* argv[]) {
	try {
		if (argc != 4) {
			std::cerr << "Usage: client <host> <port> <clientid>\n";
			return 1;
		}

		boost::asio::io_service io_service;	//async I/O

		tcp::resolver resolver(io_service);
		std::string port(argv[2]);
		tcp::resolver::query query(argv[1], port);
		clientid = argv[3];
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		tcp::socket socket(io_service);
		std::cout << "Connecting to server: " << argv[1] << ":" << port << "\n";
		boost::asio::connect(socket, endpoint_iterator);

		{
			std::string message = "HELLO_I_AM " + clientid;
			boost::system::error_code ignored_error; //TASK don't ignore errors - this is meant to be a reference
			std::cout << "Logging in\n";
			boost::asio::write(socket, boost::asio::buffer(message), ignored_error);	//all the write functions require an error code reference.. this is actually writing the buffer data containing thr message to the socket 
		}
		std::cout << "sleep 5\n";
		sleep(5);
		int numOrders = 10 + rand() % 100;
		for (int i = 0; i < numOrders; ++i) { //TASK have the server respond to the client with fill messages
			sendNewOrder(socket);
			sleep(10); //TASK make random between 0.1s and 3s
		}
		std::cout << "Logging out\n";
		{
			std::string message = "QUIT ";
			boost::system::error_code ignored_error;
			boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
		}

		//		for (;;)
		//		{
		//			boost::array<char, 128> buf;
		//			boost::system::error_code error;
		//
		//			size_t len = socket.read_some(boost::asio::buffer(buf), error);
		//
		//			if (error == boost::asio::error::eof)
		//				break; // Connection closed cleanly by peer.
		//			else if (error)
		//				throw boost::system::system_error(error); // Some other error.
		//
		//			std::cout.write(buf.data(), len);
		//		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
