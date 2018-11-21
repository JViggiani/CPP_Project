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
	std::string symbol;
	Order::Direction direction;
	size_t quantity;
	float limitPrice;

	is >> symbol;
	if ( is.eof() ) return is;
	is >> direction;
	is >> quantity;
	is >> limitPrice;

	order = Order(symbol, direction, quantity, limitPrice);
	return is;
}

Order::Order( const std::string& symbol, Direction direction,
		size_t quantity, float limitPrice ) :
		symbol_( symbol ),
		direction_( direction ),
		quantity_( quantity ),
		limitPrice_( limitPrice ){
}

std::string Order::serialise() const {
	std::ostringstream oss;
	oss << symbol_ << " " << static_cast<char>(direction_) << " " << quantity_ << " " << limitPrice_;
	return oss.str();
}

std::string Order::toString() const {
	std::ostringstream oss;
	oss << "{ Order symbol '" << symbol_
		<< "' direction '" << static_cast<char>(direction_)
		<< "' quantity '" << quantity_
		<< "' limit price '" << limitPrice_
		<< "' }";
	return oss.str();
}