#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>

enum class Direction
{
   UP,
   DOWN,
   LEFT,
   RIGHT,
};

typedef std::vector<std::string> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string strVal;

   while(std::getline(f, strVal))
   {
      vals.push_back(strVal);
   }
   f.close();
}

void printGrid(const T& grid)
{
   for (const auto& row : grid)
   {
      std::cout << row << "\n";
   }
}

void shiftGrid(T& grid, const Direction& dir)
{
   if (dir == Direction::UP)
   {
      for (int32_t row=0; row<grid.size(); ++row)
      {
         for (int32_t col=0; col<grid[0].size(); ++col)
         {
            if (grid[row][col] == 'O')
            {
               // Shift until not possible
               grid[row][col] = '.';
               auto i{row};
               while (i >= 0)
               {
                  if (grid[i][col] != '.') break;
                  --i;
               }
               grid[i+1][col] = 'O';
            }
         }
      }
   }
   else if (dir == Direction::DOWN)
   {
      for (int32_t row=grid.size()-1; row>=0; --row)
      {
         for (int32_t col=0; col<grid[0].size(); ++col)
         {
            if (grid[row][col] == 'O')
            {
               // Shift until not possible
               grid[row][col] = '.';
               auto i{row};
               while (i < grid.size())
               {
                  if (grid[i][col] != '.') break;
                  ++i;
               }
               grid[i-1][col] = 'O';
            }
         }
      }
   }
   else if (dir == Direction::LEFT)
   {
      for (int32_t col=0; col<grid[0].size(); ++col)
      {
         for (int32_t row=0; row<grid.size(); ++row)
         {
            if (grid[row][col] == 'O')
            {
               // Shift until not possible
               grid[row][col] = '.';
               auto i{col};
               while (i >= 0)
               {
                  if (grid[row][i] != '.') break;
                  --i;
               }
               grid[row][i+1] = 'O';
            }
         }
      }
   }
   else 
   {
      for (int32_t col=grid[0].size()-1; col>=0; --col)
      {
         for (int32_t row=0; row<grid.size(); ++row)
         {
            if (grid[row][col] == 'O')
            {
               // Shift until not possible
               grid[row][col] = '.';
               auto i{col};
               while (i < grid[0].size())
               {
                  if (grid[row][i] != '.') break;
                  ++i;
               }
               grid[row][i-1] = 'O';
            }
         }
      }
   }
}

void part1(T input)
{
   shiftGrid(input, Direction::UP);

   uint64_t sum{0};

   for (auto row=0; row<input.size(); ++row)
   {
      for (auto col=0; col<input[0].size(); ++col)
      {
         if (input[row][col] == 'O')
         {
            sum += input.size() - row;
         }
      }
   }

   std::cout << sum << "\n";
}

void part2(T input)
{
   std::vector<T> history{input};
   auto loopStart{0};
   auto loopSize{0};
   while (true)
   {
      shiftGrid(input, Direction::UP);
      shiftGrid(input, Direction::LEFT);
      shiftGrid(input, Direction::DOWN);
      shiftGrid(input, Direction::RIGHT);

      auto it{std::find(history.begin(), history.end(), input)};
      if (it != history.end())
      {
         loopStart = std::distance(history.begin(), it);
         loopSize = std::distance(it, history.end());
         break;
      }

      history.push_back(input);
   }

   auto cyclePos{(1000000000 - loopStart) % loopSize};

   auto targetGrid{history[loopStart+cyclePos]};
   uint64_t sum{0};

   for (auto row=0; row<targetGrid.size(); ++row)
   {
      for (auto col=0; col<targetGrid[0].size(); ++col)
      {
         if (targetGrid[row][col] == 'O')
         {
            sum += targetGrid.size() - row;
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
