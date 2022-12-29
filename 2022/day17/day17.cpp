#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>

typedef std::string T;

enum class Type
{
   AIR,
   ROCK,
   WALL,
};

struct Point
{
   Point() {}
   Point(int32_t x, int32_t y) : x(x), y(y) {}
   Point(std::initializer_list<int32_t> l) : x(*l.begin()), y(*(l.begin()+1)) {}

   Point operator-(const Point& p) { return Point(p.x - this->x, p.y - this->y); };
   bool operator<(const Point& p) const { return x < p.x || x == p.x && y < p.y; }

   int32_t x{0};
   int32_t y{0};
};

struct Shape
{
   Shape(std::initializer_list<Point> l) : 
            points(l),
            w(std::max(l, [](auto& a, auto&b){ return a.x < b.x; }).x - std::min(l, [](auto& a, auto&b){ return a.x < b.x; }).x + 1),
            h(std::max(l, [](auto& a, auto&b){ return a.y < b.y; }).y - std::min(l, [](auto& a, auto&b){ return a.y < b.y; }).y + 1) {}

   bool checkIntersect(Point position, Point posTarget)
   {
      // Find point relative to target and origin
      Point posShifted = position - posTarget;

      // Return true if any points equal this new shifted point
      if (points.find(posShifted) != points.end())
      {
         return true;
      }
      return false;
   }

   std::set<Point> points; 
   uint32_t w;
   uint32_t h;
};

static const std::array<Shape, 5> pieces{
   Shape({{0,0}, {1,0}, {2,0}, {3,0}         }),
   Shape({{0,1}, {1,1}, {2,1}, {1,0}, {1,2}  }),
   Shape({{0,0}, {1,0}, {2,0}, {2,1}, {2,2}  }),
   Shape({{0,0}, {0,1}, {0,2}, {0,3}         }),
   Shape({{0,0}, {1,0}, {1,1}, {0,1}         })
};

void parseInputFile(std::string filename, T& val)
{
   std::fstream f(filename);
   f >> val;
   f.close();
}

void printChamber(std::vector<std::vector<Type>>& chamber, Shape& s, Point& position)
{
   for (int32_t j=chamber[0].size()-1; j>=0; --j)
   {
      for (int32_t i=0; i<chamber.size(); ++i)
      {
         if (s.checkIntersect(position, {i,j})) std::cout << "@";
         else if (chamber[i][j] == Type::WALL) std::cout << "|";
         else if (chamber[i][j] == Type::ROCK) std::cout << "#";
         else std::cout << ".";
      }
      std::cout << "\n";
   }
   std::cout << "\n";
}

void expandChamber(std::vector<std::vector<Type>>& chamber, uint32_t addHeight)
{
   // Resize the two walls
   chamber[0].resize(chamber[0].size() + addHeight, Type::WALL);
   chamber[chamber.size()-1].resize(chamber[chamber.size()-1].size() + addHeight, Type::WALL);
   // Then the middle
   for (auto i=1; i<chamber.size()-1; ++i)
   {
      chamber[i].resize(chamber[i].size() + addHeight, Type::AIR);
   }
}

uint64_t getStackHeight(T& shifts, uint64_t numPieces)
{
   uint64_t pieceIndex = 0;
   uint64_t moveIndex = 0;
   uint64_t topPieceHeight = 0;

   // Create the chamber
   std::vector<std::vector<Type>> chamber(9);

   for (auto count = 0; count < numPieces; ++count)
   {
      // Select the next shape
      Shape s = pieces[pieceIndex++ % pieces.size()];

      // Adjust the chamber to fit the piece
      if (topPieceHeight + 3 + s.h > chamber[0].size())
      {
         expandChamber(chamber, 300);
      }

      // Place piece starting position
      Point position{3, static_cast<int32_t>(topPieceHeight + 3)};

      // Move down and shift until the piece can no longer move down
      while(true)
      {
         // Shift the piece
         if (shifts[moveIndex++ % shifts.length()] == '>')
         {
            ++position.x; // Temporary shift to check
            for (auto& p : s.points)
            {
               if (chamber[p.x + position.x][p.y + position.y] != Type::AIR)
               {
                  --position.x; // Restore previous
                  break;
               }
            }
         }
         else
         {
            --position.x; // Temporary shift to check
            for (auto& p : s.points)
            {
               if (chamber[p.x + position.x][p.y + position.y] != Type::AIR)
               {
                  ++position.x; // Restore previous
                  break;
               }
            }
         }

         // Move piece down
         bool blocked = false;
         --position.y; // Temporary shift to check
         for (auto& p : s.points)
         {
            if (position.y + p.y < 0 || chamber[p.x + position.x][p.y + position.y] != Type::AIR)
            {
               ++position.y; // Restore previous
               blocked = true;
               break;
            }
         }
         if (blocked) break;
      }

      // Make piece permanent
      for (auto& p : s.points)
      {
         chamber[position.x + p.x][position.y + p.y] = Type::ROCK;
      }

      // Adjust height
      if (position.y + s.h > topPieceHeight) topPieceHeight = position.y + s.h;
   }
   return topPieceHeight;
}

void findLoopInfo(T& shifts, uint64_t numPieces, uint64_t& startLoopPieceIndex, uint64_t& endLoopPieceIndex, uint64_t& startLoopShiftIndex, uint64_t& endLoopShiftIndex)
{
   uint64_t pieceIndex = 0;
   uint64_t moveIndex = 0;
   uint64_t topPieceHeight = 0;

   startLoopPieceIndex = 0;
   startLoopShiftIndex = 0;
   endLoopPieceIndex = 0;
   endLoopShiftIndex = 0;

   // Create the chamber
   std::vector<std::vector<Type>> chamber(9);

   for (auto count = 0; count < numPieces; ++count)
   {
      // Select the next shape
      Shape s = pieces[pieceIndex++ % pieces.size()];

      // Adjust the chamber to fit the piece
      if (topPieceHeight + 3 + s.h > chamber[0].size())
      {
         expandChamber(chamber, 300);
      }

      // Place piece starting position
      Point position{3, static_cast<int32_t>(topPieceHeight + 3)};

      // Move down and shift until the piece can no longer move down
      while(true)
      {
         // Shift the piece
         if (shifts[moveIndex++ % shifts.length()] == '>')
         {
            ++position.x; // Temporary shift to check
            for (auto& p : s.points)
            {
               if (chamber[p.x + position.x][p.y + position.y] != Type::AIR)
               {
                  --position.x; // Restore previous
                  break;
               }
            }
         }
         else
         {
            --position.x; // Temporary shift to check
            for (auto& p : s.points)
            {
               if (chamber[p.x + position.x][p.y + position.y] != Type::AIR)
               {
                  ++position.x; // Restore previous
                  break;
               }
            }
         }

         // Move piece down
         bool blocked = false;
         --position.y; // Temporary shift to check
         for (auto& p : s.points)
         {
            if (position.y + p.y < 0 || chamber[p.x + position.x][p.y + position.y] != Type::AIR)
            {
               ++position.y; // Restore previous
               blocked = true;
               break;
            }
         }
         if (blocked) break;
      }

      // Make piece permanent
      for (auto& p : s.points)
      {
         chamber[position.x + p.x][position.y + p.y] = Type::ROCK;
      }

      // Check for complete row in last 10 rows
      for (auto y = chamber[0].size() - 1; y >= std::max(0, static_cast<int>(chamber[0].size() - 10)); --y)
      {
         bool fullRow = true;
         for (auto x = 1; x < chamber.size() - 1; ++x)
         {
            if (chamber[x][y] != Type::ROCK)
            {
               fullRow = false;
               break;
            }
         }
         if (fullRow)
         {
            if (startLoopPieceIndex == 0)
            {
               startLoopPieceIndex = pieceIndex - 1;
               startLoopShiftIndex = moveIndex - 1;
            }
            else
            {
               if (startLoopPieceIndex % pieces.size() == (pieceIndex - 1) % pieces.size() &&
                   startLoopShiftIndex % shifts.size() == (moveIndex - 1) % shifts.size())
               {
                  endLoopPieceIndex = pieceIndex - 1;
                  endLoopShiftIndex = moveIndex - 1;
                  return;
               }
            }
         }
      }

      // Adjust height
      if (position.y + s.h > topPieceHeight) topPieceHeight = position.y + s.h;
   }
}

void part1(T input)
{
   std::cout << getStackHeight(input, 2022) << "\n";
}

void part2(T input)
{
   uint64_t startLoopPieceIndex{0};
   uint64_t endLoopPieceIndex{0};
   uint64_t startLoopShiftIndex{0}; 
   uint64_t endLoopShiftIndex{0};
   findLoopInfo(input, 10000000, startLoopPieceIndex, endLoopPieceIndex, startLoopShiftIndex, endLoopShiftIndex);
   
   if (!startLoopPieceIndex && !startLoopShiftIndex && !endLoopPieceIndex && !endLoopShiftIndex)
   {
      std::cout << "Unable to find loop info" << "\n";
      return;
   }

   // Calculate loop height
   uint64_t loopHeight = getStackHeight(input, endLoopPieceIndex) - 
                         getStackHeight(input, startLoopPieceIndex);

   // Calculate number of loops in the full input
   uint64_t numLoops = (1000000000000 - startLoopPieceIndex) / (endLoopPieceIndex - startLoopPieceIndex); 
   uint64_t numRemain = (1000000000000 - startLoopPieceIndex) % (endLoopPieceIndex - startLoopPieceIndex);

   // Add together the loop heights plus the remainders
   std::cout << getStackHeight(input, startLoopPieceIndex + numRemain) + loopHeight * numLoops << "\n";
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