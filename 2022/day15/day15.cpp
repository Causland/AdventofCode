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

typedef std::set<Point> T;

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
      vals.insert(Point(s_x, s_y, Type::SENSOR));
      vals.insert(Point(b_x, b_y, Type::BEACON));

      auto distance = 
         (s_x > b_x ? s_x - b_x : b_x - s_x) +
         (s_y > b_y ? s_y - b_y : b_y - s_y);

      // Fill in the two halves of the diamond created by the 4 corners of the manhattan distance
      for (auto x = s_x - distance; x <= s_x + distance; ++x)
      {
         auto y = s_y - (distance - (x > s_x ? x - s_x : s_x - x));
         for (auto f_y = s_y; f_y >= y; --f_y)
         {
            vals.insert(Point(x, f_y, Type::MARKED));
         }
      }
      for (auto x = s_x - distance; x <= s_x + distance; ++x)
      {
         auto y = s_y + (distance - (x > s_x ? x - s_x : s_x - x));
         for (auto f_y = s_y; f_y <= y; ++f_y)
         {
            vals.insert(Point(x, f_y, Type::MARKED));
         }
      }
   }
   f.close();
}

void printGrid(T& input)
{
   for (auto y = -11; y <=30; ++y)
   {
      for (auto x = -10; x <=30; ++x)
      {
         auto it = input.find(Point(x,y));
         if (it != input.end())
         {
            if (it->t == Type::BEACON) std::cout << "B";
            else if (it->t == Type::SENSOR) std::cout << "S";
            else if (it->t == Type::MARKED) std::cout << "#";
            else std::cout << "This shouldn't happen in p1";
         }
         else
         {
            std::cout << ".";
         }
      }
      std::cout << "\n";
   }
}

void part1(T input)
{
   printGrid(input);
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