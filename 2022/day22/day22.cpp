#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

enum class Type
{
   OPEN,
   WALL,
   EDGE,
   OOB,
};

enum class Direction
{
   RIGHT = 0,
   DOWN = 1,
   LEFT = 2,
   UP = 3,
};

struct Player
{
   void rotate(const char dir)
   {
      if (dir == 'R')
      {
         if (lookDirection == Direction::UP) lookDirection = Direction::RIGHT;
         else if (lookDirection == Direction::RIGHT) lookDirection = Direction::DOWN;
         else if (lookDirection == Direction::DOWN) lookDirection = Direction::LEFT;
         else lookDirection = Direction::UP;
      }
      else
      {
         if (lookDirection == Direction::UP) lookDirection = Direction::LEFT;
         else if (lookDirection == Direction::LEFT) lookDirection = Direction::DOWN;
         else if (lookDirection == Direction::DOWN) lookDirection = Direction::RIGHT;
         else lookDirection = Direction::UP;
      }
   }

   Direction lookDirection{Direction::RIGHT};
   uint32_t row{0};
   uint32_t col{0};
};

struct Position
{
   Position(uint32_t r, uint32_t c) : row(r), col(c) {};
   bool operator<(const Position& p) const
   {
      return this->row < p.row || (this->row == p.row && this->col < p.col); 
   }

   uint32_t row{0};
   uint32_t col{0};
};

typedef std::vector<std::vector<Type>> T;

void parseInputFile(std::string filename, T& vals, std::string& movement)
{
   std::fstream f(filename);
   std::string line;
   std::vector<std::string> board;

   bool processingMap{true};
   while(std::getline(f, line))
   {
      if (processingMap)
      {
         if (line.empty())
         { 
            processingMap = false; 
            continue;
         }
         board.push_back(line);
      }
      else
      {
         movement = line;
      }
   }
   f.close();

   // Build out the map of different type tiles
   // Start with the top line
   vals.emplace_back();
   vals.back().push_back(Type::OOB);
   for (auto col=0; col<board[0].length(); ++col)
   {
      if (board[0][col] != ' ') vals.back().push_back(Type::EDGE);
      else vals.back().push_back(Type::OOB);
   }
   // Fill in the middle rows with left and right boundaries
   for (auto row=0; row < board.size(); ++row)
   {
      vals.emplace_back();
      // Insert a character at the beginning of each row
      if (board[row][0] != ' ') vals.back().push_back(Type::EDGE);
      else vals.back().push_back(Type::OOB);

      for (auto col=0; col < board[row].length(); ++col)
      {
         if (board[row][col] == ' ')
         {
            if (row + 1 < board.size() && board[row+1][col] != ' ') vals.back().push_back(Type::EDGE);
            else if (row - 1 >= 0 && board[row-1][col] != ' ') vals.back().push_back(Type::EDGE);
            else if (col + 1 < board[row].length() && board[row][col+1] != ' ') vals.back().push_back(Type::EDGE);
            else if (col - 1 >= 0 && board[row][col-1] != ' ') vals.back().push_back(Type::EDGE);
            else vals.back().push_back(Type::OOB);
         }
         else if (board[row][col] == '.') vals.back().push_back(Type::OPEN);
         else vals.back().push_back(Type::WALL);
      }

      // Insert a character at the end of each row
      if (board[row][board[row].length()-1] != ' ') vals.back().push_back(Type::EDGE);
      else vals.back().push_back(Type::OOB);

      if (row - 1 >= 0 && board[row].length() < board[row-1].length())
      {
         auto dist = board[row-1].length() - board[row].length() - 1;
         while(dist--) vals.back().push_back(Type::EDGE);
      }

      if (row + 1 < board.size() && board[row].length() < board[row+1].length())
      {
         auto dist = board[row+1].length() - board[row].length() - 1;
         while(dist--) vals.back().push_back(Type::EDGE);
      }
   }
   // Add a bottom line
   vals.emplace_back();
   vals.back().push_back(Type::OOB);
   for (auto col=0; col<board[board.size()-1].length(); ++col)
   {
      if (board[board.size()-1][col] != ' ') vals.back().push_back(Type::EDGE);
      else vals.back().push_back(Type::OOB);
   }

   // Find the longest row
   auto it = std::max_element(board.begin(), board.end(), [](auto& a, auto& b){ return a.length() < b.length(); });
   auto length = it->size();

   for (auto& row : vals)
   {
      while(row.size() < length) row.push_back(Type::OOB);
   }
}

void printMap(const T& map, const std::map<Position, Direction>& moves)
{
   for (auto row = 0; row < map.size(); ++row)
   {
      for (auto col = 0; col < map[row].size(); ++col)
      {
         auto it = moves.find(Position(row, col));
         if (it != moves.end())
         {
            switch(it->second)
            {
               case Direction::UP: std::cout << "^"; break;
               case Direction::RIGHT: std::cout << ">"; break;
               case Direction::DOWN: std::cout << "v"; break;
               case Direction::LEFT: std::cout << "<"; break;
            }
         }
         else
         {
            switch(map[row][col])
            {
               case Type::EDGE: std::cout << '@'; break;
               case Type::OPEN: std::cout << '.'; break;
               case Type::WALL: std::cout << '#'; break;
               case Type::OOB:  std::cout << ' '; break;
            }
         }
      }
      std::cout << "\n";
   }
}

void part1(T& input, std::string& movement)
{
   Player p;
   std::map<Position, Direction> moves;
   
   // Find the top left most open space and place player there
   for(auto col=1; col<input[1].size(); ++col)
   {
      if (input[1][col] == Type::OPEN)
      {
         p.row = 1;
         p.col = col;
         break;
      }
   }

   // Follow instructions until the end
   auto pos = 0;
   while (pos < movement.length())
   {
      if (movement[pos] != 'L' && movement[pos] != 'R')
      {
         auto startPos = pos;
         while(pos < movement.length() && movement[pos] != 'L' && movement[pos] != 'R') ++pos;

         uint32_t numMoves = 0;
         if (startPos == pos)
         {
            numMoves = std::stoul(movement.substr(startPos, 1));
         }
         else
         {
            numMoves = std::stoul(movement.substr(startPos, pos - startPos));
         }

         // Move the number of spaces
         while(numMoves--)
         {
            auto it = moves.find(Position(p.row, p.col));
            if (it != moves.end())
            {
               it->second = p.lookDirection;
            }
            else
            {
               moves.insert({Position(p.row, p.col), p.lookDirection});
            }
            if (p.lookDirection == Direction::UP)
            {
               if (input[p.row - 1][p.col] == Type::EDGE)
               {
                  for (auto row = input.size()-1; row >= 0; --row)
                  {
                     if (input[row][p.col] == Type::WALL) break; // Skip if a wall encountered first
                     if (input[row][p.col] == Type::OPEN)
                     {
                        p.row = row;
                        break;
                     }
                  }
               }
               else if (input[p.row - 1][p.col] == Type::OPEN)
               {
                  p.row = p.row - 1;
               }
            }
            else if(p.lookDirection == Direction::RIGHT)
            {
               if (input[p.row][p.col + 1] == Type::EDGE)
               {
                  for (auto col = 0; col < input[p.row].size(); ++col)
                  {
                     if (input[p.row][col] == Type::WALL) break; // Skip if a wall encountered first
                     if (input[p.row][col] == Type::OPEN)
                     {
                        p.col = col;
                        break;
                     }
                  }
               }
               else if (input[p.row][p.col + 1] == Type::OPEN)
               {
                  p.col = p.col + 1;
               }
            }
            else if (p.lookDirection == Direction::DOWN)
            {
               if (input[p.row + 1][p.col] == Type::EDGE)
               {
                  for (auto row = 0; row < input.size(); ++row)
                  {
                     if (input[row][p.col] == Type::WALL) break; // Skip if a wall encountered first
                     if (input[row][p.col] == Type::OPEN)
                     {
                        p.row = row;
                        break;
                     }
                  }
               }
               else if (input[p.row + 1][p.col] == Type::OPEN)
               {
                  p.row = p.row + 1;
               }
            }
            else
            {
               if (input[p.row][p.col - 1] == Type::EDGE)
               {
                  for (auto col = input[p.row].size() - 1; col >= 0; --col)
                  {
                     if (input[p.row][col] == Type::WALL) break; // Skip if a wall encountered first
                     if (input[p.row][col] == Type::OPEN)
                     {
                        p.col = col;
                        break;
                     }
                  }
               }
               else if (input[p.row][p.col - 1] == Type::OPEN)
               {
                  p.col = p.col - 1;
               }
            }
         }
      }
      else
      {
         p.rotate(movement[pos]);
         ++pos;
      }
   }

   auto result = 1000 * p.row + 4 * p.col + static_cast<uint32_t>(p.lookDirection);
   std::cout << result << "\n";
}

void part2(T& input, std::string& movement)
{
   Player p;
   std::map<Position, Direction> moves;
   
   // Find the top left most open space and place player there
   for(auto col=1; col<input[1].size(); ++col)
   {
      if (input[1][col] == Type::OPEN)
      {
         p.row = 1;
         p.col = col;
         break;
      }
   }

   // Follow instructions until the end
   auto pos = 0;
   while (pos < movement.length())
   {
      if (movement[pos] != 'L' && movement[pos] != 'R')
      {
         auto startPos = pos;
         while(pos < movement.length() && movement[pos] != 'L' && movement[pos] != 'R') ++pos;

         uint32_t numMoves = 0;
         if (startPos == pos)
         {
            numMoves = std::stoul(movement.substr(startPos, 1));
         }
         else
         {
            numMoves = std::stoul(movement.substr(startPos, pos - startPos));
         }

         // Move the number of spaces
         while(numMoves--)
         {
            auto it = moves.find(Position(p.row, p.col));
            if (it != moves.end())
            {
               it->second = p.lookDirection;
            }
            else
            {
               moves.insert({Position(p.row, p.col), p.lookDirection});
            }
            if (p.lookDirection == Direction::UP)
            {
            }
            else if(p.lookDirection == Direction::RIGHT)
            {
            }
            else if (p.lookDirection == Direction::DOWN)
            {
            }
            else
            {
            }
         }
      }
      else
      {
         p.rotate(movement[pos]);
         ++pos;
      }
   }
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
   std::string movement;
   parseInputFile(part1File, vals, movement);
   part1(vals, movement);

   vals.clear();
   movement.clear();

   parseInputFile(part2File, vals, movement);
   part2(vals, movement);
   
   return 0;
}