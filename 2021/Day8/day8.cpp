// AoC21 Day 7:
// P1 - How many times do digits 1, 4, 7, or 8 appear?
// P2 - 

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include "Seven_Seg_Disp.h"

int main()
{
    std::ifstream file("testinput.txt"); // Open the input file and store it in a list
    std::string s;
    std::vector<std::pair<std::vector<Seven_Seg_Disp>, std::vector<Seven_Seg_Disp>>> inputs;
    while (file.good())
    {
        std::vector<Seven_Seg_Disp> codes;
        std::vector<Seven_Seg_Disp> outputs;
        for (unsigned int i=0; i<10; ++i)
        {
            file >> s;
            codes.emplace_back(s);
        }
        file >> s; // Get rid of | character
        for (unsigned int i=0;i<4; ++i)
        {
            file >> s;
            outputs.emplace_back(s);
        }
        std::pair<std::vector<Seven_Seg_Disp>, std::vector<Seven_Seg_Disp>> pair = {codes, outputs};
        inputs.push_back(pair);
    }

//==============================================================================
// Part 1
    {
        unsigned int count = 0;
        for (auto& [codes, outputs] : inputs)
        {
            count += std::count_if(outputs.begin(), outputs.end(), 
                            [](auto& disp){return disp.size == 2 || disp.size == 3 || disp.size == 4 || disp.size == 7;});
        }
        std::cout << "Count -> " << std::to_string(count) << "\n"; 
    }
//==============================================================================
// Part 2
    {   
        for (auto& [codes, outputs] : inputs)
        {
            std::map<char, std::string> possible_connections({
                                                        {'a', "u"}, 
                                                        {'b', "u"},
                                                        {'c', "u"},
                                                        {'d', "u"},
                                                        {'e', "u"},
                                                        {'f', "u"},
                                                        {'g', "u"}}); 
            // Find segments for 1, 4, 7
            uint8_t code_for_one;
            uint8_t code_for_four;
            uint8_t code_for_seven;
            for (auto& code : codes)
            {
                if (code.size == 1)
                    code_for_one = code.segments;
                if (code.size == 4)
                    code_for_four = code.segments;
                if (code.size == 3)
                    code_for_seven = code.segments;
            }
            for (auto& output : outputs)
            {
                if (output.size == 1)
                    code_for_one = output.segments;
                if (output.size == 4)
                    code_for_four = output.segments;
                if (output.size == 3)
                    code_for_seven = output.segments;
            }
            // Determine top segment if possible
            if (code_for_one && code_for_seven)
                possible_connections['a'] = Seven_Seg_Disp::codeToString(code_for_one ^ code_for_seven);
            // Determine segments for 1 and 4
            if (code_for_one)
            {
                possible_connections['c'] = Seven_Seg_Disp::codeToString(code_for_one);
                possible_connections['f'] = Seven_Seg_Disp::codeToString(code_for_one);
                if (code_for_four)
                    possible_connections['b'] = Seven_Seg_Disp::codeToString(code_for_one ^ code_for_four);
                    possible_connections['d'] = Seven_Seg_Disp::codeToString(code_for_one ^ code_for_four);
            }
            
            // Find codes that have one unknown segment and discover what the segment is
            for (auto& code : codes)
            {
                unsigned char unknownCount = 0;
                for (unsigned int i=0;i<code.size;++i)
                {
                    if (possible_connections[code.segmentStr[i]] == "u")
                        unknownCount += 1;
                    else if (possible_connections[code.segmentStr[i]].size() > 1)
                    {
                        
                    }
                }
            }
        }
    }
}