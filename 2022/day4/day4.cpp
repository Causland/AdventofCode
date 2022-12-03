#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

void parseInputFile(std::string filename, std::vector<uint32_t>& vals)
{
   std::fstream f(filename);
   std::string strVal;

   while(std::getline(f, strVal))
   {
      vals.push_back(std::stol(strVal));
      strVal.clear();
   }
   f.close();
}

void part1(std::string filename)
{
}

void part2(std::string filename)
{
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