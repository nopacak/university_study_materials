// MY PART OF THE CODE START
#include "TextBasedCandlestickPlotter.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>

TextBasedCandlestickPlotter::TextBasedCandlestickPlotter(int height)
    : plotHeight(height) {}

void TextBasedCandlestickPlotter::plotCandlesticks(const std::vector<Candlestick> &candlesticks)
{
    findMinMaxPrices(candlesticks);
    for (int row = plotHeight - 1; row >= 0; --row)
    {
        for (const auto &candle : candlesticks)
        {
            std::cout << generateCandlestickLine(candle, row);
        }
        std::cout << '\n';
    }
}

void TextBasedCandlestickPlotter::findMinMaxPrices(const std::vector<Candlestick> &candlesticks)
{
    minPrice = std::numeric_limits<double>::max();
    maxPrice = std::numeric_limits<double>::min();
    for (const auto &candle : candlesticks)
    {
        minPrice = std::min(minPrice, candle.getLow());
        maxPrice = std::max(maxPrice, candle.getHigh());
    }
}

int TextBasedCandlestickPlotter::scalePriceToPlotHeight(double price)
{
    double range = maxPrice - minPrice;
    return static_cast<int>((price - minPrice) / range * (plotHeight - 1));
}

std::string TextBasedCandlestickPlotter::generateCandlestickLine(const Candlestick &candle, int row)
{
    int openLine = scalePriceToPlotHeight(candle.getOpen());
    int closeLine = scalePriceToPlotHeight(candle.getClose());
    int highLine = scalePriceToPlotHeight(candle.getHigh());
    int lowLine = scalePriceToPlotHeight(candle.getLow());

    std::ostringstream lineStream;

    // Determine the character to use based on the row and candlestick data
    if (row == highLine || row == lowLine)
    {
        lineStream << "  -  ";
    }
    else if (row >= std::min(openLine, closeLine) && row <= std::max(openLine, closeLine))
    {
        lineStream << " | | ";
    }
    else if (row < highLine && row > openLine || row < closeLine && row > lowLine)
    {
        lineStream << "  |  ";
    }
    else
    {
        lineStream << "     ";
    }

    // Append two tabs for spacing between candlesticks
    lineStream << "\t\t";

    return lineStream.str();
}

// MY PART OF THE CODE END