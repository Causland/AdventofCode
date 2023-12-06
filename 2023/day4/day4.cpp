#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <sstream>
#include <set>

struct Card
{
   const uint32_t numMatching;
   const uint32_t points;
};

typedef std::vector<Card> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string strVal;

   while(std::getline(f, strVal))
   {
      auto colonPos{strVal.find(':')};
      auto pipePos{strVal.find('|')};
      auto winNumsStr{strVal.substr(colonPos+1, pipePos-colonPos-2)};
      std::stringstream winNumsStream{winNumsStr};
      std::string winNum;
      std::vector<uint32_t> winNums;
      while(std::getline(winNumsStream, winNum, ' ')) 
      {
         if (winNum.empty()) continue;
         winNums.push_back(std::stoul(winNum));
         winNum.clear();
      }

      auto myNumsStr{strVal.substr(pipePos+1)};
      std::stringstream myNumsStream{myNumsStr};
      std::string myNum;
      std::vector<uint32_t> myNums;
      while(std::getline(myNumsStream, myNum, ' ')) 
      {
         if (myNum.empty()) continue;
         myNums.push_back(std::stoul(myNum));
         myNum.clear();
      }

      std::sort(winNums.begin(), winNums.end());
      std::sort(myNums.begin(), myNums.end());

      std::vector<uint32_t> both;
      std::set_intersection(winNums.begin(), winNums.end(), 
                            myNums.begin(), myNums.end(),
                            std::back_inserter(both));

      Card c{static_cast<uint32_t>(both.size()), 
             static_cast<uint32_t>(both.size() >= 1 ? (1 << (both.size()-1)) : 0)};
      vals.push_back(c);
   }

   f.close();
}

void part1(T input)
{
   std::cout << std::accumulate(input.begin(), input.end(), 0, 
                     [](const int& a, const Card& b){ return a + b.points; }) << "\n";
}

void part2(T input)
{
   uint64_t total{0};
   std::vector<uint32_t> cardCount(input.size(), 1);

   bool found{true};
   while(found)
   {
      found = false;
      for (auto i=0; i<cardCount.size(); ++i)
      {
         if (cardCount[i] > 0)
         {
            found = true;
            total += cardCount[i];
            for (auto j=1; j<=input[i].numMatching; ++j)
            {
               cardCount[i+j] += cardCount[i];
            }
            cardCount[i] = 0;
         }
      }
   }

   std::cout << total << "\n";
}

int main(int argc, char** argv)
{
   if (argc < 3)
   {
      std::cout << "Enter 2 file options (0 for test input or 1 real input)\n";
      return 1;
   }

   std::string part1Option(argv[1]);
   std::string part1File = "";
   if (part1Option == "0")
   {
      part1File = "test.txt";
   }
   else
   {
      part1File = "input.txt";
   }

   std::string part2Option(argv[2]);
   std::string part2File = "";
   if (part2Option == "0")
   {
      part2File = "test.txt";
   }
   else
   {
      part2File = "input.txt";
   }

   T vals;
   parseInputFile(part1File, vals);
   part1(vals);

   vals.clear();

   parseInputFile(part2File, vals);
   part2(vals);
   
   return 0;
}