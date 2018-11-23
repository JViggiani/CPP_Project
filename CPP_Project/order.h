#ifndef Order_h_
#define Order_h_


// WHAT'S WRONG HERE???
#include <thread>
#include <deque>
#include <string>
#include <mutex>

class Order {
    friend std::istream& operator>>( std::istream&, Order& );
public:
    enum Direction { Buy = 'B', Sell = 'S' };
private:
	std::string clientId_;
	int orderId_;
	int recievedTime_;
    std::string symbol_;
    Direction direction_;
    size_t quantity_;
    float limitPrice_;
	bool partiallyFilled_;

public:

    /*
       *  @brief  
	   *  Order parameter constructor
       *
       *  Sets the symbol, direction, quantity and limitprice parameters of an instance of Order
	   *  
	   *  @params
	   *  symbol is a string representing the commodity being traded, eg "VOD.L"
	   *  direction is an enum value 'B' or 'S' representing buy or sell orders. 
	   *  quantity represents the number of shares being ordered
	   *  limitPrice is the price set by the client which is the minimum sell or maximum buy price which they will accept
	*/
	Order(std::string clientId, int orderId, const std::string& symbol, Direction direction,
		size_t quantity, float limitPrice) :
		clientId_(clientId),
		orderId_(orderId),
		symbol_(symbol),
		direction_(direction),
		quantity_(quantity),
		limitPrice_(limitPrice),
		partiallyFilled_(false){
	}

	Order() = default;

	const std::string& getClientId(void);
	int getOrderId(void);
	int getRecievedTime(void);
	const std::string& getSymbol(void);
	Direction getDirection(void);
	size_t getQuantity(void);
	float getLimitPrice(void);
	bool getPartiallyFilled(void);

	void setClientId(const std::string& clientId);
	void setOrderId(int orderId);
	void setRecievedTime(int recievedTime);
	void setSymbol(const std::string& symbol);
	void setDirection(Direction direction);
	void setQuantity(size_t quantity);
	void setLimitPrice(float limitPrice);
	void setPartiallyFilled(bool partiallyFilled);

    /*
		* @brief 
		* Prepares the order information to be transmitted in a string format over the network
		* 
		* Returns a string in the format "VOD.L B 1763 1.23" / "symbol_ direction_ qualtity_ limitPrice_"
			* ie returns the order information delimited by spaces
		* Information is sent over the network via string because of differences in big-endian and little-endian between operating systems.
	*/
	std::string serialise() const;

	/*
		* @brief
		* Prepares the order information to be presented on the console
		* Returns a string containing the symbol_ direction_ qualtity_ limitPrice_ quantities as well as a lot of junk.
		* Only intended to be used to print information to the console.
	*/
    std::string toString() const;

	/*
		* Overloading the less than operator so that Order a < Order b returns true if a.limitPrice_ < b.limitPrice_
		* In the case where the prices are equal, Order a < Order b returns true if a.recievedTime_ < b.recievedTime_
	*/
	bool operator<(Order &order) 
	{ 
		return (this->limitPrice_ = order.limitPrice_ ? this->recievedTime_ < order.recievedTime_ : this->limitPrice_ < order.limitPrice_);
	}
};

/*
	* Overloading the input operator.
	* Takes an order and places the order's symbol, direction, quantity and limitPrice on a given istream in that order.
	* direction is placed on the istream as either 'B' or 'S'. 
*/
std::istream& operator>>( std::istream&, Order& );

#endif // Order_h_
