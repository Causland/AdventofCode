#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <set>

struct Point
{
   Point(const uint64_t& x=0, const uint64_t& y=0) : x(x), y(y) {}

   bool operator<(const Point& p) const { return x < p.x || x == p.x && y < p.y; }
   bool operator==(const Point& p) const { return x == p.x && y == p.y; }

   uint64_t x;
   uint64_t y;
};

struct Expansion
{
   std::set<uint64_t> xLocs;
   std::set<uint64_t> yLocs;
};

typedef std::pair<std::vector<Point>, Expansion> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string strVal;
   std::vector<std::string> universe;

   while(std::getline(f, strVal))
   {
      universe.push_back(strVal);
      strVal.clear();
   }
   f.close();

   // Insert expansion rows or columns if empty into set
   for (auto row=0; row < universe.size(); ++row)
   {
      if (universe[row].find('#') == std::string::npos)
      {
         // Expand the universe
         vals.second.yLocs.insert(row);
      }
   }
   for (auto col=0; col<universe[0].size(); ++col)
   {
      bool found{false};
      for (auto& line : universe)
      {
         if (line[col] == '#')
         {
            found = true;
            break;
         }
      }
      
      if (!found)
      {
         // Expand the universe
         vals.second.xLocs.insert(col);
      }
   }

   for (auto i=0; i<universe.size(); ++i)
   {
      for (auto j=0; j<universe[i].size(); ++j)
      {
         if (universe[i][j] == '#')
         {
            vals.first.emplace_back(j, i);
         }
      }
   }
}

uint64_t calcDist(const Point& a, const Point& b, const Expansion& expansion, const uint64_t& expansionRate)
{
   auto xExpansion{0};
   auto yExpansion{0};

   if (a.x < b.x)
   {
      auto count{std::count_if(expansion.xLocs.begin(), expansion.xLocs.end(), 
                                 [&a, &b](const uint64_t& e){ return a.x <= e && e < b.x; })};
      xExpansion = expansionRate * count - count;
   }
   else 
   {
      auto count{std::count_if(expansion.xLocs.begin(), expansion.xLocs.end(), 
                                 [&a, &b](const uint64_t& e){ return b.x <= e && e < a.x; })};
      xExpansion = expansionRate * count - count;
   
   }

   if (a.y < b.y)
   {
      auto count{std::count_if(expansion.yLocs.begin(), expansion.yLocs.end(), 
                                 [&a, &b](const uint64_t& e){ return a.y <= e && e < b.y; })};
      yExpansion = expansionRate * count - count;
   }
   else 
   {
      auto count{std::count_if(expansion.yLocs.begin(), expansion.yLocs.end(), 
                                 [&a, &b](const uint64_t& e){ return b.y <= e && e < a.y; })};
      yExpansion = expansionRate * count - count;
   }

   return std::abs(static_cast<int>(a.x - b.x)) + xExpansion + std::abs(static_cast<int>(a.y - b.y)) + yExpansion;
}

void part1(T input)
{
   const std::vector<Point>& galaxies{input.first};
   const Expansion& expansion{input.second};

   uint64_t sum{0};

   for (auto i=0; i<galaxies.size()-1; ++i)
   {
      for (auto j=i+1; j<galaxies.size(); ++j)
      {
         sum += calcDist(galaxies[i], galaxies[j], expansion, 2);
      }
   }

   std::cout << sum << "\n";
}

void part2(T input)
{
   const std::vector<Point>& galaxies{input.first};
   const Expansion& expansion{input.second};

   uint64_t sum{0};

   for (auto i=0; i<galaxies.size()-1; ++i)
   {
      for (auto j=i+1; j<galaxies.size(); ++j)
      {
         sum += calcDist(galaxies[i], galaxies[j], expansion, 1000000);
      }
   }

   std::cout << sum << "\n";
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

   T vals2;
   parseInputFile(part2File, vals2);
   part2(vals2);
   
   return 0;
}
