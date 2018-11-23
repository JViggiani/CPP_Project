#ifndef OrderBook_h_
#define OrderBook_h_

#include <thread>
#include <deque>
#include <set>
#include <string>
#include <mutex>
#include "order.h"

class OrderBook {
private:

	/*
		Stores Order objects ordered by their price. TODO: NOT CURRENTLY ORDERED LIKE THIS - find out how to order by order.limitPrice;
	*/
	std::multiset<Order> orderBook_;

	/*
		* Takes a position iterator which points to an Order in the orderBook_ and searches for an appropriate counter-order to trade with.
		* Checks both orders for their direction (buy/sell) as well as whether the buy price > sell price.
		* Returns a nullptr if no appropriate trades can be made.
	*/
	std::multiset<Order>::iterator findTrades(std::multiset<Order>::iterator position);

	/*
		* Determines which order can be wholly filled and which order can be partially filled given that each order is likely of different quantities.
		* In the case where the quantities are equal, both orders are fully filled via the fillOrder function. 
	*/
	void executeTrade(std::multiset<Order>::iterator, std::multiset<Order>::iterator);
	
	/*
	*/
	void fillOrder(const Order&);
	void fillOrder(const Order&, int);

public:
	/*
		* Inserts the order into the orderBook_ multiset. 
		* Automatically searches for any possible trades via the findTrade method.
	*/
    void addOrder(const Order& order);

	long unsigned int getNumberOfOrders(void);
	long unsigned int getNumberOfOrders(Order::Direction);

    // void removeOrder - TASK add order cancel options 

};


std::istream& operator>>( std::istream&, Order& );

#endif // OrderBook_h_

