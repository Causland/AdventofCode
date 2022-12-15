#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

typedef std::vector<int32_t> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string line;

   while(std::getline(f, line))
   {
      auto posSpace = line.find(' ');
      auto instruction = line.substr(0, posSpace);

      if (instruction == "noop")
      {
         vals.push_back(0);
      }
      else if (instruction == "addx")
      {
         auto valStr = line.substr(posSpace + 1);
         vals.push_back(std::stoi(valStr));
      }
   }
   f.close();
}

void part1(T input)
{
   std::vector<int32_t> signalStrengths;
   auto regVal = 1;
   auto count = 1;
   bool delayed = false;
   for (auto i = 0; i < input.size();)
   {
      if (count == 20 || count == 60 || count == 100 || count == 140 || count == 180 || count == 220)
      {
         signalStrengths.push_back(regVal * count);
      }

      if (input[i] == 0)
      {
         delayed = false;
         ++i;
      }
      else if (!delayed)
      {
         delayed = true;
      }
      else
      {
         regVal += input[i];
         ++i;
         delayed = false;
      }
      ++count;
   }

   auto sum = std::accumulate(signalStrengths.begin(), signalStrengths.end(), 0);
   std::cout << sum << "\n";
}

void part2(T input)
{
   char* crt = new char[6 * 40];
   auto regVal = 1;
   uint32_t count = 0;
   bool delayed = false;
   for (auto i = 0; i < input.size();)
   {
      if (regVal - 1 == count % 40 || regVal == count % 40 || regVal + 1 == count % 40)
      {
         crt[count] = '#';
      }
      else
      {
         crt[count] = '.';
      }

      if (input[i] == 0)
      {
         delayed = false;
         ++i;
      }
      else if (!delayed)
      {
         delayed = true;
      }
      else
      {
         regVal += input[i];
         ++i;
         delayed = false;
      }
      ++count;
   }

   for (auto i = 0; i < 6; ++i)
   {
      for (auto j = 0; j < 40; ++j)
      {
         std::cout << crt[i * 40 + j];
      }
      std::cout << "\n";
   }

   delete[] crt;
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