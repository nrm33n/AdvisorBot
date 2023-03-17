#include "advisorbot.h"
#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <string>
#include "CSVReader.h"
#include "OrderBook.h"

advisorbot::advisorbot() {
}

void advisorbot::init()
{
    std::vector<std::string> input;
    currentTime = orderBook.getEarliestTime();
    printMenu();

    while (true)
    {
        std::cout << "___________________________________" << std::endl;
        input = getUserOption();
    }
}

void advisorbot::printMenu()
{
    std::cout << "Hello! I am advisorbot." << std::endl;
    std::cout << "Please enter a command to get started. Type 'help' for help." << std::endl;
}

/** list all available commands*/
void advisorbot::printHelp() //C1 --> help
{
    std::cout << "The available commands are help, help <cmd>, prod, min, max, avg, predict, time, step, trend, bye." << std::endl; 
}

/** description of commands*/
void advisorbot::printCmd(std::vector<std::string> tokens) //C2 --> help cmd
{
    if (tokens.size() > 1) {
        if (tokens[1] == "cmd") {
            std::cout << "Provides purpose of selected command." << std::endl;
            std::cout << "E.g. help prod" << std::endl;
        }
        else if (tokens[1] == "prod")
            std::cout << "Lists available products" << std::endl;
        else if (tokens[1] == "min") {
            std::cout << "Find minimum bid or ask for product in current time step" << std::endl;
            std::cout << "Enter command in the form: min product ask/bid" << std::endl;
            std::cout << "E.g. min DOGE/BTC ask" << std::endl;
        }
        else if (tokens[1] == "max") {
            std::cout << "Find maximum bid or ask for product in current time step" << std::endl;
            std::cout << "Enter command in the form: max product ask/bid" << std::endl;
            std::cout << "E.g. max BTC/USDT bid" << std::endl;
        }
        else if (tokens[1] == "avg") {
            std::cout << "compute average ask or bid for sent product over the sent number of time steps" << std::endl;
            std::cout << "Enter command in the form: avg product ask/bid timestep" << std::endl;
            std::cout << "E.g. avg ETH/BTC ask 10" << std::endl;
        }
        else if (tokens[1] == "predict") {
            std::cout << "predict max or min ask or bid for sent product for next time step using data from all previous timesteps" << std::endl;
            std::cout << "Enter command in the form: predict min/max product ask/bid timestep" << std::endl;
            std::cout << "E.g. predict min ETH/BTC ask 10" << std::endl;
        }
        else if (tokens[1] == "time")
            std::cout << "state current time in dataset" << std::endl;
        else if (tokens[1] == "step")
            std::cout << "move to next time step" << std::endl;
        else if (tokens[1] == "trend")
            std::cout << "iterates over all previous timesteps to determine the overall trend of the given product" << std::endl;
        else if (tokens[1] == "bye")
            std::cout << "ends the program" << std::endl;
        else //if string entered is not a valid command 
            std::cout << "Sorry! I didn't quite catch that. Please try again using an acceptable command." << std::endl;
    }
}

/** list all available products*/
void advisorbot::printMarketProd() //C3 --> prod
{
    //list available products 
    for (std::string const& p : orderBook.getKnownProducts()) {
        std::cout << "Product: " << p << std::endl;
    }
}

/** find minimum bid or ask for prod in current time step*/
void advisorbot::printMin(std::vector<std::string> tokens) //C4 --> min
{
    //eg user> min ETH/BTC ask 
    // advisorbot> The min ask for ETH/BTC is 1.0
    
    if (tokens.size() != 3)
    {
        std::cout << "Bad input! Please print your command in the form: min product ask/bid" << std::endl;
    }
    else {
        try {
                std::string h = tokens[1]; //store tokens in h so they can be passed into orderBook.getOrders( ) 

                if (tokens[2] == "ask") { 

                    std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, h, currentTime);
                    std::cout << "Product: " << h << std::endl;
                    std::cout << "Min asks: " << OrderBook::getLowPrice(entries) << std::endl;
                }

                if (tokens[2] == "bid") {
                    std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid, h, currentTime);
                    std::cout << "Product: " << tokens[1] << std::endl;
                    std::cout << "Min bids: " << OrderBook::getLowPrice(entries) << std::endl;

                }
        }
        catch (const std::exception& e)
        {
            std::cout << "Sorry! This command could not be completed. Please try again." << std::endl;
        };
    } 
}

/** find max bid/ask for product in current timestep*/
void advisorbot::printMax(std::vector<std::string> tokens) //C5 --> max
{
    //eg user> max ETH/BTC bid
    //advisorbot> the max bid for ETH/BTC bid is 4
    if (tokens.size() != 3)
    {
        std::cout << "Bad input! Please try again using the form: max product ask/bid" << std::endl;
    }
    else {
        try {
            std::string h = tokens[1]; //store products as a string so user inputted product can be passed into orderBook.getOrders( ) 

            if (tokens[2] == "ask") {

                std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, h, currentTime);
                std::cout << "Product: " << h << std::endl;
                std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;

            }
            if (tokens[2] == "bid") {
                std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid, h, currentTime);
                std::cout << "Product: " << tokens[1] << std::endl;
                std::cout << "Max bid: " << OrderBook::getHighPrice(entries) << std::endl;

            }
        }
        catch (const std::exception& e) {
            std::cout << "Sorry! This command could not be carried out. Please try again." << std::endl;
        };
    }      
}

/** compute average ask or bid for the sent product over the sent number of timesteps*/
void advisorbot::printAvg(std::vector<std::string> tokens) //C6 --> avg 
{
    //eg: user> avg ETH/BTC ask 10 
    //advisorbot> The average ETH/BTC ask price over the last 10 timesteps was 1.0
    if (tokens.size() != 4) {
        std::cout << "Bad input!" << std::endl;
        std::cout << "Please try again. Enter your command in the form: avg prod ask/bid timesteps." << std::endl;
    }
    else {
        try {
            //store current time in string so the following processes do not change it for the user
            std::string timestamp = currentTime;
                std::string prod = tokens[1];
                std::string askbid = tokens[2];
                int steps = stoi(tokens[3]);

                //create an array to store all the average prices over each timestep we iterate over 
                std::vector<double>priceAvg = {};

                if (tokens[2] == "ask") {
                    for (int i = 0; 1 <= i <= 2; i++) {
                        //please uncomment the following code to verify that this function works as it should 
                        //std::cout << "timestamp: " << timestamp << std::endl;
                        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, prod, timestamp);
                        //for each iteration over each timestep, push the average of the highest and lowest values into the priceAvg array vector
                        priceAvg.push_back((OrderBook::getHighPrice(entries) + OrderBook::getLowPrice(entries)) / 2);
                        //get previous timestamps 
                        timestamp = orderBook.getPrevTime(timestamp);
                        //stop program from infinitely looping by breaking when user inputted timesteps reached
                        if (i == steps - 1)
                            break;
                    }
                }
                else if (tokens[2] == "bid") {
                
                    for (int i = 0; 0 <= i <= 2; i++) {
                        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid, prod, timestamp);
                        std::cout << "timestamp: " << timestamp << std::endl;
                        //push avg of highest and lowest values for each timestep into array vector 
                        priceAvg.push_back((OrderBook::getHighPrice(entries) + OrderBook::getLowPrice(entries)) / 2);
                        timestamp = orderBook.getPrevTime(timestamp);
                        //stop program from infinitely looping by breaking when user inputted timesteps reached
                        if (i == steps - 1)
                            break;
                    }
               
                }
                else {
                    //if the user does not specify ask or bid, output the following 
                    std::cout << "Please enter your command in the form: avg product ask/bid timesteps" << std::endl;
                    std::cout << "For example: avg ETH/BTC ask 10" << std::endl;
                }

           //add up all the values in the priceAvg array 
            double totalAvg = 0;
            for (auto& num : priceAvg)
                totalAvg += num;
         
            //divide the total of the average over each timestep by the number of timesteps to create a final average 
            double finalAvg; 
            finalAvg = totalAvg / steps; 

            std::cout << "The average " << prod << " " << askbid << " price over the last " << steps << " timesteps was " << finalAvg << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Sorry! This command could not be carried out. Please try again." << std::endl;
        };
    }
}

/** predict max/min product ask/bid for the next timestep given previous timesteps*/
void advisorbot::printPredict(std::vector<std::string> tokens) //C7 --> predict 
{  //eg user> predict max ETH/BTC bid 10

    //check for token size first to see if string vector can be processed 
    if (tokens.size() != 4) {
        std::cout << "Bad input!" << std::endl;
        std::cout << "Please try again. Enter your command in the form: predict max/min prod ask/bid" << std::endl;
    }
    else { //if token amount is correct, enter try/catch block 
        std::string timestamp = currentTime;
        std::string earliest = orderBook.getEarliestTime();
        if (timestamp == earliest) {
            std::cout << "Sorry! Not enough data to predict. Please try again from a later timestep." << std::endl;
        }
        else{
            
            try {
                std::string timestamp = currentTime;
                //std::string earliest = orderBook.getEarliestTime();
                std::string earliest = "2020/06/01 11:57:30.328127";
                std::string prod = tokens[2];
                int counter = 0;
                if (tokens[1] == "max") {
                    std::vector<double> highVals = {};

                    if (tokens[3] == "ask") {
                        for (int i = 0; 1 <= i <= 2; i++) {
                            std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, prod, timestamp);
                            //push highest values at each timestep into an array to be processed later 
                            highVals.push_back(OrderBook::getHighPrice(entries));
                            std::cout << "At timestep " << timestamp << " the highest ask price is " << orderBook.getHighPrice(entries) << std::endl;
                            //go backwards in time to previous timestamps 
                            timestamp = orderBook.getPrevTime(timestamp);
                            //increment counter 
                            counter += 1;
                            if (timestamp == earliest)
                                break;
                        }
                    }

                    else if (tokens[3] == "bid") {
                        for (int i = 0; 1 <= i <= 2; i++) {
                            std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid, prod, timestamp);
                            //push highest vid values into an array to be processed later 
                            highVals.push_back(OrderBook::getHighPrice(entries));
                            std::cout << "At timestep " << timestamp << " the highest bid price is " << orderBook.getHighPrice(entries) << std::endl;
                            //iterate backwards over csv file timestamps 
                            timestamp = orderBook.getPrevTime(timestamp);
                            // increment counter
                            counter += 1;
                            //prevent infinite loop
                            if (timestamp == earliest)
                                break; 
                        }
                    }
                    //add up every value in the array of highest values from each timestep 
                    double highAvg = 0;
                    for (auto& num : highVals)
                        highAvg += num;

                    //divide the added up array by number of timesteps to recieve average 
                    double highFinal;
                    highFinal = highAvg / counter;

                    std::cout << "The predicted " << tokens[1] << " value for " << prod << " using data from " << counter << " previous timesteps is " << highFinal << std::endl;
                    std::cout << "The final timestamp is: " << timestamp << std::endl;
                }

                else if (tokens[1] == "min")
                {
                    std::string prod = tokens[2];
                    int counter = 0;
                    std::vector<double> lowVals = {};

                    if (tokens[3] == "ask") {
                        for (int i = 0; 1 <= i <= 2; i++) {
                            //std::cout << "timestamp: " << timestamp << std::endl;
                            std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, prod, timestamp);
                            //push lowest ask prices over each timestep to vector array
                            lowVals.push_back(OrderBook::getLowPrice(entries));
                            std::cout << "At timestep " << timestamp << " the lowest ask price is " << orderBook.getLowPrice(entries) << std::endl;
                            //get previous timesteps 
                            timestamp = orderBook.getPrevTime(timestamp);
                            //increment counter 
                            counter += 1;
                            if (timestamp == earliest)
                                break;
                        }
                    }

                    else if (tokens[3] == "bid") {
                        for (int i = 0; 1 <= i <= 2; i++) {
                            //std::cout << "timestamp: " << timestamp << std::endl;
                            std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid, prod, timestamp);
                            //push lowest bid prices over each timestep to vector array 
                            lowVals.push_back(OrderBook::getLowPrice(entries));
                            std::cout << "At timestep " << timestamp << " the lowest bid price is " << orderBook.getLowPrice(entries) << std::endl;
                            //iterate over previous timesteps
                            timestamp = orderBook.getPrevTime(timestamp);
                            //increment counter 
                            counter += 1;
                            if (timestamp == earliest)
                                break;
                        }
                    }

                    //add up all lowest values for each iteration in the array
                    double lowAvg = 0;
                    for (auto& num : lowVals)
                        lowAvg += num;

                    //divide the added up value of lowest values by number of timesteps to get average lowest value
                    double lowFinal;
                    lowFinal = lowAvg / counter;
                    std::cout << "The predicted " << tokens[1] << " value for " << prod << " using data from " << counter << " previous timesteps is " << lowFinal << std::endl;

                }
                else
                    std::cout << "Please make sure you enter a command in the form: predict max/min product ask/bid timesteps" << std::endl;

            }
            catch (const std::exception& e)
            {
                std::cout << "Sorry! This command could not be carried out. Please try again." << std::endl;
            };
        }
    }
}

void advisorbot::currentTimeStep() //C8 --> time 
{
    //state current time 
    std::cout << "Current time is: " << currentTime << std::endl;
}

void advisorbot::nextTimeFrame() //C9 --> step
{
    //next time frame 
    std::string last = "2020/06/01 14:56:35.210165";
    std::string rn = currentTime;
    //make sure the user cannot move to the next timestep if they are already at the last one
    if (rn == last) {
        std::cout << "You are at the latest timestep." << std::endl;
    }
    else {
        //move to the next timestep
        std::cout << "Going to next time frame." << std::endl;
        currentTime = orderBook.getNextTime(currentTime);
        std::cout << "Now at " << currentTime << std::endl;
    }
}

void advisorbot::printTrend(std::vector<std::string> tokens)
{ //eg. user> trend ETH/BTC ask 

    if (tokens.size() != 3) {
        std::cout << "Bad input!" << std::endl;
        std::cout << "Please try again. Enter your command in the form: trend prod ask/bid" << std::endl;
    }
    else {
        std::string timestamp = currentTime;
        //std::string earliest = orderBook.getEarliestTime();
        std::string earliest = "2020/06/01 11:57:30.328127";
        if (timestamp == earliest) {
            std::cout << "Sorry! Not enough data to find a trend. Please try again from a later timestep." << std::endl;
        }
        else {
            std::string prod = tokens[1];
            //create a vector to store the average of each product 
            std::vector<double> priceAvg = {};
            int counter = 0;
            if (tokens[2] == "ask") {
                
                for (int i = 0; 1 <= i <= 2; i++) {
                    std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, prod, timestamp);
                    //push lowest+ highest avg ask prices over each timestep to vector array
                    priceAvg.push_back((OrderBook::getHighPrice(entries) + OrderBook::getLowPrice(entries)) / 2);
                    //get previous timesteps 
                    timestamp = orderBook.getPrevTime(timestamp);
                    //increment counter 
                    counter += 1;
                    if (timestamp == earliest)
                        break;
                }
            }
            else if (tokens[2] == "bid") {
                //int counter = 0;
                for (int i = 0; 1 <= i <= 2; i++) {
                    std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid, prod, timestamp);
                    //push lowest+ highest avg ask prices over each timestep to vector array
                    priceAvg.push_back((OrderBook::getHighPrice(entries) + OrderBook::getLowPrice(entries)) / 2);
                    //get previous timesteps 
                    timestamp = orderBook.getPrevTime(timestamp);
                    //increment counter 
                    counter += 1;
                    //avoid infinite loop
                    if (timestamp == earliest)
                        break;
                }
            }
            else {
                std::cout << "Please make sure you enter a command in the form: trend prod ask/bid" << std::endl;
            }

            //create a vector to store each plot point for potential graph based off this data
            std::vector<double> point;
            // use float instead of int as it is larger, less data loss
            for (float i = 0; 0 <= i <= priceAvg.size(); i++) {
                //iterate over entire vector to find the slope between each point 
                point.push_back((((i + 1) - i) / (priceAvg[i + 1] - priceAvg[i])));
                //print it out 
                //std::cout << point[i] << std::endl;
                //stop the iteration
                if (i == priceAvg.size())
                    break;
            }

            //just the slope of the first and last data points 
            double end = point.size();
            double final = end / (point[end] - point[0]);

            std::cout << "The overall linear trend is: " << final << std::endl;

            if (final < 0) {
                std::cout << "The " << tokens[2] << " price of for " << prod << " is falling according to data from the last " << counter << " timesteps." << std::endl;
            }
            else if (final > 0) {
                std::cout << "The " << tokens[2] << " price of for " << prod << " is rising according to data from the last " << counter << " timesteps." << std::endl;
            }
            else if (final == 0) {
                std::cout << "The " << tokens[2] << " price of for " << prod << " is unchanged according to data from the last " << counter << " timesteps." << std::endl;
            }
            else
                std::cout << "Something went wrong." << std::endl;
        }

    }
}

std::vector<std::string> advisorbot::getUserOption()
{
    //user input string 
    std::string userInput;
    std::getline(std::cin, userInput);

    //tokenise string using blank space delimiter 
    std::vector<std::string> tokens = CSVReader::tokenise(userInput, ' ');

    //execute function depending on token at index 0
    //if no commands match the inputted string, return userInput, state this is not a valid command
    if (tokens[0] == "help" && tokens.size() == 1)
        printHelp();
    else if (tokens[0] == "help" && tokens.size() == 2)
        printCmd(tokens);
    else if (tokens[0] == "prod")
        printMarketProd();
    else if (tokens[0] == "min")
        printMin(tokens);
    else if (tokens[0] == "max")
        printMax(tokens);
    else if (tokens[0] == "avg")
        printAvg(tokens);
    else if (tokens[0] == "predict")
        printPredict(tokens);
    else if (tokens[0] == "time")
        currentTimeStep();
    else if (tokens[0] == "step")
        nextTimeFrame();
    else if (tokens[0] == "trend")
        printTrend(tokens);
    else if (tokens[0] == "bye") {
        std::cout << "Exiting.. Press any key to complete exit." << std::endl;
        std::cout << "Bye bye!" << std::endl;
        std::exit;
    }
    else {
        std::cout << "Sorry! This is not a valid command. Please type 'help' for help." << std::endl;
        for (std::string const& userInput : tokens) {
             std::cout << "You entered: " << userInput << std::endl;
        }
    }
    return tokens;
}


