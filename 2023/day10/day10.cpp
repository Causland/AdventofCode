#include <iostream>
#include <cstdint>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <queue>
#include <cmath>
#include <algorithm>

struct Point
{
   Point(const uint64_t& x=0, const uint64_t& y=0,
         const uint64_t& p_x=0, const uint64_t& p_y=0) : x(x), y(y), p_x(p_x), p_y(p_y) {}

   bool operator<(const Point& p) const { return y < p.y || y == p.y && x < p.x; }

   uint64_t x;
   uint64_t y;

   uint64_t p_x;
   uint64_t p_y;
};

typedef std::pair<Point, std::vector<std::string>> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string line;

   uint64_t y{0};
   while(std::getline(f, line))
   {
      vals.second.push_back(line);
      const auto pos{line.find('S')};
      if (pos != std::string::npos)
      {
         vals.first.x = pos;
         vals.first.y = y;
      }
      ++y;
   }
   f.close();
}

bool checkUp(const Point& p, const std::vector<std::string>& grid)
{
   if (p.y > 0)
   {
      if (p.y - 1 == p.p_y && p.x == p.p_x) return false;

      char c{grid[p.y-1][p.x]};
      if (c == '|' || c == '7' || c == 'F' || c == 'S') return true;
   }
   return false;
}
bool checkDown(const Point& p, const std::vector<std::string>& grid)
{
   if (p.y < grid.size() - 1)
   {
      if (p.y + 1 == p.p_y && p.x == p.p_x) return false;

      char c{grid[p.y+1][p.x]};
      if (c == '|' || c == 'L' || c == 'J' || c == 'S') return true;
   }
   return false;
}
bool checkLeft(const Point& p, const std::vector<std::string>& grid)
{
   if (p.x > 0)
   {
      if (p.y == p.p_y && p.x - 1 == p.p_x) return false;

      char c{grid[p.y][p.x-1]};
      if (c == '-' || c == 'L' || c == 'F' || c == 'S') return true;
   }
   return false;
}
bool checkRight(const Point& p, const std::vector<std::string>& grid)
{
   if (p.x < grid[0].size() - 1)
   {
      if (p.y == p.p_y && p.x + 1 == p.p_x) return false;

      char c{grid[p.y][p.x+1]};
      if (c == '-' || c == 'J' || c == '7' || c == 'S') return true;
   }
   return false;
}

std::set<Point> calcLoop(Point& start, const std::vector<std::string>& grid)
{
   start.p_x = UINT64_MAX;
   start.p_y = UINT64_MAX;

   // Find the loop using BFS
   std::set<Point> loop;

   for (auto i=0; i<4; ++i)
   {
      std::set<Point> visited;
      std::queue<Point> to_visit;
      
      // Skip the starting node and insert one direction 
      // into the to_visit queue to detect loop back to start
      if (i == 0)
      {
         if (checkUp(start, grid)) to_visit.push(Point(start.x, start.y-1, start.x, start.y));
      }
      else if (i == 1)
      {
         if (checkDown(start, grid)) to_visit.push(Point(start.x, start.y+1, start.x, start.y));
      }
      else if (i == 2)
      {
         if (checkLeft(start, grid)) to_visit.push(Point(start.x-1, start.y, start.x, start.y));
      }
      else 
      {
         if (checkRight(start, grid)) to_visit.push(Point(start.x+1, start.y, start.x, start.y));
      }

      while(!to_visit.empty())
      {
         Point p{to_visit.front()};
         to_visit.pop();

         if (visited.find(p) != visited.end()) continue;
         visited.insert(p);

         if (p.x == start.x && p.y == start.y) break;

         // Insert more nodes to check
         char c{grid[p.y][p.x]};
         if (c == '|')
         {
            if (checkUp(p, grid)) to_visit.push(Point(p.x, p.y-1, p.x, p.y));
            if (checkDown(p, grid)) to_visit.push(Point(p.x, p.y+1, p.x, p.y));   
         }
         else if (c == '-')
         {
            if (checkLeft(p, grid)) to_visit.push(Point(p.x-1, p.y, p.x, p.y));
            if (checkRight(p, grid)) to_visit.push(Point(p.x+1, p.y, p.x, p.y));   
         }
         else if (c == 'L')
         {
            if (checkUp(p, grid)) to_visit.push(Point(p.x, p.y-1, p.x, p.y));
            if (checkRight(p, grid)) to_visit.push(Point(p.x+1, p.y, p.x, p.y));   
         }
         else if (c == 'J')
         {
            if (checkUp(p, grid)) to_visit.push(Point(p.x, p.y-1, p.x, p.y));
            if (checkLeft(p, grid)) to_visit.push(Point(p.x-1, p.y, p.x, p.y));
         }
         else if (c == '7') 
         {
            if (checkLeft(p, grid)) to_visit.push(Point(p.x-1, p.y, p.x, p.y));
            if (checkDown(p, grid)) to_visit.push(Point(p.x, p.y+1, p.x, p.y));   

         }
         else if (c == 'F')
         {
            if (checkRight(p, grid)) to_visit.push(Point(p.x+1, p.y, p.x, p.y));   
            if (checkDown(p, grid)) to_visit.push(Point(p.x, p.y+1, p.x, p.y));   
         }
      }

      auto it{visited.find(start)};
      if (it != visited.end())
      {
         do
         {
            loop.insert(*it);
            it = visited.find(Point(it->p_x, it->p_y));
         }
         while (it->x != start.x || it->y != start.y);

         break; // Don't execute more for loops
      }
   }

   return loop;
}

void part1(T input)
{
   const auto loop{calcLoop(input.first, input.second)};
   std::cout << std::ceil(static_cast<double>(loop.size()) / 2) << "\n";
}

void part2(T input)
{
   const auto loop{calcLoop(input.first, input.second)};

   // Scan from top to bottom of loop, calculating points between loop
   const Point tl{*loop.begin()};
   const Point br{*std::prev(loop.end())};

   for (const auto& p : loop)
   {
        std::cout << p.x << " " << p.y << "\n";
   }

   uint64_t count{0};
   for (auto y=tl.y+1; y<=br.y-1; ++y)
   {
      std::vector<Point> rowSet;
      std::copy_if(loop.begin(), loop.end(), std::back_inserter(rowSet), [&y](const Point& p){ return p.y == y; });

      int64_t vertCount{0};
      for (auto it=rowSet.begin(); it != rowSet.end(); ++it)
      {
         if (it != rowSet.begin())
         {
            auto dist{it->x - std::prev(it)->x};
            if (dist > 1 && vertCount % 2 != 0) 
            {
               count += dist - 1;
            }
         }

         auto c{input.second[it->y][it->x]};
         if (c == '|' || c == 'F' || c == '7' || c == 'S') ++vertCount;

      }
   }

   std::cout << count << "\n";
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
      part2File = "test3.txt";
   }
   else
   {
      part2File = "input.txt";
   }

   T vals;
   parseInputFile(part1File, vals);
   part1(vals);

   T vals2;
   parseInputFile(part2File, vals2);
   part2(vals2);
   
   return 0;
}
