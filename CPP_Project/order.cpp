#include "order.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

std::istream& operator>>( std::istream& is, Order::Direction& direction ){	
	std::string d;

	is >> d;
	if ( d == "B" ) {
		direction = Order::Direction::Buy;
	} else if ( d == "S" ) {
		direction = Order::Direction::Sell;
	} else {
		throw std::runtime_error("Can't read a direction: it must be B or S");
	}

	return is;
}
std::istream& operator>>( std::istream& is, Order& order ){
	std::string clientId;
	int orderId;
	std::string symbol;
	Order::Direction direction;
	size_t quantity;
	float limitPrice;

	is >> clientId;
	is >> orderId;
	is >> symbol;
	if ( is.eof() ) return is;
	is >> direction;
	is >> quantity;
	is >> limitPrice;

	order = Order(clientId, orderId, symbol, direction, quantity, limitPrice);
	return is;
}

/*
	GETTERS:
*/

const std::string & Order::getClientId(void)
{
	return this->clientId_;
}

int Order::getOrderId(void)
{
	return this->orderId_;
}

int Order::getRecievedTime(void)
{
	return this->recievedTime_;
}

const std::string & Order::getSymbol(void)
{
	return this->symbol_;
}

Order::Direction Order::getDirection(void)
{
	return this->direction_;
}

size_t Order::getQuantity(void)
{
	return this->quantity_;
}

float Order::getLimitPrice(void)
{
	return this->limitPrice_;
}

bool Order::getPartiallyFilled(void)
{
	return this->partiallyFilled_;
}

/*
	SETTERS:
*/

void Order::setClientId(const std::string& clientId)
{
	this->clientId_ = clientId;
}

void Order::setOrderId(int orderId)
{
	this->orderId_ = orderId;
}

void Order::setRecievedTime(int recievedTime)
{
	this->recievedTime_ = recievedTime;
}

void Order::setSymbol(const std::string& symbol)
{
	this->symbol_ = symbol;
}

void Order::setDirection(Direction direction)
{
	this->direction_ = direction;
}

void Order::setQuantity(size_t quantity)
{
	this->quantity_ = quantity;
}

void Order::setLimitPrice(float limitPrice)
{
	this->limitPrice_ = limitPrice;
}

void Order::setPartiallyFilled(bool partiallyFilled)
{
	this->partiallyFilled_ = partiallyFilled;
}

/*
	MEMBER METHODS:
*/

std::string Order::serialise() const {
	std::ostringstream oss;
	oss << clientId_ << " " << orderId_ << " " << symbol_ << " " << static_cast<char>(direction_) << " " << quantity_ << " " << limitPrice_;
	return oss.str();
}

std::string Order::toString() const {
	std::ostringstream oss;
	oss << "{ Client ID '" << clientId_
		<< "' order ID '" << orderId_
		<< "' order symbol '" << symbol_
		<< "' direction '" << static_cast<char>(direction_)
		<< "' quantity '" << quantity_
		<< "' limit price '" << limitPrice_
		<< "' }";
	return oss.str();
}