#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

typedef std::vector<std::string> T;

const std::map<int32_t, char> decToSna{
   {-2, '='},
   {-1, '-'},
   {0,  '0'},
   {1,  '1'},
   {2,  '2'}
};

const std::map<char, int32_t> snaToDec{
   {'=', -2},
   {'-', -1},
   {'0',  0},
   {'1',  1},
   {'2',  2}
};

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string line;

   while(std::getline(f, line))
   {
      vals.push_back(line);
   }
   f.close();
}

char performCarry(int32_t& num)
{
   char carry{'0'};
   if (num > 2)
   {
      carry = '1';
      if (num == 3) num = -2;
      else if (num == 4) num = -1;
      else if (num == 5) num = 0;
   }
   else if (num < -2)
   {
      carry = '-';
      if (num == -3) num = 2;
      else if (num == -4) num = 1;
      else if (num == -5) num = 0;
   }
   return carry;
}

std::string addSnafu(const std::string x, const std::string y)
{
   std::string result;

   char carry{'0'};
   int32_t sum{0};
   int32_t xval{0};
   int32_t yval{0};
   int32_t cval{0};
   auto xit = x.rbegin(), yit = y.rbegin();
   for (; xit != x.rend() && yit != y.rend(); ++xit, ++yit)
   {
      xval = snaToDec.at(*xit);
      yval = snaToDec.at(*yit);
      cval = snaToDec.at(carry);

      sum = xval + yval + cval;
      carry = performCarry(sum); // Reduces sum

      char c = decToSna.at(sum);
      result.insert(result.begin(), c);
   }

   // Check for any remaining digits
   for (; xit != x.rend(); ++xit)
   {
      xval = snaToDec.at(*xit);
      cval = snaToDec.at(carry);

      sum = xval + cval;
      carry = performCarry(sum); // Reduces sum
      
      char c = decToSna.at(sum);
      result.insert(result.begin(), c);
   }

   for (; yit != y.rend(); ++yit)
   {
      yval = snaToDec.at(*yit);
      cval = snaToDec.at(carry);

      sum = yval + cval;
      carry = performCarry(sum); // Reduces sum
      
      char c = decToSna.at(sum);
      result.insert(result.begin(), c);
   }
   
   if (carry != '0') result.insert(result.begin(), carry);
   return result;
}

void part1(T input)
{
   std::string total;
   for (const auto& snafu : input)
   {
      total = addSnafu(total, snafu);
   }

   std::cout << total << "\n";
}

void part2(T input)
{
   // Star just given after part1
   std::cout<<addSnafu("1-2=-=", "21")<<"\n";
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