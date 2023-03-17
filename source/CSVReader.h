#pragma once
#include "OrderBookEntry.h"
#include <vector>
#include <string>
#include <iostream>

class CSVReader
{
	public: 
		CSVReader();

		static std::vector<OrderBookEntry> readCSV(std::string csvFile);
		static std::vector<std::string> tokenise(std::string csvLine, char separator);
		static OrderBookEntry stringsToOBE(std::string price,
											std::string amount,
											std::string timestamp,
											std::string product,
											OrderBookType OrderBookType);
		/*static OrderBookEntry stringstoMin(std::string product,
											  std::string timestamp,
											  OrderBookType OrderBookType);*/

	//private: 
		static OrderBookEntry stringsToOBE(std::vector<std::string> strings);
		//static OrderBookEntry stringstoMin(std::vector<std::string> strings);

};

j
