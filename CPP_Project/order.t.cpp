#include <gtest/gtest.h>
#include <stdexcept>
#include "../CPP_Project/order.h"

TEST(OrderTest, Getters)
{
	Order o = Order("VOD.L", Order::Direction::Buy, 1500, 120.65);

	EXPECT_EQ(o.getSymbol, "VOD.L");
	EXPECT_EQ(o.getDirection, Order::Direction::Buy);
	EXPECT_EQ(o.getQuantity, 1500);
	EXPECT_EQ(o.getLimitPrice, 120.65);
}

TEST(OrderSetters, Setters)
{
	Order o = Order("VOD.L", Order::Direction::Buy, 1500, 120.65);

	o.setSymbol("HSBA");
	o.setDirection(Order::Direction::Sell);
	o.setQuantity(1200);
	o.setLimitPrice(134.25);

	EXPECT_EQ(o.getSymbol, "HSBA");
	EXPECT_EQ(o.getDirection, Order::Direction::Sell);
	EXPECT_EQ(o.getQuantity, 1200);
	EXPECT_EQ(o.getLimitPrice, 134.25);
}

TEST(OrderSerialise, Serialise)
{
	Order o = Order("VOD.L", Order::Direction::Buy, 1500, 120.65);
	
	EXPECT_EQ(o.serialise(), "VOD.L B 1500 120.65");
}

TEST(OrderToString, ToString)
{
	Order o = Order("VOD.L", Order::Direction::Buy, 1500, 120.65);

	EXPECT_EQ(o.toString(), 
		"{ Order symbol 'VOD.L' direction 'Buy' quantity '1500' limit price '120.65' }"
	);
}