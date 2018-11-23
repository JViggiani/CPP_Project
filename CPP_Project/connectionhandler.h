#ifndef ConnectionHandler_h_
#define ConnectionHandler_h_

#include <thread>
#include <deque>
#include <string>
#include <mutex>

class ConnectionHandler {
private:
	std::deque< std::string > queue_;
	std::thread thread_;
	bool quitReceived_;
	std::mutex mutex_;
	std::string userIdentifiedAs_;
private:

	/*
		* Reads and interprets the messages recieved on the queue_. 
		* Can handle messages of type QUIT, HELLO_I_AM and NEW_ORDER
		* QUIT: quitRecieved_ = true and front item in queue removed
		* HELLO_I_AM: Reads userIdentifiedAs_ which is equivilent to the clientId in the client class. Removes first order from queue
		* NEW_ORDER: Reads information and creates a new order based off of the information provided. 
		* This method also handles partially read messages by passing them back into the queue and trying to find the other part later. 
		* The mutex is locked at the beginning of this process in order to prevent concurrency issues. 
	*/
	void processMessages();
public:

	/*
		* Begins the thread which the ConnectionHandler object holds.
		* The thread immediately calls the processMessages method on itself. 
		* quitRecieved is initialised to false.
	*/
	void start();

	/*
		* Takes a buffer of characters, typically an array, and turns it into a a string before adding it to the queue_
	*/
	void queueMessage( const char* buffer, size_t length );

	bool quitReceived() const {
		return quitReceived_;
	}
	
	/*
		* Joins the connectionhandler threads for safe program escape.
	*/
	void join();
};

#endif // ConnectionHandler_h_
