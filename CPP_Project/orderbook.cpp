#include "orderbook.h"
#include <iterator>
#include <iostream>

long unsigned int OrderBook::getNumberOfOrders(void)
{
	return orderBook_.size();
}

long unsigned int OrderBook::getNumberOfOrders(Order::Direction direction)
{
	int numOfOrders = 0;
	for (std::multiset<Order>::iterator position = orderBook_.begin(); position != orderBook_.end(); position++)
	{
		if (direction == (*position).getDirection())
		{
			++numOfOrders;
		}
	}
	return numOfOrders;
}

void OrderBook::addOrder(const Order& order)
{
	std::multiset<Order>::iterator position = orderBook_.insert(order);
	std::multiset<Order>::iterator counterOrderPosition = this->findTrades(position);

	if (counterOrderPosition != orderBook_.end())
	{
		position->getDirection() == Order::Direction::Buy ? executeTrade(position, counterOrderPosition) : executeTrade(counterOrderPosition, position);
	}
}

std::multiset<Order>::iterator OrderBook::findTrades(std::multiset<Order>::iterator position)
{
	std::multiset<Order>::iterator comparatorPosition = position;
	for (comparatorPosition;
		position->getDirection() == Order::Direction::Buy ? comparatorPosition != this->orderBook_.begin : comparatorPosition != this->orderBook_.end;
		position->getDirection() == Order::Direction::Buy ? --comparatorPosition : ++comparatorPosition)
	{
		if (position->getDirection() != comparatorPosition->getDirection()
			&& position->getDirection() == Order::Direction::Buy ? (position->getLimitPrice() >= comparatorPosition->getLimitPrice()) : (position->getLimitPrice() <= comparatorPosition->getLimitPrice()))
		{
			return comparatorPosition;
		}
	}

	return this->orderBook_.end();
}

void OrderBook::executeTrade(std::multiset<Order>::iterator buyOrderPosition, std::multiset<Order>::iterator sellOrderPosition)
{
	if (buyOrderPosition->getQuantity() > sellOrderPosition->getQuantity())
	{
		fillOrder(*sellOrderPosition);
		fillOrder(*buyOrderPosition, *sellOrderPosition->getQuantity);
	}
	else if (buyOrderPosition->getQuantity() < sellOrderPosition->getQuantity())
	{
		fillOrder(*buyOrderPosition);
		fillOrder(*sellOrderPosition, *buyOrderPosition->getQuantity);
	}
	else if (buyOrderPosition->getQuantity() == sellOrderPosition->getQuantity())
	{
		fillOrder(*buyOrderPosition);
		fillOrder(*sellOrderPosition);
	}
}

void OrderBook::fillOrder(const Order& order)
{
	// orderBook_.erase(order);	- is this just equivilent to the below? Needs testing
	
	auto itr = orderBook_.find(order);
	if (itr != orderBook_.end()) {
			orderBook_.erase(itr);
	}
	order.getPartiallyFilled() == true ? 
		std::cout << "ORDER PARTIALLY FILLED: " : 
		std::cout << "ORDER FULLY FILLED: \n";
	
	std::cout << order.toString
		<< "\n----------";
	delete &order;
}

void OrderBook::fillOrder(const Order& order, int fillQuantity)
{
	Order* partiallyFilledOrder = new Order(order.getClientId(), order.getOrderId(), order.getSymbol(), order.getDirection(), order.getQuantity(), order.getLimitPrice());
	partiallyFilledOrder->setRecievedTime(order.getRecievedTime());
	partiallyFilledOrder->setPartiallyFilled(true);
	fillOrder(order);
	this->addOrder(*partiallyFilledOrder);
}