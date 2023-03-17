#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "OrderBookEntry.h"
#include <vector>
#include "OrderBook.h"

class advisorbot
{
	public: 
		advisorbot(); 
		void init();


	private:
		void printMenu();
		void printHelp(); //--C1, C2 
		void printCmd(std::vector<std::string> tokens); //C2
		void printMarketProd(); //--C3 
		void printMin(std::vector<std::string> tokens); //--C4
		void printMax(std::vector<std::string> tokens); //--C5 
		void printAvg(std::vector<std::string> tokens); //--C6 
		void printPredict(std::vector<std::string> tokens); //--C7
		void currentTimeStep(); //--C8 time
		void nextTimeFrame(); //--C9
		void printTrend(std::vector<std::string> tokens);
		std::vector<std::string> getUserOption();

		std::string currentTime;

		OrderBook orderBook{ "orderbook_new.csv" };
		
};

