// AoC21 Day 1:
// P1 - Count number of times where values increase
// P2 - Count number of times where sliding window of values increase

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main() 
{
    std::ifstream file("input.txt"); // Open the input file and store it in a vector
    unsigned int value;
    std::vector<unsigned int> vec;
    while (file >> value)
        vec.push_back(value);

//==============================================================================
// Part 1
    {
        unsigned int count = 0;
        unsigned int prevVal = 0xFFFFFFFF;
        for (auto& i : vec)
        {
            if (i > prevVal)
                ++count;
            prevVal = i;
        }
        std::cout<<std::to_string(count)<<"\n";
    }

//===============================================================================
// Part 2
    {
        unsigned int count = 0;
        unsigned int prevWindowVal = vec[0] + vec[1] + vec[2];
        unsigned int currWindowVal = vec[0] + vec[1] + vec[2];
        for (unsigned int i=3;i<vec.size();++i)
        {
            currWindowVal = currWindowVal - vec[i-3] + vec[i];
            if (currWindowVal > prevWindowVal)
                ++count;
            prevWindowVal = currWindowVal;
        }
        std::cout<<std::to_string(count)<<"\n";
    }
}

