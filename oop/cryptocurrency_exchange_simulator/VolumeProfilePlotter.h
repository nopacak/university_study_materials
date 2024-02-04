// MY PART OF THE CODE START
#include <vector>
#include <string>
#include <map>
#include "Candlestick.h" // Include this if Candlestick class contains volume data

class VolumeProfilePlotter
{
public:
    // Constructor: Initializes the plotter with a specific plot height
    explicit VolumeProfilePlotter(int height);

    // Method to plot the volume profile given a map of timeframes and their corresponding volumes
    void plotVolumeProfile(const std::map<std::string, double> &volumeByTimeframe);

private:
    int plotHeight; // Height of the plot in text lines

    // Finds the maximum volume in the given data to scale the plot
    double findMaxVolume(const std::map<std::string, double> &volumeByTimeframe);

    // Scales the given volume to fit within the plot's height
    int scaleVolumeToPlotHeight(double volume, double maxVolume);

    // Generates a single line of the volume plot for a specific timeframe
    std::string generateVolumeLine(double volume, double maxVolume, int row);

};

// MY PART OF THE CODE END