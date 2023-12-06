#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

typedef std::vector<std::string> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string strVal;

   while(std::getline(f, strVal))
   {
      vals.push_back(strVal);
      strVal.clear();
   }
   f.close();
}

bool isNumber(const char& c) { return (c >= 49) && (c <= 57); }

void part1(T input)
{
   std::vector<uint32_t> nums;

   for (const auto& line : input)
   {
      std::string strNum;
      for (auto it=line.begin(); it !=line.end(); ++it)
      {
         if (isNumber(*it)) 
         {
            strNum.push_back(*it);
            break;
         }
      }

      for (auto rit=line.rbegin(); rit !=line.rend(); ++rit)
      {
         if (isNumber(*rit)) 
         {
            strNum.push_back(*rit);
            break;
         }
      }

      nums.push_back(std::stoul(strNum));      
   }

   std::cout << std::accumulate(nums.begin(), nums.end(), 0) << "\n";
}

char wordCheck(const std::string& word)
{
   if (word.size() < 3) return '0';

   if (word[0] == 'o')
   {
      if (word.substr(0, 3) == "one") return '1';
   }
   else if (word[0] == 't')
   {
      if (word.substr(0, 3) == "two") return '2';
      if (word.size() < 5) return '0';
      if (word == "three") return '3';
   }
   else if (word[0] == 'f')
   {
      if (word.size() < 4) return '0';
      if (word.substr(0, 4) == "four") return '4';
      if (word.substr(0, 4) == "five") return '5';
   }
   else if (word[0] == 's')
   {
      if (word.substr(0, 3) == "six") return '6';
      if (word == "seven") return '7';
   }
   else if (word[0] == 'e')
   {
      if (word == "eight") return '8';
   }
   else if (word[0] == 'n')
   {
      if (word.substr(0, 4) == "nine") return '9';
   }

   return '0';
}

void part2(T input)
{
   std::vector<uint32_t> nums;

   for (const auto& line : input)
   {
      std::string strNum;
      for (auto i=0; i<line.size(); ++i)
      {
         if (isNumber(line[i])) 
         {
            strNum.push_back(line[i]);
            break;
         }

         const char c{wordCheck(line.substr(i, std::min(static_cast<size_t>(5), line.size() - i)))};
         if (c != '0')
         {
            strNum.push_back(c);
            break;
         }
      }

      for (auto i=line.size()-1; i >=0; --i)
      {
         if (isNumber(line[i])) 
         {
            strNum.push_back(line[i]);
            break;
         }
         
         const char c{wordCheck(line.substr(i, std::min(static_cast<size_t>(5), line.size() - i)))};
         if (c != '0')
         {
            strNum.push_back(c);
            break;
         }
      }

      nums.push_back(std::stoul(strNum));      
   }

   std::cout << std::accumulate(nums.begin(), nums.end(), 0) << "\n";
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
      part1File = "test1.txt";
   }
   else
   {
      part1File = "input.txt";
   }

   std::string part2Option(argv[2]);
   std::string part2File = "";
   if (part2Option == "0")
   {
      part2File = "test2.txt";
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