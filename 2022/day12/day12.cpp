#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>

typedef std::vector<std::vector<char>> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string line;

   while(std::getline(f, line))
   {
      vals.emplace_back();
      for(auto c : line)
      {
         vals.back().push_back(c);
      }
   }
   f.close();
}

void findStartAndEnd(T& map, uint32_t& startRow, uint32_t& startCol, uint32_t& endRow, uint32_t& endCol)
{
   bool foundStart = false;
   bool foundEnd = false;

   for (auto i = 0; i < map.size(); ++i)
   {
      for (auto j = 0; j < map[0].size(); ++j)
      {
         if (map[i][j] == 'S')
         {
            startRow = i;
            startCol = j;
            map[i][j] = 'a';
            foundStart = true;
         }
         else if(map[i][j] == 'E')
         {
            endRow = i;
            endCol = j;
            map[i][j] = 'z';
            foundEnd = true;
         }

         if (foundStart && foundEnd)
         {
            return;
         }
      }
   }
}

void printVisitedMap(const T& map, const std::set<std::pair<uint32_t, uint32_t>>& visited)
{
   for (auto i=0; i<map.size(); ++i)
   {
      for(auto j=0; j<map[0].size(); ++j)
      {
         if (visited.find({i, j}) != visited.end())
         {
            std::cout << "X";
         }
         else
         {
            std::cout << ".";
         }
      }
      std::cout << "\n";
   }
   std::cout << "\n";
}

uint32_t findShortestPath(const T& map, uint32_t row, uint32_t col, 
                           const uint32_t& targetRow, const uint32_t& targetCol, 
                              std::set<std::pair<uint32_t, uint32_t>>& visited)
{
   if (row == targetRow && col == targetCol)
   {
      return 1;
   }

   visited.insert({row, col});

   // Find min path out of up/down/left/right
   uint32_t upDist = 0xFFFF;
   uint32_t downDist = 0xFFFF;
   uint32_t leftDist = 0xFFFF;
   uint32_t rightDist = 0xFFFF;

   if (row != 0 && visited.find({row-1, col}) == visited.end() && map[row-1][col] <= map[row][col] + 1)
   {
      upDist = findShortestPath(map, row-1, col, targetRow, targetCol, visited);
   }
   if (row != map.size() - 1 && visited.find({row+1, col}) == visited.end() && map[row+1][col] <= map[row][col] + 1)
   {
      downDist = findShortestPath(map, row+1, col, targetRow, targetCol, visited);
   }
   if (col != 0 && visited.find({row, col-1}) == visited.end() && map[row][col-1] <= map[row][col] + 1)
   {
      leftDist = findShortestPath(map, row, col-1, targetRow, targetCol, visited);
   }
   if(col != map[0].size() - 1 && visited.find({row, col+1}) == visited.end() && map[row][col+1] <= map[row][col] + 1)
   {
      rightDist = findShortestPath(map, row, col+1, targetRow, targetCol, visited);
   }

   visited.erase({row, col});
   return std::min({upDist, downDist, leftDist, rightDist}) + 1;
}

void part1(T input)
{
   uint32_t startRow{0};
   uint32_t startCol{0}; 
   uint32_t endRow{0}; 
   uint32_t endCol{0};
   findStartAndEnd(input, startRow, startCol, endRow, endCol);

   std::set<std::pair<uint32_t, uint32_t>> theSet;

   uint32_t result = findShortestPath(input, startRow, startCol, endRow, endCol, theSet);
   std::cout << result << "\n";
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