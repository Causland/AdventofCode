#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

void parseInputFile(std::string filename, std::vector<std::pair<char, char>>& rounds)
{
   std::fstream f(filename);
   char theirs;
   char ours;

   while(f >> theirs >> ours)
   {
      rounds.push_back({theirs, ours});
   }
   f.close();
}

enum class Outcome
{
   LOSS = 0,
   DRAW = 3,
   WIN  = 6,
};

Outcome checkOutcome(char theirs, char ours)
{
   // Shift ours down to theirs
   ours -= 23; // X-->A
   if (theirs == ours)
   {
      return Outcome::DRAW;
   }
   else if(theirs == 'A')
   {
      if (ours == 'B') return Outcome::WIN;
      if (ours == 'C') return Outcome::LOSS;
   }
   else if (theirs == 'B')
   {
      if (ours == 'A') return Outcome::LOSS;
      if (ours == 'C') return Outcome::WIN;
   }
   else // theirs == 'C'
   {
      if (ours == 'A') return Outcome::WIN;
      if (ours == 'B') return Outcome::LOSS;
   }
   std::cout << "Reached impossible outcome\n";
   return Outcome::DRAW;
}

void part1(std::string filename)
{
   std::vector<std::pair<char, char>> rounds;
   parseInputFile(filename, rounds);

   auto totalScore = 0;
   for (const auto& round : rounds)
   {
      auto outcome = checkOutcome(round.first, round.second);
      totalScore += static_cast<uint32_t>(outcome) + round.second - 87; // shift down to 1,2,3
   }

   std::cout << totalScore << "\n";
}

char findOption(char theirs, char outcome)
{
   switch(outcome)
   {
      case 'Y':
      {
         return theirs;
      }
      case 'X':
      {
         if (theirs == 'A') return 'C';
         if (theirs == 'B') return 'A';
         if (theirs == 'C') return 'B';
      }
      case 'Z':
      {
         if (theirs == 'A') return 'B';
         if (theirs == 'B') return 'C';
         if (theirs == 'C') return 'A';
      }
   }
   std::cout << "Reached impossible option\n";
   return 'A';
}

void part2(std::string filename)
{
   std::vector<std::pair<char, char>> rounds;
   parseInputFile(filename, rounds);

   auto totalScore = 0;
   for (const auto& round : rounds)
   {
      auto option = findOption(round.first, round.second);
      totalScore += (round.second - 88) * 3 + option - 64;
   }

   std::cout << totalScore << "\n";
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