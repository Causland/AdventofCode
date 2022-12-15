#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

enum class ElementType
{
   LIST,
   INT,
};

struct Element
{
   ElementType type;
   union value
   {
      std::vector<Element> l;
      uint32_t i;
   };
};

typedef std::vector<uint32_t> T;

void parseInputFile(std::string filename, T& vals)
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

void part1(T input)
{
}

void part2(T input)
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

   T vals;
   parseInputFile(part1File, vals);
   part1(vals);

   vals.clear();

   parseInputFile(part2File, vals);
   part2(vals);
   
   return 0;
}