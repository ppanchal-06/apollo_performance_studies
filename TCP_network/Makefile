# Makefile for Boost.Asio TCP Server and Client

CXX = g++
CXXFLAGS = -Wall -O2 -std=c++11
LDFLAGS = -lboost_system -pthread

TARGETS = tcp_server tcp_client

all: $(TARGETS)

tcp_server: tcp_server.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

tcp_client: tcp_client.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS)

