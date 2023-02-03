#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>
#include <list>

typedef std::vector<std::vector<std::vector<bool>>> T;

struct Node
{
   Node(const uint32_t& row, const uint32_t& col, const uint32_t& minute) : row(row), col(col), minute(minute) {};
   bool operator<(const Node& p) const { return this->row < p.row || 
                                           (this->row == p.row && this->col < p.col) ||
                                           (this->row == p.row && this->col == p.col && this->minute < p.minute); };
   bool operator==(const Node& p) const { return this->row == p.row && this->col == p.col && this->minute == p.minute; };

   uint32_t h{0};
   uint32_t f{0};
   uint32_t g{0};

   uint32_t row{0};
   uint32_t col{0};
   uint32_t minute{0};
};

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

uint32_t calculateH(const uint32_t& endRow, const uint32_t& endCol, const uint32_t& currRow, const uint32_t& currCol)
{
   return ((currRow > endRow) ? currRow - endRow : endRow - currRow) +
          ((currCol > endCol) ? currCol - endCol : endCol - currCol);
}

uint32_t findShortestPath(T& maps, const uint32_t& startRow, const uint32_t& startCol, const uint32_t& startMinute, const uint32_t& endRow, const uint32_t& endCol)
{
   std::vector<Node> openList;
   std::vector<Node> closedList;

   Node start(startRow, startCol, startMinute);
   start.g = 0;
   start.h = calculateH(endRow, endCol, startRow, startCol);
   start.f = start.g + start.h;
   openList.push_back(start);

   while(!openList.empty())
   {
      const auto& mit = std::min_element(openList.begin(), openList.end(), [](const auto& a, const auto& b){ return a.f < b.f; });

      if (mit->row == endRow && mit->col == endCol)
      {
         return mit->minute;
      }

      const auto m = *mit;
      openList.erase(mit);
      closedList.push_back(m);

      const auto& map = maps[(m.minute+1)%maps.size()];
      std::vector<Node> children;
      if (!map[m.row][m.col]) children.emplace_back(m.row, m.col, m.minute+1);
      if (m.row < map.size()-1 && !map[m.row+1][m.col]) children.emplace_back(m.row+1, m.col, m.minute+1);
      if (m.row > 0 && !map[m.row-1][m.col]) children.emplace_back(m.row-1, m.col, m.minute+1);
      if (!map[m.row][m.col+1]) children.emplace_back(m.row, m.col+1, m.minute+1);
      if (!map[m.row][m.col-1]) children.emplace_back(m.row, m.col-1, m.minute+1);

      for (const auto& n : children)
      {
         if (std::find(closedList.begin(), closedList.end(), n) != closedList.end()) continue;
         auto cost = m.g + 1;

         const auto& oit = std::find(openList.begin(), openList.end(), n);
         if (oit != openList.end() && cost < n.g)
         {
            openList.erase(oit);
         }
         const auto& cit = std::find(closedList.begin(), closedList.end(), n);
         if (cit != closedList.end() && cost < n.g)
         {
            closedList.erase(cit);
         }
         if (oit == openList.end() && cit == closedList.end())
         {
            openList.push_back(n);
            auto& node = openList.back();
            node.g = cost;
            node.h = calculateH(endRow, endCol, node.row, node.col);
            node.f = node.g + node.h;
         }
      }
   }

   return UINT32_MAX;
}

void part1(T& input)
{
   // Determine start and end
   uint32_t startRow = 0;
   uint32_t endRow = input[0].size()-1;
   uint32_t startCol = std::find(input[0][0].begin(), input[0][0].end(), false) - input[0][0].begin();
   uint32_t endCol = std::find(input[0][input[0].size()-1].begin(), input[0][input[0].size()-1].end(), false) - input[0][input[0].size()-1].begin();

   std::cout << findShortestPath(input, startRow, startCol, 0, endRow, endCol) << "\n";
}

void part2(T& input)
{
   // Determine start and end
   uint32_t startRow = 0;
   uint32_t endRow = input[0].size()-1;
   uint32_t startCol = std::find(input[0][0].begin(), input[0][0].end(), false) - input[0][0].begin();
   uint32_t endCol = std::find(input[0][input[0].size()-1].begin(), input[0][input[0].size()-1].end(), false) - input[0][input[0].size()-1].begin();

   auto there = findShortestPath(input, startRow, startCol, 0, endRow, endCol);
   auto back = findShortestPath(input, endRow, endCol, there, startRow, startCol);
   auto backAgain = findShortestPath(input, startRow, startCol, back, endRow, endCol);

   std::cout << backAgain << "\n";
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