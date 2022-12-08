#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <array>

typedef std::string T;

void parseInputFile(std::string filename, T& val)
{
   std::fstream f(filename);
   f >> val;
   std::transform(val.begin(), val.end(), val.begin(), [](auto c){ return c - 'a'; });
   f.close();
}

void part1(T input)
{
   std::array<uint8_t, 26> mapping{0};
   auto result = 0;
   for (auto i = 0; i < input.length() - 4; ++i)
   {
      if (i >= 4) --mapping[input[i-4]];
      ++mapping[input[i]];

      if (i >= 3 && std::count_if(mapping.begin(), mapping.end(), [](auto& count){ return count > 1; }) == 0)
      {
         result = i + 1;
         break;
      }
   }

   std::cout << result << "\n";
}

void part2(T input)
{
   std::array<uint8_t, 26> mapping{0};
   auto result = 0;
   for (auto i = 0; i < input.length() - 14; ++i)
   {
      if (i >= 14) --mapping[input[i-14]];
      ++mapping[input[i]];

      if (i >= 13 && std::count_if(mapping.begin(), mapping.end(), [](auto& count){ return count > 1; }) == 0)
      {
         result = i + 1;
         break;
      }
   }

   std::cout << result << "\n";
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