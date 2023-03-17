#include "MerkelMain.h"
#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <string>
#include <regex>
#include "CSVReader.h"
#include "OrderBook.h"
#include "advisorbot.h"

MerkelMain::MerkelMain()
{

}

void MerkelMain::init()
{
    //loadOrderBook();
    std::string input;
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 10);

    while (true)
    {
        std::cout << "Hello! I am advisorbot." << std::endl;
        std::cout << "Enter your command! Type 'help' for help." << std::endl;
        std::cout << "=====================" << std::endl;
        input = getUserOption();
        //processUserOption(input);

        /*if (input == 'end') {
            std::cout << "Bye bye!" << std::endl;
            break;
        };*/
    }

}

void MerkelMain::enterAsk() //3
{
    std::cout << "Make an ask. Enter the amount you'd like to offer: product,price,amount. eg ETH/BTC,200,0.5" << std::endl;
    std::string input; 
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "Bad input!" << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::ask
            );
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "wallet lookin kl33n baybee" << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Insufficient funds." << std::endl;
            }
            
        }
        catch (const std::exception& e)
        {
            std::cout << "MerkelMain::enterAsk Bad input!" << std::endl;
        }
    }

    std::cout << "You typed: " << input << std::endl;
}

void MerkelMain::enterBid() //4
{
    std::cout << "Make a bid. Enter the amount you'd like to bid: product, price,amount eg: ETH/BTC,200,0.5" << std::endl;
    std::string input; 
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid bad input!" << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid
            );
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet lookin ok" << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Insufficient funds." << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "MerkelMain::enterBid bad input" << std::endl;
        }
    }
}

void MerkelMain::printWallet() //5
{
    std::cout << "Your wallet fat king" << std::endl;
    std::cout << wallet.toString() << std::endl;
}

std::string MerkelMain::getUserOption()
{
    std::string userInput;
    std::getline(std::cin, userInput);

    std::vector<std::string> tokens = CSVReader::tokenise(userInput, ' ');
    std::cout << userInput << std::endl;
   
    std::cout << "token 0 is" << tokens[0] << std::endl;

    for (std::string const& userInput : tokens) {
        std::cout << userInput << std::endl;        
    }

    if (tokens.size() > 1) {
        if (tokens[1] == "prod")
            std::cout << "List available products" << std::endl;
        else if (tokens[1] == "min")
            std::cout << "Find minimum bid or ask for product in current time step" << std::endl;
        else if (tokens[1] == "max")
            std::cout << "Find maximum bid or ask for product in current time step" << std::endl;
        else if (tokens[1] == "avg")
            std::cout << "compute average ask or bid for sent product over the sent number of time steps" << std::endl;
        else if (tokens[1] == "predict")
            std::cout << "predict max or min ask or bid for sent product for next time step" << std::endl;
        else if (tokens[1] == "time")
            std::cout << "state current time in dataset" << std::endl;
        else if (tokens[1] == "step")
            std::cout << "move to next time step" << std::endl;
    }

    return userInput;
            
}

void MerkelMain::processUserOption(const char* userInput)

{
    

}
j
