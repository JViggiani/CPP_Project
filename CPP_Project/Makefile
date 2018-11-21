ALL: server client
TESTS: orderbook.t

GTESTDIRECTORY=/home/ibutt/googletest-master/googletest
GTESTMAIN=gtest_main.a
CPP11=-std=c++11
COMPILE_FOR_DEBUG=-Og -g
LIBRARIES= \
/usr/lib64/libboost_system-mt.so.1.53.0

CPPFLAGSWITHGTEST=$(CPP11) $(COMPILE_FOR_DEBUG) -I$(GTESTDIRECTORY)/include $(GTESTDIRECTORY)/make/$(GTESTMAIN)

clean:
	-rm -f server client *.o *.gch

connectionhandler.o: connectionhandler.cpp connectionhandler.h
	g++ $(CPP11) $(COMPILE_FOR_DEBUG) -c $^

order.o: order.cpp order.h
	g++ $(CPP11) $(COMPILE_FOR_DEBUG) -c $^

orderbook.o: orderbook.cpp orderbook.h
	g++ $(CPP11) $(COMPILE_FOR_DEBUG) -c $^

server: server.cpp connectionhandler.o order.o orderbook.o
	g++ $(CPP11) $(COMPILE_FOR_DEBUG) -o $@ server.cpp order.o orderbook.o connectionhandler.o $(LIBRARIES)

client: client.cpp order.o
	g++ $(CPP11) $(COMPILE_FOR_DEBUG) -o $@ $^ client.cpp order.o $(LIBRARIES)

orderbook.t: orderbook.t.cpp orderbook.o
	g++ $(CPP11) $(CPPFLAGSWITHGTEST) -o $@ orderbook.t.cpp orderbook.o $(LIBRARIES)
