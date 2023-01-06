#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <iomanip>

typedef std::vector<int64_t> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string line;

   while(std::getline(f, line))
   {
      vals.push_back(std::stoll(line));
   }
   f.close();
}

void mixNumbers(const T& input, std::vector<uint32_t>& resultOrder, const int32_t& encryptionKey)
{
   for (auto i = 0; i < input.size(); ++i)
   {
      // Find the starting index (Assume that the value is found)
      int32_t index = std::find(resultOrder.begin(), resultOrder.end(), i) - resultOrder.begin();
      int64_t shifts = input[i] * encryptionKey;
      if (shifts == 0) continue; // No need to do anything here

      // Remove the current index (The list is reduced by 1)
      resultOrder.erase(resultOrder.begin() + index);

      // Shift right
      if (shifts > 0)
      {
         index += (shifts % resultOrder.size());
         if (index >= resultOrder.size())
         {
            index = index - resultOrder.size();
         }
      }
      // Shift left
      else
      {
         index -= (static_cast<uint64_t>(-1 * shifts) % resultOrder.size());
         if (index <= 0)
         {
            index = resultOrder.size() + index;
         }
      }

      // Reinsert at the new location
      resultOrder.insert(resultOrder.begin() + index, i);
   }
}

uint32_t getZeroIndex(const T& input, const std::vector<uint32_t>& resultOrder)
{
   for (auto i = 0; i < resultOrder.size(); ++i)
   {
      if (input[resultOrder[i]] == 0)
      {
         return i;
      }
   }
   std::cout << "0 not found\n";
   return 0;
}

int64_t calculateGroveCoordSum(const T& input, const std::vector<uint32_t>& resultOrder, const uint32_t& zeroIndex, const int32_t& encryptionKey)
{
   int64_t sum{0};
   sum += input[resultOrder[(zeroIndex + 1000) % resultOrder.size()]];
   sum += input[resultOrder[(zeroIndex + 2000) % resultOrder.size()]];
   sum += input[resultOrder[(zeroIndex + 3000) % resultOrder.size()]];
   sum *= encryptionKey;
   return sum;
}

void part1(const T& input)
{
   const int32_t encryptionKey{1};
   std::vector<uint32_t> resultOrder;
   for (auto i = 0; i < input.size(); ++i) resultOrder.push_back(i);
   mixNumbers(input, resultOrder, encryptionKey);

   uint32_t zeroIndex = getZeroIndex(input, resultOrder);
   std::cout << calculateGroveCoordSum(input, resultOrder, zeroIndex, encryptionKey) << "\n";
}

void part2(T input)
{
   const int32_t encryptionKey{811589153};
   std::vector<uint32_t> resultOrder;
   for (auto i = 0; i < input.size(); ++i) resultOrder.push_back(i);

   for (auto i = 0; i < 10; ++i)
   {
      mixNumbers(input, resultOrder, encryptionKey);
   }

   uint32_t zeroIndex = getZeroIndex(input, resultOrder);
   std::cout << calculateGroveCoordSum(input, resultOrder, zeroIndex, encryptionKey) << "\n";
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