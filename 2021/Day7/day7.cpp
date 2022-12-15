// AoC21 Day 7:
// P1 - Save fuel by aligning numbers together by distance
// P2 - Save fuel by aligning numbers together by distance with non-constant fuel economy

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include <math.h>

int main()
{
    std::ifstream file("input.txt"); // Open the input file and store it in a list
    std::string s;
    file >> s;
    std::stringstream s_stream(s); //create string stream from the string
    std::list<unsigned short> crabPositions;
    while(s_stream.good()) {
        std::string substr;
        unsigned short val;
        getline(s_stream, substr, ','); //get first string delimited by comma
        val = std::stoi(substr);
        crabPositions.push_back(val);
    }
//==============================================================================
// Part 1
    {
        double avg = 0;
        for (auto& pos : crabPositions)
            avg += pos;
        avg /= crabPositions.size();
        double stdDev = 0;
        for (auto& pos : crabPositions)
            stdDev += ((pos - avg) * (pos - avg));
        stdDev /= crabPositions.size();
        stdDev = sqrt(stdDev);
        std::cout << "Average -> " << std::to_string(avg) << " Standard Deviation -> " << std::to_string(stdDev) << "\n";

        // Brute Force all values
        {
            unsigned int minDistance = 0xFFFFFFFF;
            int runningDistance = 0;
            int minPos = 0;
            int minVal = *(std::min_element(crabPositions.begin(), crabPositions.end()));
            int maxVal = *(std::max_element(crabPositions.begin(), crabPositions.end()));
            for (int i=maxVal*-1; i<=maxVal; ++i)
            {
                for (auto& pos : crabPositions)
                {
                    if (i > pos)
                        runningDistance += i - pos;
                    else
                        runningDistance += pos - i;
                }
                //std::cout << std::to_string(i) << " " << std::to_string(runningDistance) << "\n";
                if (runningDistance < minDistance)
                {
                    minDistance = runningDistance;
                    minPos = i;
                }
                runningDistance = 0;
            }
            std::cout << "Min position -> " << std::to_string(minPos) << " Min distance -> " << std::to_string(minDistance) << "\n";
        }
    }
//==============================================================================
// Part 2
    {
        // Brute Force all values
        {
            unsigned int minDistance = 0xFFFFFFFF;
            int runningDistance = 0;
            int minPos = 0;
            int minVal = *(std::min_element(crabPositions.begin(), crabPositions.end()));
            int maxVal = *(std::max_element(crabPositions.begin(), crabPositions.end()));
            for (int i=minVal*-1; i<=maxVal; ++i)
            {
                for (auto& pos : crabPositions)
                {
                    if (i > pos)
                    {
                        int distance = i - pos;
                        for (unsigned int j=1;j<=distance;++j)
                            runningDistance += j;
                    }
                    else
                    {
                        int distance = pos - i;
                        for (unsigned int j=1;j<=distance;++j)
                            runningDistance += j;
                    }
                }
                //std::cout << std::to_string(i) << " " << std::to_string(runningDistance) << "\n";
                if (runningDistance < minDistance)
                {
                    minDistance = runningDistance;
                    minPos = i;
                }
                runningDistance = 0;
            }
            std::cout << "Min position -> " << std::to_string(minPos) << " Min distance -> " << std::to_string(minDistance) << "\n";
        }
    }
}