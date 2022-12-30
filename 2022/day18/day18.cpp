#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <cstdio>
#include <set>

struct Cube
{
   Cube() {};
   Cube(const int32_t& x, const int32_t& y, const int32_t& z) : x(x), y(y), z(z) {};
   bool operator<(const Cube& c) const 
   {
       return x < c.x || 
              x == c.x && y < c.y ||
              x == c.x && y == c.y && z < c.z; 
   }

   int32_t x{0};
   int32_t y{0};
   int32_t z{0}; 
};

typedef std::set<Cube> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string line;

   while(std::getline(f, line))
   {
      Cube c;
      sscanf(line.c_str(), "%u,%u,%u", &c.x, &c.y, &c.z);
      vals.insert(c);
   }
   f.close();
}

bool checkNeighbor(const Cube& a, const Cube& b)
{
   return (a.x > b.x ? a.x - b.x : b.x - a.x) + 
          (a.y > b.y ? a.y - b.y : b.y - a.y) +
          (a.z > b.z ? a.z - b.z : b.z - a.z) == 1;
}

uint32_t countNeighbors(const T& cubes, const Cube& c)
{
   return std::count_if(cubes.begin(), cubes.end(), [c](const auto& aCube){ return checkNeighbor(c, aCube); });
}

bool isInternal(const T& cubes, const Cube& c, 
                const int32_t minX, const int32_t minY, const int32_t minZ, 
                const int32_t maxX, const int32_t maxY, const int32_t maxZ)
{

   // Must find a cube in all 6 directions
   // -x
   bool found = false;
   for (int32_t x = c.x - 1; x >= minX; --x)
   {
      if (cubes.find(Cube(x, c.y, c.z)) != cubes.end())
      {
         found = true;
         break;
      }
   }
   if (!found) return false;
   // x
   found = false;
   for (int32_t x = c.x + 1; x <= maxX; ++x)
   {
      if (cubes.find(Cube(x, c.y, c.z)) != cubes.end())
      {
         found = true;
         break;
      }
   }
   if (!found) return false;
   // -y
   found = false;
   for (int32_t y = c.y - 1; y >= minY; --y)
   {
      if (cubes.find(Cube(c.x, y, c.z)) != cubes.end())
      {
         found = true;
         break;
      }
   }
   if (!found) return false;
   // y
   found = false;
   for (int32_t y = c.y + 1; y <= maxY; ++y)
   {
      if (cubes.find(Cube(c.x, y, c.z)) != cubes.end())
      {
         found = true;
         break;
      }
   }
   if (!found) return false;
   // -z
   found = false;
   for (int32_t z = c.z - 1; z >= minZ; --z)
   {
      if (cubes.find(Cube(c.x, c.y, z)) != cubes.end())
      {
         found = true;
         break;
      }
   }
   if (!found) return false;
   // z
   found = false;
   for (int32_t z = c.z + 1; z <= maxZ; ++z)
   {
      if (cubes.find(Cube(c.x, c.y, z)) != cubes.end())
      {
         found = true;
         break;
      }
   }
   if (!found) return false;

   return true;
}

void part1(T input)
{
   uint32_t surfaceArea = input.size() * 6; // Max amount of sides 

   for (const auto& cube : input)
   {
      surfaceArea -= countNeighbors(input, cube);
   }

   std::cout << surfaceArea << "\n";
}

void part2(T input)
{
   int32_t minX{INT32_MAX};
   int32_t maxX{0};
   int32_t minY{INT32_MAX};
   int32_t maxY{0};
   int32_t minZ{INT32_MAX};
   int32_t maxZ{0};

   for (const auto& cube : input)
   {
      if (cube.x < minX) minX = cube.x;
      else if (cube.x > maxX) maxX = cube.x;

      if (cube.y < minY) minY = cube.y;
      else if (cube.y > maxY) maxY = cube.y;

      if (cube.z < minZ) minZ = cube.z;
      else if (cube.z > maxZ) maxZ = cube.z;
   }

   // Create the complementary set to the input
   std::set<Cube> complement;
   for (auto x = minX; x <= maxX; ++x)
   {
      for (auto y = minY; y <= maxY; ++y)
      {
         for (auto z = minZ; z <= maxZ; ++z)
         {
            Cube c(x,y,z);
            if (input.find(c) == input.end() && isInternal(input, c, minX, minY, minZ, maxX, maxY, maxZ))
            {
               complement.insert(c);
            }
         }
      }
   }

   // Like part 1 find the surface area with internal surfaces
   uint32_t surfaceArea = input.size() * 6; // Max amount of sides 

   for (const auto& cube : input)
   {
      surfaceArea -= countNeighbors(input, cube);
   }

   // Subtract off any internal surface area
   for (const auto& cube : complement)
   {
      surfaceArea -= countNeighbors(input, cube);
   }

   std::cout << surfaceArea << "\n";
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