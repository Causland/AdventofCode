#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>

void parseInputFile(std::string filename, std::vector<std::vector<uint32_t>>& vals)
{
   std::fstream f(filename);
   std::string strVal;

   vals.emplace_back();
   while(std::getline(f, strVal))
   {
      if (strVal.empty())
      {
         vals.emplace_back();
         continue;
      }
      vals.back().push_back(std::stol(strVal));
      strVal.clear();
   }
   f.close();
}


void part1(std::string filename)
{
   std::vector<std::vector<uint32_t>> vals;
   parseInputFile(filename, vals);

   uint32_t maxVal = 0;
   uint32_t sum = 0;
   for (auto& list : vals)
   {
      for (auto& val : list)
      {
         sum += val;
      }
      if (sum > maxVal)
      {
         maxVal = sum;
      }
      sum = 0;
   }

   std::cout << maxVal << "\n";
}

void part2(std::string filename)
{
   std::vector<std::vector<uint32_t>> vals;
   parseInputFile(filename, vals);

   std::vector<uint32_t> theHeap;

   uint32_t sum = 0;
   for (auto& list : vals)
   {
      for (auto& val : list)
      {
         sum += val;
      }
      theHeap.push_back(sum);
      std::push_heap(theHeap.begin(), theHeap.end());
      sum = 0;
   }

   uint32_t result = 0;
   for (auto i=0; i<3; ++i)
   {
      std::pop_heap(theHeap.begin(), theHeap.end());
      result += theHeap.back();
      theHeap.pop_back();
   }

   std::cout << result << "\n";
}

int main()
{
   part1("input.txt");
   part2("input.txt");
}