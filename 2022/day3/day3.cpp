#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>

void parseInputFile(std::string filename, std::vector<std::string>& rucksacks)
{
   std::fstream f(filename);
   std::string rucksack;

   while(std::getline(f, rucksack))
   {
      rucksacks.push_back(rucksack);
   }
   f.close();
}

char findDuplicateItem(std::string first, std::string second)
{
   std::set<char> firstCompartmentChars;
   for (auto& c : first)
   {
      firstCompartmentChars.insert(c);
   }
   for (auto& c : second)
   {
      if (firstCompartmentChars.find(c) != firstCompartmentChars.end())
      {
         return c;
      }
   }

   std::cout << "Error case reached in findDuplicateItem()\n";
   return '0';
}

uint8_t getPriority(char item)
{
   if (item >= 'a')
   {
      return item - 96; // Shift 'a-z' down to 1
   }
   else
   {
      return item - 38; // Shift 'A-Z' down to 27
   }
}

void part1(std::string filename)
{
   std::vector<std::string> rucksacks;
   parseInputFile(filename, rucksacks);

   std::string compartment1;
   std::string compartment2;

   auto total = 0;
   for (const auto& rucksack : rucksacks)
   {
      auto mid = rucksack.length() / 2;
      compartment1 = rucksack.substr(0, mid);
      compartment2 = rucksack.substr(mid);
      auto duplicate = findDuplicateItem(compartment1, compartment2);
      total += getPriority(duplicate);
   }
   std::cout << total << "\n";
}

char findBadge(std::string first, std::string second, std::string third)
{
   std::sort(first.begin(), first.end());
   std::sort(second.begin(), second.end());
   std::sort(third.begin(), third.end());

   std::string joinTwo;
   joinTwo.resize(std::max({first.length(), second.length(), third.length()}));

   std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), joinTwo.begin());

   std::string result{"0"};
   std::set_intersection(joinTwo.begin(), joinTwo.end(), third.begin(), third.end(), result.begin());

   return result.front();
}

void part2(std::string filename)
{
   std::vector<std::string> rucksacks;
   parseInputFile(filename, rucksacks);

   auto total = 0;
   for (auto it = rucksacks.begin(); it != rucksacks.end(); it+=3)
   {
      auto badge = findBadge(*it, *(it+1), *(it+2));
      total += getPriority(badge);
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

   part1(part1File);
   part2(part2File);
   
   return 0;
}