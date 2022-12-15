// AoC21 Day 3: 
// P1 - Calculate gamma and epsilon rate of binary numbers
// P2 - Calculate oxygen and co2 values of binary numbers

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <bitset>

unsigned int findMostCommonValueAtBit(std::list<unsigned short>& list, unsigned int bitPos)
{
    unsigned int count = 0;
    count = std::count_if(list.begin(), list.end(), [bitPos](auto num){return ((num >> bitPos) & 0x1);});
    if (count < list.size() / 2.0)
        return 0;
    else
        return 1;
}

int main() 
{
    std::ifstream file("input.txt"); // Open the input file and store it in a vector
    std::string bitString; // 12 bit values
    unsigned short value;
    std::list<unsigned short> list;
    while (file >> bitString)
    {
        value = std::stoi(bitString, nullptr, 2);
        list.push_back(value);
    }
//==============================================================================
// Part 1
    {
        unsigned short gamma = 0;
        unsigned short epsilon = 0;
        for (int i=0;i<12;++i)
        {
            if (findMostCommonValueAtBit(list, i) == 1)
                gamma = (gamma | (1 << i));
        }
        epsilon = (~gamma) & 0x0FFF;
        std::cout << "gamma=" << std::to_string(gamma) << " epsilon=" << std::to_string(epsilon) << " Mult=" << std::to_string(gamma*epsilon) << "\n";
    }
//==============================================================================
// Part 2
    {
        std::list<unsigned short> oxyList = list;
        std::list<unsigned short> co2List = list;
        for (int i = 11; i >= 0; --i)
        {
            unsigned int oxyVal = findMostCommonValueAtBit(oxyList, i);
            unsigned int co2Val = findMostCommonValueAtBit(co2List, i);

            if (oxyList.size() > 1)
                oxyList.remove_if([oxyVal, i](auto val){return ((val >> i) & 1) != oxyVal;});
            if (co2List.size() > 1)
                co2List.remove_if([co2Val, i](auto val){return ((val >> i) & 1) == co2Val;});
            if (oxyList.size() == 1 && co2List.size() == 1)
                break;
                
        }
        unsigned short lifeSupportRating = oxyList.front();
        unsigned short co2GenRating = co2List.front();
        std::cout << "lsr=" << std::to_string(lifeSupportRating) << " cgr=" << std::to_string(co2GenRating) << " Mult=" << std::to_string(lifeSupportRating*co2GenRating) << "\n";
    }
}