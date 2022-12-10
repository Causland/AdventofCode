#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

struct Tree
{
   Tree(uint32_t h) : height(h) {}
   Tree() = delete;

   bool operator<=(const Tree& t)
   {
      return this->height <= t.height;
   }

   uint32_t height{0};
   uint32_t scenicScore{0};
   bool visible{false};
};

typedef std::vector<std::vector<Tree>> T;

void checkVisibility(T& forest)
{
   for (auto i = 0; i < forest.size(); ++i)
   {
      for (auto j = 0; j < forest[i].size(); ++j)
      {
         bool isVisibleUp = true;
         bool isVisibleDown = true;
         bool isVisibleLeft = true;
         bool isVisibleRight = true;

         uint32_t viewUp = 0;
         uint32_t viewDown = 0;
         uint32_t viewLeft = 0;
         uint32_t viewRight = 0;

         do
         {
            // Up
            for(auto k = i - 1; k >= 0; --k)
            {
               if (j == 0 || j == forest[i].size() - 1) break;

               ++viewUp;

               if (forest[i][j] <= forest[k][j])
               {
                  isVisibleUp = false;
                  break;
               }
            }

            // Down
            for (auto k = i + 1; k < forest.size(); ++k)
            {
               if (j == 0 || j == forest[i].size() - 1) break;

               ++viewDown;

               if (forest[i][j] <= forest[k][j])
               {
                  isVisibleDown = false;
                  break;
               }
            }

            // Left
            for (auto k = j - 1; k >= 0; --k)
            {
               if (i == 0 || i == forest.size() - 1) break;

               ++viewLeft;

               if (forest[i][j] <= forest[i][k])
               {
                  isVisibleLeft = false;
                  break;
               }
            }

            // Right
            for (auto k = j + 1; k < forest[i].size(); ++k)
            {
               if (i == 0 || i == forest.size() - 1) break;

               ++viewRight;

               if (forest[i][j] <= forest[i][k])
               {
                  isVisibleRight = false;
                  break;
               }
            }

         } while(0);

         forest[i][j].visible = (isVisibleLeft || isVisibleRight || isVisibleUp || isVisibleDown);
         forest[i][j].scenicScore = viewLeft * viewRight * viewUp * viewDown;
      }
   }
}

void parseInputFile(std::string filename, T& forest)
{
   std::fstream f(filename);
   std::string line;

   while(std::getline(f, line))
   {
      forest.emplace_back();
      for (char& c : line)
      {
         forest.back().push_back(c - 48); // Shift 0-9 down to actual int val
      }
   }
   f.close();

   checkVisibility(forest);
}

void part1(T input)
{
   uint32_t numVisible = 0;
   for (auto& row : input)
   {
      numVisible += std::count_if(row.begin(), row.end(), [](const Tree& tree) { return tree.visible; });
   }

   std::cout << numVisible << "\n";
}

void part2(T input)
{
   uint32_t maxScore = 0;
   for (auto& row : input)
   {
      auto rowMax = std::max_element(row.begin(), row.end(), [](const Tree& a, const Tree& b){ return a.scenicScore < b.scenicScore; });
      if (rowMax->scenicScore > maxScore) maxScore = rowMax->scenicScore;
   }

   std::cout << maxScore << "\n";
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