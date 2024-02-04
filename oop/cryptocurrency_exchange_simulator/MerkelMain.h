#pragma once
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "TextBasedCandlestickPlotter.h"
#include "VolumeProfilePlotter.h"

class MerkelMain
{
    public:
        MerkelMain();
        /** Call this to start the sim */
        void init();
        void printCandlestickData();  // MY PART OF THE CODE - New method to handle candlestick data
        void displayCandlesticks(const std::vector<Candlestick> &candlesticks); // MY PART OF THE CODE - Helper method for displaying candlesticks
        std::map<std::string, double> getVolumeData(const std::string &product, OrderBookType type); // New method for volume data
        double calculateVolumeForTimeFrame(const std::string &timeFrame, const std::string &product, OrderBookType type);
        void showVolumeProfile();

    private:
        void printMenu();
        void printHelp();
        void printMarketStats();
        void enterAsk();
        void enterBid();
        void printWallet();
        void gotoNextTimeframe();
        int getUserOption();
        void processUserOption(int userOption);


        std::string currentTime;

    OrderBook orderBook{"20200601.csv"};
    Wallet wallet;
};
