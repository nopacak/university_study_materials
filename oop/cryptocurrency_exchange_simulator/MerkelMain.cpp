#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"


MerkelMain::MerkelMain()
{
}

void MerkelMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 10);

    while (true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}

void MerkelMain::printMenu()
{
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an ask
    std::cout << "3: Make an ask " << std::endl;
    // 4 make a bid
    std::cout << "4: Make a bid " << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;
    // 6 continue
    std::cout << "6: Continue " << std::endl;

    // MY PART OF THE CODE START- new menu options added
    std::cout << "7: Show Candlestick Data" << std::endl;
    std::cout << "8: Show Volume Profile" << std::endl;
    // MY PART OF THE CODE END

    std::cout << "============== " << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketStats()
{
    for (std::string const &p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                                  p, currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
    }
}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else
    {
        try
        {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::ask);
            obe.username = "simuser";
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else
            {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << " MerkelMain::enterAsk Bad input " << std::endl;
        }
    }
}

void MerkelMain::enterBid()
{
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else
    {
        try
        {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid);
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else
            {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }
    }
}

void MerkelMain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}


void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    for (std::string p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry &sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            if (sale.username == "simuser")
            {
                // update the wallet
                wallet.processSale(sale);
            }
        }
    }

    currentTime = orderBook.getNextTime(currentTime);
}





// MY PART OF THE CODE START - Function to prompt the user for the necessary inputs and display the calculated candlestick data.

void MerkelMain::printCandlestickData()
{

    std::cout << "Get Candlestick Data - enter the product and order type, eg  ETH/BTC, bid" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    if (tokens.size() != 2)
    {
        std::cout << "MerkelMain::printCandlestickData Bad input! " << input << std::endl;
    }
    std::string product = tokens[0];
    OrderBookType type = OrderBookEntry::stringToOrderBookType(tokens[1]);

    std::string currentFrame = currentTime;
    std::string previousFrame = orderBook.getPreviousTime(currentTime);

    std::vector<std::string> timeframes = {previousFrame, currentFrame};

    std::vector<Candlestick> candlesticks = orderBook.generateCandlestickData(product, type, timeframes);
    displayCandlesticks(candlesticks);

    TextBasedCandlestickPlotter plotter(15);
    plotter.plotCandlesticks(candlesticks);
}

void MerkelMain::displayCandlesticks(const std::vector<Candlestick> &candlesticks)
{
    for (const Candlestick &candle : candlesticks)
    {
        std::cout << "Date: " << candle.getDate()
                  << ", Open: " << candle.getOpen()
                  << ", High: " << candle.getHigh()
                  << ", Low: " << candle.getLow()
                  << ", Close: " << candle.getClose() << std::endl;
    }
}

std::map<std::string, double> MerkelMain::getVolumeData(const std::string &product, OrderBookType type)
{
    std::map<std::string, double> volumeData;

    int numberOfTimeframes = 5; // Example: 5 timeframes before and after the current time
    std::string timeFrame = currentTime;

    for (int i = 0; i < numberOfTimeframes; ++i)
    {
        timeFrame = orderBook.getPreviousTime(timeFrame);
        double volume = calculateVolumeForTimeFrame(timeFrame, product, type);
        volumeData[timeFrame] = volume;
    }

    timeFrame = currentTime;
    for (int i = 0; i <= numberOfTimeframes; ++i)
    {
        double volume = calculateVolumeForTimeFrame(timeFrame, product, type);
        volumeData[timeFrame] = volume;
        timeFrame = orderBook.getNextTime(timeFrame);
    }

    return volumeData;
}

double MerkelMain::calculateVolumeForTimeFrame(const std::string &timeFrame, const std::string &product, OrderBookType type)
{
    double totalVolume = 0.0;
    std::vector<OrderBookEntry> orders = orderBook.getOrders(type, product, timeFrame);
    for (const auto &order : orders)
    {
        totalVolume += order.amount;
    }
    return totalVolume;
}

void MerkelMain::showVolumeProfile()
{
    std::cout << "Get Volume Profile Data - enter the product and order type, eg  ETH/BTC, bid" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    if (tokens.size() != 2)
    {
        std::cout << "MerkelMain::printCandlestickData Bad input! " << input << std::endl;
    }
    std::string product = tokens[0];
    OrderBookType type = OrderBookEntry::stringToOrderBookType(tokens[1]);

    // Get volume data
    std::map<std::string, double> volumeData = getVolumeData(product, type);

    // Plot the volume profile
    VolumeProfilePlotter plotter(10); // 10 is an example plot height
    plotter.plotVolumeProfile(volumeData);
}

// MY PART OF THE CODE END





int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-8" << std::endl;
    std::getline(std::cin, line);
    try
    {
        userOption = std::stoi(line);
    }
    catch (const std::exception &e)
    {
        //
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::processUserOption(int userOption)
{
    if (userOption == 0) // bad input
    {
        std::cout << "Invalid choice. Choose 1-8" << std::endl;
    }
    if (userOption == 1)
    {
        printHelp();
    }
    if (userOption == 2)
    {
        printMarketStats();
    }
    if (userOption == 3)
    {
        enterAsk();
    }
    if (userOption == 4)
    {
        enterBid();
    }
    if (userOption == 5)
    {
        printWallet();
    }
    if (userOption == 6)
    {
        gotoNextTimeframe();
    }
    // MY PART OF THE CODE START - Additional user input processing
    if (userOption == 7)
    {
        printCandlestickData();
    }
    if (userOption == 8)
    {
        showVolumeProfile();
    }
    // MY PART OF THE CODE END
}
