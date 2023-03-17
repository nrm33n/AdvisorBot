#pragma once
#include <string>

/// <summary>
/// if it is an ask, u are losing currency a and gaining currency b
/// if bid, you are gaining currency a and losing currency b
/// eg OrderBookType::asksale on product ETH/BTC, lose ETH, gain BTC
/// </summary>
enum class OrderBookType { bid, ask, unknown, asksale, bidsale };

class OrderBookEntry
{
    public:

    OrderBookEntry( double _price,
                    double _amount,
                    std::string _timestamp,
                    std::string _product,
                    OrderBookType _orderType,
                    std::string username = "dataset");

    static OrderBookType stringToOrderBookType(std::string s);

    static bool compareByTimestamp(OrderBookEntry& e1, OrderBookEntry& e2)
    {
        return e1.timestamp < e2.timestamp;
    }
    //create this to get the previous timestamp 
    static bool timestampDesc(OrderBookEntry& e1, OrderBookEntry& e2)
    {
        return e1.timestamp > e2.timestamp;
    }
    static bool compareByPriceAsc(OrderBookEntry& e1, OrderBookEntry& e2)
    {
        return e1.price < e2.price;
    }
    static bool compareByPriceDesc(OrderBookEntry& e1, OrderBookEntry& e2)
    {
        return e1.price > e2.price;
    }

    double price;
    double amount;
    std::string timestamp;
    std::string product;
    OrderBookType orderType;
    std::string username;
};
