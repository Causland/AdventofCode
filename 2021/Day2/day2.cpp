// AoC21 Day 2: 
// P1 - Simple find where the submarine is going
// P2 - Use aim to find where sub is going

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main() 
{
    std::ifstream file("input.txt"); // Open the input file and store it in a vector
    std::string command;
    unsigned int value;
    std::vector<std::pair<std::string, unsigned int>> vec;
    while (file >> command >> value)
    {
        vec.emplace_back(command, value);
    }
//==============================================================================
// Part 1
    {
        unsigned int depth = 0;
        unsigned int pos = 0;
        for (auto& [command, value] : vec)
        {
            if (command == "forward")
            {
                pos += value;
                continue;
            }
            if (command == "down")
            {
                depth += value;
                continue;
            }
            if (command == "up")
            {
                depth -= value;
                continue;
            }
        }
        std::cout << "Depth=" << std::to_string(depth) << " Pos=" << std::to_string(pos) << " Mult=" << std::to_string(depth*pos) << "\n";
    }
//==============================================================================
// Part 2
    {
        unsigned int depth = 0;
        unsigned int pos = 0;
        int aim = 0;
        for (auto& [command, value] : vec)
        {
            if (command == "forward")
            {
                pos += value;
                depth += aim*value;
                continue;
            }
            if (command == "down")
            {
                aim += value;
                continue;
            }
            if (command == "up")
            {
                aim -= value;
                continue;
            }
        }
        std::cout << "Depth=" << std::to_string(depth) << " Pos=" << std::to_string(pos) << " Mult=" << std::to_string(depth*pos) << "\n";
    }
}
        
