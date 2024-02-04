// MY PART OF THE CODE START 

#pragma once

#include <string>

// The Candlestick class represents a single candlestick in a candlestick chart,
// containing the open, high, low, and close prices for a given timeframe.
class Candlestick
{
public:
    // Constructor to initialize the candlestick with its date and price data.
    Candlestick(std::string date, double open, double high, double low, double close);

    // Getters to access the private member variables.
    std::string getDate() const;
    double getOpen() const;
    double getHigh() const;
    double getLow() const;
    double getClose() const;

private:
    std::string date; // The date of the candlestick.
    double open;      // The opening price.
    double high;      // The highest price.
    double low;       // The lowest price.
    double close;     // The closing price.
};


// MY PART OF THE CODE END
