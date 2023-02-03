#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

typedef std::vector<std::vector<std::vector<bool>>> T;

enum class Position
{
   UP,
   DOWN,
   LEFT,
   RIGHT,
   WALL,
};

void printGrid(const std::vector<std::vector<std::vector<Position>>>& grid)
{
   for (auto row : grid)
   {
      for (auto pos : row)
      {
         if (pos.size() > 1)                     std::cout << pos.size();
         else if (pos.empty())                   std::cout << ".";
         else if (pos.back() == Position::WALL)  std::cout << "#";
         else if (pos.back() == Position::UP)    std::cout << "^";
         else if (pos.back() == Position::DOWN)  std::cout << "v";
         else if (pos.back() == Position::LEFT)  std::cout << "<";
         else if (pos.back() == Position::RIGHT) std::cout << ">";
      }
      std::cout << "\n";
   }
   std::cout << "\n";
}

void printGridOccupied(const std::vector<std::vector<bool>>& grid)
{
   for (auto row : grid)
   {
      for (auto pos : row)
      {
         if (pos) std::cout << "#";
         else std::cout << ".";
      }
      std::cout << "\n";
   }
   std::cout << "\n";
}

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string line;

   std::vector<std::vector<std::vector<Position>>> grid;
   while(std::getline(f, line))
   {
      grid.emplace_back();
      for (auto& c : line)
      {
         grid.back().emplace_back();
         switch(c)
         {
            case '#':
               grid.back().back().push_back(Position::WALL);
               break;
            case '.':
               break;
            case '^':
               grid.back().back().push_back(Position::UP);
               break;
            case 'v':
               grid.back().back().push_back(Position::DOWN);
               break;
            case '<':
               grid.back().back().push_back(Position::LEFT);
               break;
            case '>':
               grid.back().back().push_back(Position::RIGHT);
               break;
            default:
               std::cout << "We shouldn't get here\n";
         }
      }
   }
   f.close();

   // Make an LCM(row, col) number of maps to cycle through during navigation
   const auto lcm = std::lcm(grid.size()-2, grid[0].size()-2);

   std::vector<std::vector<bool>> frame{grid.size(), std::vector<bool>(grid[0].size(), false)};
   for (auto i=0; i<lcm; ++i)
   {  
      // Build out the frame
      for (auto row=0; row < grid.size(); ++row)
      {
         for (auto col=0; col < grid[0].size(); ++col)
         {
            frame[row][col] = !grid[row][col].empty();
         }
      }

      // Add frame to the vector of frames for navigation
      vals.push_back(frame);
      // Reset the frame vector
      frame = std::vector<std::vector<bool>>(grid.size(), std::vector<bool>(grid[0].size(), false));

      // Move the storms on the grid by making a new one
      std::vector<std::vector<std::vector<Position>>> newGrid{grid.size(), std::vector<std::vector<Position>>(grid[0].size())};
      for (auto row=0; row < grid.size(); ++row)
      {
         for (auto col=0; col < grid[0].size(); ++col)
         {
            for (auto pos : grid[row][col])
            {
               if (pos == Position::WALL)
               {
                  newGrid[row][col].push_back(Position::WALL);
               }
               else if(pos == Position::UP)
               {
                  if (!grid[row-1][col].empty() && grid[row-1][col].back() == Position::WALL)
                  {
                     newGrid[newGrid.size()-2][col].push_back(Position::UP);
                  }
                  else
                  {
                     newGrid[row-1][col].push_back(Position::UP);
                  }
               }
               else if(pos == Position::DOWN)
               {
                  if (!grid[row+1][col].empty() && grid[row+1][col].back() == Position::WALL)
                  {
                     newGrid[1][col].push_back(Position::DOWN);
                  }
                  else
                  {
                     newGrid[row+1][col].push_back(Position::DOWN);
                  }
               }
               else if(pos == Position::LEFT)
               {
                  if (!grid[row][col-1].empty() && grid[row][col-1].back() == Position::WALL)
                  {
                     newGrid[row][newGrid[0].size()-2].push_back(Position::LEFT);
                  }
                  else
                  {
                     newGrid[row][col-1].push_back(Position::LEFT);
                  }
               }
               else if(pos == Position::RIGHT)
               {
                  if (!grid[row][col+1].empty() && grid[row][col+1].back() == Position::WALL)
                  {
                     newGrid[row][1].push_back(Position::RIGHT);
                  }
                  else
                  {
                     newGrid[row][col+1].push_back(Position::RIGHT);
                  }
               }
            }
         }
      }
      grid = newGrid;
   }
}

uint32_t findFastestPath(const T& grids, uint32_t row, uint32_t col, uint32_t minute, uint32_t& fastest, 
                         const uint32_t& endRow, const uint32_t& endCol)
{
   // Select the grid to use
   const std::vector<std::vector<bool>>& grid = grids[minute%grids.size()];

   // Check termination conditions
   if (row == endRow && col == endCol) return minute;
   if (grid[row][col]) return UINT32_MAX;

   // Continue to move through paths and find min of all of them
   uint32_t min = UINT32_MAX;
   auto val = findFastestPath(grids, row+1, col, minute+1, fastest, endRow, endCol);
   if (val < min) min = val;
   val = findFastestPath(grids, row, col+1, minute+1, fastest, endRow, endCol);
   if (val < min) min = val;
   if (row > 0)
   {
      val = findFastestPath(grids, row-1, col, minute+1, fastest, endRow, endCol);
      if (val < min) min = val;
   }
   val = findFastestPath(grids, row, col-1, minute+1, fastest, endRow, endCol);
   if (val < min) min = val;
   val = findFastestPath(grids, row, col, minute+1, fastest, endRow, endCol);
   if (val < min) min = val;

   // If the local min is faster than the fastest of any branches thus far, set fastest
   if (min < fastest)
   {
      fastest = min;
      return min;
   }
   else
   {
      return UINT32_MAX;
   }
}

void part1(T& input)
{
   // Determine start and end
   uint32_t startRow = 0;
   uint32_t endRow = input[0].size()-1;
   uint32_t startCol = std::find(input[0][0].begin(), input[0][0].end(), false) - input[0][0].begin();
   uint32_t endCol = std::find(input[0][input[0].size()-1].begin(), input[0][input[0].size()-1].end(), false) - input[0][input[0].size()-1].begin();

   uint32_t fastest = UINT32_MAX;
   std::cout << findFastestPath(input, startRow, startCol, 0, fastest, endRow, endCol) << "\n";
}

void part2(T& input)
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