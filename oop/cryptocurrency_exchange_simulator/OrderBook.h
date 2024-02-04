#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "Candlestick.h"
#include <string>
#include <vector>

class OrderBook
{
    public:
        /** construct, reading a csv data file */
        OrderBook(std::string filename);
        /** return vector of all known products in the dataset*/
        std::vector<std::string> getKnownProducts();
        /** return vector of Orders according to the sent filters*/
        std::vector<OrderBookEntry> getOrders(OrderBookType type,
                                              std::string product,
                                              std::string timestamp);

        /** returns the earliest time in the orderbook*/
        std::string getEarliestTime();
        /** returns the next time after the 
             * sent time in the orderbook  
             * If there is no next timestamp, wraps around to the start
             * */
        std::string getNextTime(std::string timestamp);
        std::string getPreviousTime(std::string timestamp);

        void insertOrder(OrderBookEntry &order);

        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

        static double getHighPrice(std::vector<OrderBookEntry> &orders);
        static double getLowPrice(std::vector<OrderBookEntry> &orders);




        // MY PART OF THE CODE START - Method to generate candlestick data for a given product and order type across timeframes.
        std::vector<Candlestick> generateCandlestickData(const std::string &product,
                                                         OrderBookType type,
                                                         const std::vector<std::string> &timeframes);

        // MY PART OF THE CODE END




    private:
        std::vector<OrderBookEntry> orders;

        // MY PART OF THE CODE START
        // Helper method to calculate the mean price from a list of orders.
        double calculateMeanPrice(const std::vector<OrderBookEntry> &entries);

        // Helper method to find the maximum price from a list of orders.
        double getMaxPrice(const std::vector<OrderBookEntry> &entries);

        // Helper method to find the minimum price from a list of orders.
        double getMinPrice(const std::vector<OrderBookEntry> &entries);

        // MY PART OF THE CODE END


};
