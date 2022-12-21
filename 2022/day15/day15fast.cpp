#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>
#include <cstdio>
#include <thread>
#include <mutex>

enum class Type
{
   BEACON,
   SENSOR,
   MARKED,
   UNMARKED,
};

struct Point
{
   Point() {}
   Point(const int32_t x, const int32_t y) : x(x), y(y) {};
   Point(const int32_t x, const int32_t y, const Type t) : x(x), y(y), t(t) {}; 

   bool operator<(const Point& p) const { return x < p.x || x == p.x && y < p.y; }

   int32_t x{0};
   int32_t y{0};
   Type t{Type::UNMARKED};
};

typedef std::vector<std::pair<Point, Point>> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string line;

   while(std::getline(f, line))
   {
      int32_t s_x = 0;
      int32_t s_y = 0;
      int32_t b_x = 0;
      int32_t b_y = 0;
      sscanf(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
               &s_x, &s_y, &b_x, &b_y);

      vals.push_back({Point(s_x, s_y), Point(b_x, b_y)});  
   }       
}

uint32_t countInRow(T& pairsToCheck, int32_t targetY)
{
   std::set<Point> hits;
   for (auto& pair : pairsToCheck)
   {
      auto s_x = pair.first.x;
      auto s_y = pair.first.y;
      auto b_x = pair.second.x;
      auto b_y = pair.second.y;

      auto distance = 
         (s_x > b_x ? s_x - b_x : b_x - s_x) +
         (s_y > b_y ? s_y - b_y : b_y - s_y);

      // Skip any sensors that do not have a FOV which reaches target
      int32_t numIntersect = 2 * (distance - (s_y > targetY ? s_y - targetY : targetY - s_y)) + 1;
      if (numIntersect <= 0) continue;

      if (b_y == targetY)
      {
         auto it = hits.find(Point(b_x, b_y));
         if (it != hits.end())
         {
            hits.erase(it);
         }
         hits.insert(Point(b_x, b_y, Type::BEACON));
      }
      if (s_y == targetY)
      {
         auto it = hits.find(Point(s_x, s_y));
         if (it != hits.end())
         {
            hits.erase(it);
         }
         hits.insert(Point(s_x, s_y, Type::SENSOR));
      }

      for (auto x = s_x - numIntersect / 2; x <= s_x + numIntersect / 2; ++x)
      {
         hits.insert(Point(x, targetY, Type::MARKED));
      }
   }

   // Do some post processing to remove non-beaconable spots
   std::erase_if(hits, [](auto& p){ return p.t != Type::MARKED; });
   return hits.size();
}

void part1(T& input)
{
   auto result = countInRow(input, 2000000);
   std::cout << result << "\n";
}

Point findMissingBeacon(std::vector<std::pair<Point, uint32_t>>& sensors,
                           const int32_t xmin, const int32_t xmax, const int32_t ymin, const int32_t ymax)
{
   // Go through each row and check for an unseen space
   for (auto y = ymin; y <= ymax; ++y)
   {
      for (auto x = xmin; x <= xmax; ++x)
      {
         bool foundEmpty = true;
         // Check each sensor to see if the current space is visible
         for (auto& pair : sensors)
         {
            // Get the distance to the sensor
            auto xdist = x > pair.first.x ? x - pair.first.x : pair.first.x - x;
            auto ydist = y > pair.first.y ? y - pair.first.y : pair.first.y - y;
            auto distToSensor = xdist + ydist;
            
            // If the sensor is visible
            if (distToSensor <= pair.second)
            {
               // Skip to the space just after the last visible space in current row
               x = pair.first.x + (pair.second - ydist);

               foundEmpty = false;
               break;
            }
         }
         if (foundEmpty) return Point(x,y);
      }
   }
   std::cout << "Failed to find point\n";
   return Point(0,0);
}

void part2(T& input)
{
   const int32_t xmin = 0;
   const int32_t xmax = 4000000;
   const int32_t ymin = 0;
   const int32_t ymax = 4000000;

   std::vector<std::pair<Point, uint32_t>> points;
   for (auto& pair : input)
   {
      auto s_x = pair.first.x;
      auto s_y = pair.first.y;
      auto b_x = pair.second.x;
      auto b_y = pair.second.y;

      uint32_t distance = 
         (s_x > b_x ? s_x - b_x : b_x - s_x) +
         (s_y > b_y ? s_y - b_y : b_y - s_y);

      points.push_back({pair.first, distance});
   }

   Point result = findMissingBeacon(points, xmin, xmax, ymin, ymax);
   std::cout << static_cast<uint64_t>(result.x) * 4000000 + static_cast<uint64_t>(result.y) << "\n";
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