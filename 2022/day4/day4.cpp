#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

struct Range
{
   uint32_t min;
   uint32_t max;
};

void parseInputFile(std::string filename, std::vector<std::pair<Range, Range>>& vals)
{
   std::fstream f(filename);
   std::string line;

   Range r1;
   Range r2;
   while(std::getline(f, line))
   {
      // I said fuck it here
      auto posDash = line.find("-");
      r1.min = std::stol(line.substr(0, posDash));
      line = line.substr(posDash + 1);
      auto posComma = line.find(",");
      r1.max = std::stol(line.substr(0, posComma));
      line = line.substr(posComma + 1);

      posDash = line.find("-");
      r2.min = std::stol(line.substr(0, posDash));
      line = line.substr(posDash + 1);
      r2.max = std::stol(line);

      vals.push_back({r1, r2});
   }

   f.close();
}

bool checkRange(Range r1, Range r2)
{
   if (r1.max >= r2.max)
   {
      if (r1.min <= r2.min)
      {
         return true;
      }
   }
   if (r2.max >= r1.max)
   {
      if (r2.min <= r1.min)
      {
         return true;
      }
   }
   return false;
}

void part1(std::string filename)
{
   std::vector<std::pair<Range, Range>> vals;
   parseInputFile(filename, vals);

   auto count = 0;
   for (const auto& ranges : vals)
   {
      if (checkRange(ranges.first, ranges.second))
      {
         ++count;
      }
   }

   std::cout << count << "\n";
}

bool checkOverlap(Range r1, Range r2)
{
   if (r1.max > r2.max && r1.min > r2.max) return false;
   if (r1.max < r2.min && r1.min < r2.min) return false;
   return true;
}

void part2(std::string filename)
{
   std::vector<std::pair<Range, Range>> vals;
   parseInputFile(filename, vals);

   auto count = 0;
   for (const auto& ranges : vals)
   {
      if (checkOverlap(ranges.first, ranges.second))
      {
         ++count;
      }
   }

   std::cout << count << "\n";
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

   part1(part1File);
   part2(part2File);
   
   return 0;
}