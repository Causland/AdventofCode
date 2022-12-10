#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>

enum class Direction
{
   UP,
   DOWN,
   LEFT,
   RIGHT,
};

typedef std::vector<Direction> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string line;

   while(std::getline(f, line))
   {
      auto posSpace = line.find(' ');
      
      auto dirStr = line.substr(0, posSpace);
      Direction dir;
      if (dirStr == "U") dir = Direction::UP;
      else if (dirStr == "D") dir = Direction::DOWN;
      else if (dirStr == "L") dir = Direction::LEFT;
      else dir = Direction::RIGHT;

      auto num = std::stoi(line.substr(posSpace + 1));

      for (auto i=0; i<num; ++i)
      {
         vals.push_back(dir);
      }
   }
   f.close();
}

struct Position
{
   int32_t row{0};
   int32_t col{0};

   bool operator==(const Position& p) const
   {
      return sameRow(p) && sameCol(p);
   }

   bool operator<(const Position& p) const
   {
      return this->row < p.row || (this->row == p.row && this->col < p.col);
   }

   bool sameRow(const Position& p) const
   {
      return this->row == p.row;
   }

   bool sameCol(const Position& p) const
   {
      return this->col == p.col;
   }
};

void moveUp(Position& p) { ++p.row; }
void moveDown(Position& p) { --p.row; }
void moveLeft(Position& p) { --p.col; }
void moveRight(Position& p) { ++p.col; }
void moveDirection(Position& p, const Direction& dir)
{
   if (dir == Direction::UP) moveUp(p);
   else if (dir == Direction::DOWN) moveDown(p);
   else if (dir == Direction::LEFT) moveLeft(p);
   else moveRight(p);
}

void printPosition(std::array<Position, 10> knots, std::set<Position> visited)
{
   std::cout << "=============\n";
   for (auto row = 20; row >= 0; --row)
   {
      for (auto col = 0; col < 25; ++col)
      {
         Position p; p.row = row; p.col = col;
         bool found = false;
         for(auto i=0; i<knots.size(); ++i)
         {
            if (knots[i] == p)
            {
               std::cout << i;
               found = true;
               break;
            }
         }
         if (found) continue;
         if (visited.find(p) != visited.end())
         {
            std::cout << "#";
         }
         else
         {
            std::cout << ".";
         }
      }
      std::cout << "\n";
   }
}

void moveStep(const Direction& dir, Position& h, Position& t, std::set<Position>& visited, bool isTail)
{
   // Record position
   if (isTail)
   {
      visited.insert(t);
   }

   // Move the tail
   if (t == h)
   {
      return; // Do nothing here
   }
   else if (t.sameRow(h) || t.sameCol(h)) // No diagonals here
   {
      if (t.row - h.row > 1)
      {
         moveDirection(t, Direction::DOWN);
      }
      else if (h.row - t.row > 1)
      {
         moveDirection(t, Direction::UP);
      }
      else if (t.col - h.col > 1)
      {
         moveDirection(t, Direction::LEFT);
      }
      else if (h.col - t.col > 1)
      {
         moveDirection(t, Direction::RIGHT);
      }
      else 
      {
         return;
      }
   }
   else
   {
      // Must be on a diagonal. Check how far and move if more than 2 in one direction
      auto rowDist = std::abs(t.row - h.row);
      auto colDist = std::abs(t.col - h.col);

      Direction rowDir;
      if (t.row - h.row > 0)
      {
         rowDir = Direction::DOWN;
      }
      else
      {
         rowDir = Direction::UP;
      }
      Direction colDir;
      if (t.col - h.col > 0)
      {
         colDir = Direction::LEFT;
      } 
      else
      {
         colDir = Direction::RIGHT;
      }

      if (rowDist <= 1 && colDist <= 1)
      {
         return; // Do nothing here
      }
      else
      {
         moveDirection(t, rowDir);
         moveDirection(t, colDir);
      }
   }
}

void part1(T input)
{
   std::set<Position> visited;
   Position h;
   Position t;

   for (const auto& dir : input)
   {
      moveDirection(h, dir);
      moveStep(dir, h, t, visited, true);
   }
   visited.insert(t);

   std::cout << visited.size() << "\n";
}

void part2(T input)
{
   std::set<Position> visited;
   std::array<Position, 10> knots;
   for (auto& knot : knots)
   {
      knot.row = 5;
      knot.col = 11;
   }

   int count = 0;
   for (const auto& dir : input)
   {
      moveDirection(knots[0], dir);
      for(auto i = 0; i < knots.size() - 1; ++i)
      {
         auto& h = knots[i];
         auto& t = knots[i + 1];
         bool isTail = (i == knots.size() - 2);
         moveStep(dir, h, t, visited, isTail);
      }
   }
   visited.insert(knots[knots.size() - 1]);

   std::cout << visited.size() << "\n";
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