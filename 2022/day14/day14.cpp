#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <sstream>

enum class Type
{
   AIR,
   ROCK,
   SAND,
};

struct Point
{
   Point() {}
   Point(const int32_t x, const int32_t y) : x(x), y(y) {};
   int32_t x{0};
   int32_t y{0};
};

struct Line
{
   Line(Point a, Point b) : p1(a), p2(b) {};
   Point p1;
   Point p2;

   std::vector<Point> getPoints()
   {
      std::vector<Point> points;
      if (p1.x == p2.x)
      {
         for (auto i = std::min(p1.y, p2.y); i <= std::max(p1.y, p2.y); ++i)
         {
            points.emplace_back(p1.x, i);
         }
      }
      else if (p1.y == p2.y)
      {
         for (auto i = std::min(p1.x, p2.x); i <= std::max(p1.x, p2.x); ++i)
         {
            points.emplace_back(i, p1.y);
         }
      }
      else
      {
         std::cout << "Diagonal lines should not happen";
      }
      return points;
   }
};

typedef std::vector<std::vector<Type>> T;

void parseInputFile(std::string filename, T& vals, bool part2)
{
   std::fstream f(filename);
   std::string line;

   std::vector<Line> lines;
   uint32_t minx = UINT32_MAX;
   uint32_t maxx = 0;
   uint32_t miny = UINT32_MAX;
   uint32_t maxy = 0;
   while(std::getline(f, line))
   {
      Point lastPoint;
      bool firstPoint = true;
      while(!line.empty())
      {
         std::string coord;
         auto posDash = line.find('-');
         coord = line.substr(0, posDash - 1);
         if (posDash == std::string::npos)
         {
            line.clear();
         }
         else
         {
            line.erase(0, posDash + 3);
         }

         auto posComma = coord.find(',');
         Point p;
         p.x = std::stoul(coord.substr(0, posComma));
         p.y = std::stoul(coord.substr(posComma + 1));

         if (p.x < minx) minx = p.x;
         if (p.x > maxx) maxx = p.x;
         if (p.y < miny) miny = p.y;
         if (p.y > maxy) maxy = p.y;

         if (!firstPoint)
         {
            lines.emplace_back(lastPoint, p);
         }
         lastPoint = p;
         firstPoint = false;
      }
   }
   f.close();

   if (part2)
   {
      // Based on 30/60/90 triangle, the base of the triangle should be at least 2 times the height
      maxy += 2;
      minx = std::max(0u, 500 - maxy);
      maxx = 500 + maxy;
      lines.emplace_back(Point(minx, maxy), Point(maxx, maxy));
   }

   // Shift all x coordinates by the min - 1
   for (auto& line : lines)
   {
      line.p1.x -= minx;
      line.p2.x -= minx;
   }

   // Construct the map
   for (auto y = 0; y <= maxy; ++y)
   {
      vals.emplace_back();
      for (auto x = 0; x <= maxx - minx; ++x)
      {
         vals[y].push_back(Type::AIR);
      }
   }
   vals[0][500 - minx] = Type::SAND;
   for (auto& line : lines)
   {
      for (auto& p : line.getPoints())
      {
         vals[p.y][p.x] = Type::ROCK;
      }
   }
}

void printMap(T& map)
{
   for (auto& row : map)
   {
      for (auto& type : row)
      {
         if (type == Type::AIR) std::cout << ".";
         else if (type == Type::SAND) std::cout << "o";
         else if (type == Type::ROCK) std::cout << "#";
      }
      std::cout << "\n";
   }
}

uint32_t countUnitsOfSand(T& input, Point& source)
{
   uint32_t unitsOfSand = 0;
   bool edgeReached = false;
   while(!edgeReached)
   {
      ++unitsOfSand;
      bool sandFalling = true;
      Point p = source;
      while(sandFalling)
      {
         sandFalling = false;
         // Move sand to next location
         if (p.y + 1 < input.size())
         {
            if (input[p.y + 1][p.x] == Type::AIR)
            {
               p.y = p.y + 1;
               sandFalling = true;
            }
            else
            {
               if (p.x - 1 >= 0)
               {
                  if (input[p.y + 1][p.x - 1] == Type::AIR)
                  {
                     p.y = p.y + 1;
                     p.x = p.x - 1;
                     sandFalling = true;
                  }
                  else
                  {
                     if (p.x + 1 < input[0].size())
                     {
                        if (input[p.y + 1][p.x + 1] == Type::AIR)
                        {
                           p.y = p.y + 1;
                           p.x = p.x + 1;
                           sandFalling = true;
                        }
                     }
                     else
                     {
                        edgeReached = true;
                     }
                  }
               }
               else
               {
                  edgeReached = true;
               }
            }
         }
         else
         {
            edgeReached = true; // We've hit the bottom of the map
                                // We must be falling off the edge
         }
         if (p.x == source.x && p.y == source.y)
         {
            edgeReached = true;
            break;
         }
      }
      input[p.y][p.x] = Type::SAND;
   }
   return unitsOfSand;
}

void part1(T input)
{
   // Find the source. Was at 500,0 but its not there with shift
   Point source;
   for (auto i = 0; i < input.size(); ++i)
   {
      for (auto j = 0; j < input[0].size(); ++j)
      {
         if (input[i][j] == Type::SAND) 
         {
            source = Point(j, i);
            break;
         }
      }
   }

   auto unitsOfSand = countUnitsOfSand(input, source);

   std::cout << unitsOfSand - 1 << "\n"; // Don't count the fall
}

void part2(T input)
{
   // Find the source. Was at 500,0 but its not there with shift
   Point source;
   for (auto i = 0; i < input.size(); ++i)
   {
      for (auto j = 0; j < input[0].size(); ++j)
      {
         if (input[i][j] == Type::SAND) 
         {
            source = Point(j, i);
            break;
         }
      }
   }

   auto unitsOfSand = countUnitsOfSand(input, source);

   std::cout << unitsOfSand << "\n";
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
   parseInputFile(part1File, vals, false);
   part1(vals);

   vals.clear();

   parseInputFile(part2File, vals, true);
   part2(vals);
   
   return 0;
}