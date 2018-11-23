#include "connectionhandler.h"
#include <iostream>
#include <sstream>
#include "order.h"
#include "orderbook.h"

void ConnectionHandler::queueMessage( const char* buffer, size_t len ) {
	std::cout << "queueMessage called";
	std::string message( buffer, len );
	std::lock_guard<std::mutex> lock( mutex_ );
	queue_.push_back( message );
	std::cout << "leaving queueMessage\n";
}

void ConnectionHandler::processMessages() {
	size_t howMany = 0;
	int recievedTime = 0;
	OrderBook* orderBook = new OrderBook;
	std::cout << "processMessages called\n" << std::endl;
	while ( !quitReceived_ ) {
		{
			std::lock_guard<std::mutex> lock( mutex_ );
			if ( queue_.empty() ) {
				//std::cout << "the queue is empty" << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			} else {
				std::cout << "got a message, so far we have " << howMany << " orders\n";
				const std::string message = queue_.front();	//does this return the right value? Should this return the item at the back instead?
				if ( "QUIT" == message.substr(0, 4) ) {		//TASK make this FIX compliant
					quitReceived_ = true;
					std::cout << "QUIT was received\n";
					queue_.pop_front();
				} else if ( "HELLO_I_AM" == message.substr(0, 10) ) {
					userIdentifiedAs_ = message.substr(10);
					std::cout << "HELLO_I_AM was received '" <<userIdentifiedAs_ << "'\n";
					queue_.pop_front();
				} else if ( "NEW_ORDER" == message.substr(0, 9) ) {
					std::string remainder = message.substr(9);
					std::cout << "NEW_ORDER was received '" <<remainder << "'\n";
					//std::stringstream ss( remainder, std::ios::binary );
					std::stringstream ss( remainder );
					Order order;
					ss >> order;
					order.setRecievedTime(recievedTime);
					orderBook->addOrder(order);
					std::streampos tellg = ss.tellg();
					std::cout << "tellg says size of message " << tellg << "\n";
					queue_.pop_front();
					//ss.ignore( ss.tellg() );
					if (tellg == -1) {	//-1 is some kind of error
						std::ios::iostate ios = ss.rdstate();
						if ( ss.eof() ) {	//eof is end of file
							std::cout << "we reached end of file\n";
							/*std::string newmessage;
							if(queue_.empty()){
								newmessage = message;
							}else{
								newmessage = message + queue_.front();
								queue_.pop_front();
							}
							queue_.push_front( newmessage );*/
						}
					} else {
						howMany++;
						std::cout << "The order was " << order.toString() << "\n";
						remainder = remainder.substr( ss.tellg() );
						//remainder = remainder.substr( 1 );
						std::cout << "new remainder is '" << remainder << "'\n";
						if ( !remainder.empty() ) queue_.push_front( remainder );
					}
				} else {
					std::cout << "Got a partial message which was '" << message << "'\n";
					queue_.pop_front();
					std::string newmessage;
					if(queue_.empty()){
						newmessage = message;
					}else{
						newmessage = message + queue_.front();
						queue_.pop_front();
					}
					queue_.push_front( newmessage );
				}
			}
		} //TASK some reason for this?
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	std::cout << " leaving processMessages\n";
	delete orderBook;
}

void ConnectionHandler::start( ) {
	quitReceived_ = false;
	thread_ = std::thread( &ConnectionHandler::processMessages, this );
}

void ConnectionHandler::join( ) {
	thread_.join();
}
