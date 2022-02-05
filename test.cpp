#include <string>
#include <vector>
#include <iostream>
#include <fstream>

std::vector<std::string> tokenise(std::string csvLine, char separator)
{
	// string vector tokens ## stores the tokens 
	std::vector<std::string> tokens;
	//int start, end ## used to delineate the position of the tokens 
	signed int start, end; 
	std::string token;
	//start = csvLine.find_first_not_of(separator) 
	start = csvLine.find_first_not_of(separator, 0);
	do {
		//end = next 'separator' after start 
		end = csvLine.find_first_of(separator, start);
		//nothing more to find
		if (start == csvLine.length() || start == end) break;
		//we foudn the separator 
		if (end >= 0) token = csvLine.substr(start, end - start);
		//end is invalid 
		else token = csvLine.substr(start, csvLine.length() - start);
		//save the token
		tokens.push_back(token);
		//move past this token
		start = end + 1;
	//continue loop condition
	} while (end > 0);


	return tokens;
}

int main()
{
	/*
	std::vector<std::string> tokens; 
	std::string s = "thing,thing1,thing2"; 

	tokens = tokenise(s, ',');

	for (std::string& t : tokens)
	{
		std::cout << t << std::endl;
	}*/

	std::ifstream csvFile{"orderbook.csv"};
	std::string line;
	std::vector<std::string> tokens;

	if (csvFile.is_open())
	{
		std::cout << "File open" << std::endl;

		while (std::getline(csvFile, line))
		{
			std::cout << "Read line " << line << std::endl;
			tokens = tokenise(line, ',');
			if (tokens.size() != 5) //bad
			{
				std::cout << "bad line" << std::endl;
				continue;
			}
			// we have 5 tokens 
			try {
				double price = std::stod(tokens[3]);
				double amount = std::stod(tokens[4]);
				std::cout << price << ":" << amount << std::endl;
			}catch (std::exception& e) {
				std::cout << "Bad float! " << tokens[3] << std::endl;
				std::cout << "Bad float!" << tokens[4] << std::endl;
				continue;
			}
			
		}

		csvFile.close();
	}
	else
	{
		std::cout << "NOT OPEN" << std::endl;
	}


	return 0;
}