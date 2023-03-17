#pragma once
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

class MerkelMain
{
    public: 
        MerkelMain();
        /**Call this to start the sim*/
        void init();
    private:
        //void loadOrderBook();
        void printMenu();
        void printMarketStats(); //2
        void enterAsk(); //3
        void enterBid(); //4
        void printWallet(); //5
        void nextTimeFrame(); //--C9
        std::string getUserOption();
        void processUserOption(const char* userOption); 

        std::string currentTime;

        OrderBook orderBook{"orderbook.csv"};
 
        Wallet wallet;
};

