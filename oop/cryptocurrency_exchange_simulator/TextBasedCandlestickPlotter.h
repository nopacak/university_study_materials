// MY PART OF THE CODE START
#include "Candlestick.h"
#include <vector>
#include <string>

class TextBasedCandlestickPlotter
{
public:
    explicit TextBasedCandlestickPlotter(int height);
    void plotCandlesticks(const std::vector<Candlestick> &candlesticks);

private:
    int plotHeight;  // The height of the plot in text lines
    double minPrice; // Minimum price in the dataset
    double maxPrice; // Maximum price in the dataset

    void findMinMaxPrices(const std::vector<Candlestick> &candlesticks);
    int scalePriceToPlotHeight(double price);
    std::string generateCandlestickLine(const Candlestick &candle, int row);
};

// MY PART OF THE CODE END