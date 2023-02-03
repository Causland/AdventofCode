#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <memory>
#include <set>

struct Position
{
   Position() {};
   Position(const int32_t& row, const int32_t& col) : row(row), col(col) {};
   
   bool operator==(const Position& p) const { return this->row == p.row && this->col == p.col; };
   bool operator<(const Position& p) const { return this->row < p.row || (this->row == p.row && this->col < p.col); };

   int32_t row{0};
   int32_t col{0};
};

typedef std::set<Position> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string line;

   uint32_t row{0};
   while(std::getline(f, line))
   {
      for (auto col = 0; col < line.length(); ++col)
      {
         if (line[col] == '#')
         {
            vals.insert(Position(row, col));
         }
      }
      ++row;
   }
   f.close();
}

std::map<Position, std::vector<Position>> getProposals(const T& elves, const uint32_t& round)
{
   std::map<Position, std::vector<Position>> proposals;
   for (const auto& elfPos : elves)
   {
      // Check if any elves surround
      bool isElfNorth{false};
      bool isElfSouth{false};
      bool isElfEast{false};
      bool isElfWest{false};

      if (elves.find(Position(elfPos.row-1, elfPos.col-1)) != elves.end() ||
          elves.find(Position(elfPos.row-1, elfPos.col))   != elves.end() ||
          elves.find(Position(elfPos.row-1, elfPos.col+1)) != elves.end())
         {
            isElfNorth = true;
         }
      if (elves.find(Position(elfPos.row+1, elfPos.col-1)) != elves.end() ||
          elves.find(Position(elfPos.row+1, elfPos.col))   != elves.end() ||
          elves.find(Position(elfPos.row+1, elfPos.col+1)) != elves.end())
         {
            isElfSouth = true;
         }
      if (elves.find(Position(elfPos.row-1, elfPos.col+1)) != elves.end() ||
          elves.find(Position(elfPos.row, elfPos.col+1))   != elves.end() ||
          elves.find(Position(elfPos.row+1, elfPos.col+1)) != elves.end())
         {
            isElfEast = true;
         }
      if (elves.find(Position(elfPos.row-1, elfPos.col-1)) != elves.end() ||
          elves.find(Position(elfPos.row, elfPos.col-1))   != elves.end() ||
          elves.find(Position(elfPos.row+1, elfPos.col-1)) != elves.end())
         {
            isElfWest = true;
         }

      Position proposal = elfPos;
      if (isElfNorth || isElfSouth || isElfEast || isElfWest)
      {
         if (round % 4 == 0)
         {
            if (!isElfNorth) proposal.row -= 1;
            else if (!isElfSouth) proposal.row += 1; 
            else if (!isElfWest) proposal.col -= 1;
            else if (!isElfEast) proposal.col += 1;
         }
         else if (round % 4 == 1)
         {
            if (!isElfSouth) proposal.row += 1; 
            else if (!isElfWest) proposal.col -= 1;
            else if (!isElfEast) proposal.col += 1;
            else if (!isElfNorth) proposal.row -= 1;
         }
         else if (round % 4 == 2)
         {
            if (!isElfWest) proposal.col -= 1;
            else if (!isElfEast) proposal.col += 1;
            else if (!isElfNorth) proposal.row -= 1;
            else if (!isElfSouth) proposal.row += 1; 
         }
         else
         {
            if (!isElfEast) proposal.col += 1;
            else if (!isElfNorth) proposal.row -= 1;
            else if (!isElfSouth) proposal.row += 1; 
            else if (!isElfWest) proposal.col -= 1;
         }
      }

      auto it = proposals.find(proposal);
      if (it != proposals.end())
      {
         it->second.push_back(elfPos);
      }
      else
      {
         proposals.insert({proposal, {elfPos}});
      }
   }
   return proposals;
}

T makeMoves(const std::map<Position, std::vector<Position>>& proposals)
{
   T elves;
   for (const auto& [proposal, srcPositions] : proposals)
   {
      if (srcPositions.size() > 1)
      {
         for (const auto& pos : srcPositions)
         {
            elves.insert(pos);
         }
      }
      else
      {
         elves.insert(proposal);
      }
   }
   return elves;
}

void printGrid(const T& elves)
{
   const auto& top = std::min_element(elves.begin(), elves.end(), [](const auto& a, const auto& b){ return a.row < b.row; })->row;
   const auto& bottom = std::max_element(elves.begin(), elves.end(), [](const auto& a, const auto& b){ return a.row < b.row; })->row;
   const auto& left = std::min_element(elves.begin(), elves.end(), [](const auto& a, const auto& b){ return a.col < b.col; })->col;
   const auto& right = std::max_element(elves.begin(), elves.end(), [](const auto& a, const auto& b){ return a.col < b.col; })->col;

   for (auto row = top; row <= bottom; ++row)
   {
      for (auto col = left; col <= right; ++col)
      {
         if (elves.find(Position(row, col)) != elves.end())
         {
            std::cout << "#";
         }
         else
         {
            std::cout << ".";
         }
      }
      std::cout << "\n";
   }
   std::cout << "-------------------\n";
}

void part1(T& input)
{
   const auto numRounds = 10;
   auto elves = input;
   for(auto i=0; i<numRounds; ++i)
   {
      const auto& proposals = getProposals(elves, i);
      elves = makeMoves(proposals);
   }

   // Calculate area
   const auto& top = std::min_element(elves.begin(), elves.end(), [](const auto& a, const auto& b){ return a.row < b.row; })->row;
   const auto& bottom = std::max_element(elves.begin(), elves.end(), [](const auto& a, const auto& b){ return a.row < b.row; })->row;
   const auto& left = std::min_element(elves.begin(), elves.end(), [](const auto& a, const auto& b){ return a.col < b.col; })->col;
   const auto& right = std::max_element(elves.begin(), elves.end(), [](const auto& a, const auto& b){ return a.col < b.col; })->col;
   auto area = (bottom - top + 1) * (right - left + 1);
   auto result = area - elves.size();

   std::cout << result << "\n";
}

void part2(T& input)
{
   auto elves = input;
   auto i = 0;
   while(true)
   {
      const auto& proposals = getProposals(elves, i);
      auto nextElves = makeMoves(proposals);
      if (elves == nextElves)
      {
         elves = nextElves;
         break;
      }
      else
      {
         elves = nextElves;
      }
      ++i;
   }
   std::cout << i+1 << "\n";
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