#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>

typedef std::vector<std::vector<std::string>> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string line;

   vals.emplace_back();
   while(std::getline(f, line))
   {
      if (line.empty()) 
      {
         vals.emplace_back();
         continue;
      }

      vals.back().push_back(line);
   }

   f.close();
}

uint64_t checkReflectRow(const std::vector<std::string>& grid, const uint64_t& tRow, const uint64_t& bRow)
{
   int64_t i{static_cast<int64_t>(tRow)};
   int64_t j{static_cast<int64_t>(bRow)};

   while (i >= 0 && j < grid.size())
   {
      if (grid[i] != grid[j]) return 0;

      --i;
      ++j;
   }

   return bRow;
}

uint64_t checkReflectCol(const std::vector<std::string>& grid, const uint64_t& lCol, const uint64_t& rCol)
{
   int64_t i{static_cast<int64_t>(lCol)};
   int64_t j{static_cast<int64_t>(rCol)};

   while (i >= 0 && j < grid[0].size())
   {
      for (auto row=0; row<grid.size(); ++row)
      {
         if (grid[row][i] != grid[row][j]) return 0;
      }

      --i;
      ++j;
   }

   return rCol;
}

uint64_t fixSmudgeRow(const std::vector<std::string>& grid, const uint64_t& tRow, const uint64_t& bRow)
{
   int64_t i{static_cast<int64_t>(tRow)};
   int64_t j{static_cast<int64_t>(bRow)};

   bool diffFound{false};
   while (i >= 0 && j < grid.size())
   {
      for (auto col=0; col<grid[0].size(); ++col)
      {
         if (grid[i][col] != grid[j][col])
         {
            if (diffFound) return 0; // Two differences not a smudge
            diffFound = true;
         }
      }

      --i;
      ++j;
   }

   if (diffFound) return bRow;

   return 0;
}

uint64_t fixSmudgeCol(const std::vector<std::string>& grid, const uint64_t& lCol, const uint64_t& rCol)
{
   int64_t i{static_cast<int64_t>(lCol)};
   int64_t j{static_cast<int64_t>(rCol)};

   bool diffFound{false};
   while (i >= 0 && j < grid[0].size())
   {
      for (auto row=0; row<grid.size(); ++row)
      {
         if (grid[row][i] != grid[row][j])
         {
            if (diffFound) return 0; // Two differences not a smudge
            diffFound = true;
         }
      }

      --i;
      ++j;
   }

   if (diffFound) return rCol;

   return 0;
}

void part1(T input)
{
   uint64_t sum{0};

   // For each grid, check rows for reflection, then cols
   for (const auto& grid : input)
   {
      bool found{false};
      for (auto i=0; i<grid.size()-1; ++i)
      {
         uint64_t sym{checkReflectRow(grid, i, i+1)};
         if (sym > 0)
         {
            sum += (100 * sym);
            found = true;
            break;
         }
      }

      if (found) continue;

      for (auto i=0; i<grid[0].size()-1; ++i)
      {
         uint64_t sym{checkReflectCol(grid, i, i+1)};
         if (sym > 0)
         {
            sum += sym;
            break;
         }
      }
   }

   std::cout << sum << "\n";
}

void part2(T input)
{
   uint64_t sum{0};

   // For each grid, check rows for reflection, then cols
   for (const auto& grid : input)
   {
      bool found{false};
      for (auto i=0; i<grid.size()-1; ++i)
      {
         uint64_t sym{fixSmudgeRow(grid, i, i+1)};
         if (sym > 0)
         {
            sum += (100 * sym);
            found = true;
            break;
         }
      }

      if (found) continue;

      for (auto i=0; i<grid[0].size()-1; ++i)
      {
         uint64_t sym{fixSmudgeCol(grid, i, i+1)};
         if (sym > 0)
         {
            sum += sym;
            break;
         }
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

   vals.clear();

   parseInputFile(part2File, vals);
   part2(vals);
   
   return 0;
}
