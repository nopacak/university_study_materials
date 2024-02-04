#include "OrderBook.h"
#include "CSVReader.h"
#include "Candlestick.h"
#include <map>
#include <algorithm>
#include <iostream>


/** construct, reading a csv data file */
OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
}

/** return vector of all know products in the dataset*/
std::vector<std::string> OrderBook::getKnownProducts()
{
    std::vector<std::string> products;

    std::map<std::string, bool> prodMap;

    for (OrderBookEntry &e : orders)
    {
        prodMap[e.product] = true;
    }

    // now flatten the map to a vector of strings
    for (auto const &e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}
/** return vector of Orders according to the sent filters*/
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                                 std::string product,
                                                 std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry &e : orders)
    {
        if (e.orderType == type &&
            e.product == product &&
            e.timestamp == timestamp)
        {
            orders_sub.push_back(e);
        }
    }
    return orders_sub;
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry> &orders)
{
    double max = orders[0].price;
    for (OrderBookEntry &e : orders)
    {
        if (e.price > max)
            max = e.price;
    }
    return max;
}

double OrderBook::getLowPrice(std::vector<OrderBookEntry> &orders)
{
    double min = orders[0].price;
    for (OrderBookEntry &e : orders)
    {
        if (e.price < min)
            min = e.price;
    }
    return min;
}

std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for (OrderBookEntry &e : orders)
    {
        if (e.timestamp > timestamp)
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
}


// MY PART OF THE CODE START - getting the previous timeframe


std::string OrderBook::getPreviousTime(std::string timestamp)
{
    std::string last_seen_timestamp = ""; // To keep track of the last seen timestamp
    for (OrderBookEntry &e : orders)
    {
        if (e.timestamp >= timestamp)
        {
            // If we reach or surpass the current timestamp, return the last seen
            break;
        }
        last_seen_timestamp = e.timestamp; // Update the last seen timestamp
    }

    // If no previous timestamp is found (i.e., if the input timestamp is the earliest),
    // wrap around to the last timestamp in the orders.
    if (last_seen_timestamp.empty() && !orders.empty())
    {
        last_seen_timestamp = orders.back().timestamp;
    }

    return last_seen_timestamp;
}


// MY PART OF THE CODE END




void OrderBook::insertOrder(OrderBookEntry &order)
{
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    // asks = orderbook.asks
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask,
                                                 product,
                                                 timestamp);
    // bids = orderbook.bids
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid,
                                                 product,
                                                 timestamp);

    // sales = []
    std::vector<OrderBookEntry> sales;

    // I put in a little check to ensure we have bids and asks
    // to process.
    if (asks.size() == 0 || bids.size() == 0)
    {
        std::cout << " OrderBook::matchAsksToBids no bids or asks" << std::endl;
        return sales;
    }

    // sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    // sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);
    // for ask in asks:
    std::cout << "max ask " << asks[asks.size() - 1].price << std::endl;
    std::cout << "min ask " << asks[0].price << std::endl;
    std::cout << "max bid " << bids[0].price << std::endl;
    std::cout << "min bid " << bids[bids.size() - 1].price << std::endl;

    for (OrderBookEntry &ask : asks)
    {
        //     for bid in bids:
        for (OrderBookEntry &bid : bids)
        {
            //         if bid.price >= ask.price # we have a match
            if (bid.price >= ask.price)
            {
                //             sale = new order()
                //             sale.price = ask.price
                OrderBookEntry sale{ask.price, 0, timestamp,
                                    product,
                                    OrderBookType::asksale};

                if (bid.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;
                }
                if (ask.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::asksale;
                }

                //             # now work out how much was sold and
                //             # create new bids and asks covering
                //             # anything that was not sold
                //             if bid.amount == ask.amount: # bid completely clears ask
                if (bid.amount == ask.amount)
                {
                    //                 sale.amount = ask.amount
                    sale.amount = ask.amount;
                    //                 sales.append(sale)
                    sales.push_back(sale);
                    //                 bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
                    //                 # can do no more with this ask
                    //                 # go onto the next ask
                    //                 break
                    break;
                }
                //           if bid.amount > ask.amount:  # ask is completely gone slice the bid
                if (bid.amount > ask.amount)
                {
                    //                 sale.amount = ask.amount
                    sale.amount = ask.amount;
                    //                 sales.append(sale)
                    sales.push_back(sale);
                    //                 # we adjust the bid in place
                    //                 # so it can be used to process the next ask
                    //                 bid.amount = bid.amount - ask.amount
                    bid.amount = bid.amount - ask.amount;
                    //                 # ask is completely gone, so go to next ask
                    //                 break
                    break;
                }

                //             if bid.amount < ask.amount # bid is completely gone, slice the ask
                if (bid.amount < ask.amount &&
                    bid.amount > 0)
                {
                    //                 sale.amount = bid.amount
                    sale.amount = bid.amount;
                    //                 sales.append(sale)
                    sales.push_back(sale);
                    //                 # update the ask
                    //                 # and allow further bids to process the remaining amount
                    //                 ask.amount = ask.amount - bid.amount
                    ask.amount = ask.amount - bid.amount;
                    //                 bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
                    //                 # some ask remains so go to the next bid
                    //                 continue
                    continue;
                }
            }
        }
    }
    return sales;
}

// MY PART OF THE CODE START - Implement the generateCandlestickData method
std::vector<Candlestick> OrderBook::generateCandlestickData(const std::string &product,
                                                            OrderBookType type,
                                                            const std::vector<std::string> &timeframes)
{
    std::vector<Candlestick> candlesticks;

    for (size_t i = 0; i < timeframes.size(); ++i)
    {
        std::string currentFrame = timeframes[i];
        std::string previousFrame = i > 0 ? timeframes[i - 1] : "";

        std::vector<OrderBookEntry> currentEntries = getOrders(type, product, currentFrame);
        std::vector<OrderBookEntry> previousEntries = previousFrame.empty() ? std::vector<OrderBookEntry>() : getOrders(type, product, previousFrame);

        double open = calculateMeanPrice(previousEntries);
        double close = calculateMeanPrice(currentEntries);
        double high = getMaxPrice(currentEntries);
        double low = getMinPrice(currentEntries);

        Candlestick candlestick(currentFrame, open, high, low, close);
        candlesticks.push_back(candlestick);
    }

    return candlesticks;
}

// Implement the calculateMeanPrice helper method
double OrderBook::calculateMeanPrice(const std::vector<OrderBookEntry> &entries)
{
    double totalValue = 0.0;
    double totalAmount = 0.0;

    for (const OrderBookEntry &entry : entries)
    {
        totalValue += entry.price * entry.amount;
        totalAmount += entry.amount;
    }

    return totalAmount == 0 ? 0 : totalValue / totalAmount;
}

// Implement the getMaxPrice helper method
double OrderBook::getMaxPrice(const std::vector<OrderBookEntry> &entries)
{
    double maxPrice = std::numeric_limits<double>::lowest();
    for (const OrderBookEntry &entry : entries)
    {
        if (entry.price > maxPrice)
            maxPrice = entry.price;
    }
    return maxPrice;
}

// Implement the getMinPrice helper method
double OrderBook::getMinPrice(const std::vector<OrderBookEntry> &entries)
{
    double minPrice = std::numeric_limits<double>::max();
    for (const OrderBookEntry &entry : entries)
    {
        if (entry.price < minPrice)
            minPrice = entry.price;
    }
    return minPrice;
}

// MY PART OF THE CODE END