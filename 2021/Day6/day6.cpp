// AoC21 Day 6:
// Population of multiplying fish

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <list>
#include <algorithm>

int main()
{
    std::ifstream file("input.txt"); // Open the input file and store it in a list
    std::string s;
    file >> s;
    std::stringstream s_stream(s); //create string stream from the string
    std::list<unsigned char> fish;
    while(s_stream.good()) {
        std::string substr;
        unsigned char val;
        getline(s_stream, substr, ','); //get first string delimited by comma
        val = std::stoi(substr);
        fish.push_back(val);
    }

//==============================================================================
// Part 1
    {
        std::list<unsigned char> fish_p1(fish);
        for (unsigned int i=0; i<80; ++i) // Simulate 80 days
        {
            unsigned int newFishCount = 0;
            for (auto& daysRemain : fish_p1)
            {
                if (daysRemain == 0)
                {
                    daysRemain = 6;
                    ++newFishCount;
                }
                else
                    --daysRemain;
            }
            for (unsigned int j=0; j<newFishCount; ++j) // Add the new fish at the end of each day
                fish_p1.push_back(8);
        }
        std::cout << "Number of fish after 80 days -> " << std::to_string(fish_p1.size()) << "\n";
    }
//==============================================================================
// Part 2
    {
        std::array<unsigned long long, 9> fishPopulation{};
        for (auto& daysRemain : fish)
            fishPopulation[daysRemain] += 1;

        for (unsigned int i=0; i<256; ++i) // Simulate 256 days
        {
            unsigned long long newFishCount = 0;
            for (int j=0; j<9; ++j)
            {
                if (j == 0)
                {
                    newFishCount = fishPopulation[0];
                }
                else
                {
                    fishPopulation[j-1] = fishPopulation[j];
                }
            }
            fishPopulation[8] = newFishCount;
            fishPopulation[6] += newFishCount;
        }

        unsigned long long totalFish = 0;
        for (auto& fishCount : fishPopulation)
            totalFish += fishCount;
        std::cout << "Number of fish after 256 days -> " << std::to_string(totalFish) << "\n";
    }
}
