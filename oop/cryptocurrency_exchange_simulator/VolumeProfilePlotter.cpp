// MY PART OF THE CODE START
#include "VolumeProfilePlotter.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <limits>

VolumeProfilePlotter::VolumeProfilePlotter(int height)
    : plotHeight(height)
{
}

void VolumeProfilePlotter::plotVolumeProfile(const std::map<std::string, double> &volumeByTimeframe)
{
    double maxVolume = findMaxVolume(volumeByTimeframe);

    // Looping from top to bottom of the plot
    for (int row = plotHeight - 1; row >= 0; --row)
    {
        for (const auto &pair : volumeByTimeframe)
        {
            const std::string &timeframe = pair.first;
            const double &volume = pair.second;

            if (row == 0)
            {
                // Print the timeframe at the bottom row
                std::cout << timeframe << " ";
            }
            else
            {
                std::cout << generateVolumeLine(volume, maxVolume, row);
            }
        }
        std::cout << '\n';
    }
}

double VolumeProfilePlotter::findMaxVolume(const std::map<std::string, double> &volumeByTimeframe)
{
    double maxVolume = 0;
    for (const auto &pair : volumeByTimeframe)
    {
        maxVolume = std::max(maxVolume, pair.second);
    }
    return maxVolume;
}

int VolumeProfilePlotter::scaleVolumeToPlotHeight(double volume, double maxVolume)
{
    return static_cast<int>((volume / maxVolume) * plotHeight);
}

std::string VolumeProfilePlotter::generateVolumeLine(double volume, double maxVolume, int row)
{
    int scaledVolume = scaleVolumeToPlotHeight(volume, maxVolume);
    if (row <= scaledVolume)
    {
        return "# ";
    }
    return "  ";
}

// MY PART OF THE CODE END