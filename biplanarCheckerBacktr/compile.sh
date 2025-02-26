#!/bin/sh
g++ -o cpp/program cpp/main.cpp cpp/biplanarTester.cpp -std=c++17 -lboost_graph
./cpp/program
