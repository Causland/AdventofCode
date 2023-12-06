#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>

struct Point
{
   Point() {}
   Point(const int32_t x, const int32_t y) : x(x), y(y) {};
   bool operator<(const Point& p) const { return x < p.x || x == p.x && y < p.y; }

   uint32_t x{0};
   uint32_t y{0};
};

struct PartNum
{
   PartNum(const uint32_t& num,
           const Point& p, 
           uint32_t width) : partNum(num), p(p), numWidth(width) {}

   uint32_t partNum;
   Point p;
   uint32_t numWidth;
};

struct Symbol
{
   Symbol(const Point& p, const char& c='.') : p(p), c(c) {}

   bool operator<(const Symbol& s) const { return p.x < s.p.x || p.x == s.p.x && p.y < s.p.y; }

   Point p;
   char c;
   mutable std::vector<uint32_t> pNums;
};

struct EngineSchematic
{
   std::vector<PartNum> partNums;
   std::set<Symbol> symbols;
};

typedef EngineSchematic T;

bool isNumber(const char& c) { return (c >= 48) && (c <= 57); }

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string strVal;

   uint32_t i{0};
   while(std::getline(f, strVal))
   {
      for (auto j=0; j<strVal.size(); ++j)
      {
         if (strVal[j] == '.') 
         {
            continue;
         }
         else if (isNumber(strVal[j]))
         {
            uint32_t w{1};
            while(j+w<strVal.size() && isNumber(strVal[j+w])) ++w;
            vals.partNums.emplace_back(std::stoul(strVal.substr(j,w)), Point(j,i), w);
            j+=w-1;
         }
         else
         {
            vals.symbols.insert(Symbol(Point(j,i), strVal[j]));
         }
      }
      ++i;
   }
   f.close();
}

bool checkAdj(const PartNum& pNum, std::set<Symbol>& symbols)
{
   bool found{false};

   // Check left
   if (pNum.p.x > 0)
   {
      if (pNum.p.y > 0)
      {
         auto it{symbols.find(Symbol(Point(pNum.p.x-1, pNum.p.y-1)))};
         if (it != symbols.end())
         {
            it->pNums.push_back(pNum.partNum);
            found = true;
         }
      }
      auto it{symbols.find(Symbol(Point(pNum.p.x-1, pNum.p.y)))};
      if (it != symbols.end())
      {
         it->pNums.push_back(pNum.partNum);
         found = true;
      }
      it = symbols.find(Symbol(Point(pNum.p.x-1, pNum.p.y+1)));
      if (it != symbols.end())
      {
         it->pNums.push_back(pNum.partNum);
         found = true;
      }
   }

   // Check top
   if (pNum.p.y > 0)
   {
      for (auto i=0; i<pNum.numWidth; ++i)
      {
         auto it{symbols.find(Symbol(Point(pNum.p.x+i, pNum.p.y-1)))};
         if (it != symbols.end())
         {
            it->pNums.push_back(pNum.partNum);
            found = true;
         }
      }
   }

   // Check right
   if (pNum.p.y > 0)
   {
      auto it{symbols.find(Symbol(Point(pNum.p.x+pNum.numWidth, pNum.p.y-1)))};
      if (it != symbols.end())
      {
         it->pNums.push_back(pNum.partNum);
         found = true;
      }
   }
   auto it{symbols.find(Symbol(Point(pNum.p.x+pNum.numWidth, pNum.p.y)))};
   if (it != symbols.end())
   {
      it->pNums.push_back(pNum.partNum);
      found = true;
   }
   it = symbols.find(Symbol(Point(pNum.p.x+pNum.numWidth, pNum.p.y+1)));
   if (it != symbols.end())
   {
      it->pNums.push_back(pNum.partNum);
      found = true;
   }

   // Check bottom
   for (auto i=0; i<pNum.numWidth; ++i)
   {
      auto it{symbols.find(Symbol(Point(pNum.p.x+i, pNum.p.y+1)))};
      if (it != symbols.end())
      {
         it->pNums.push_back(pNum.partNum);
         found = true;
      }
   }   

   return found;
}

void part1(T input)
{
   std::cout << std::accumulate(input.partNums.begin(), input.partNums.end(), 0,
                               [&input](int sum, const PartNum& a) 
                                 { return checkAdj(a, input.symbols) ? sum+a.partNum : sum; })
             << "\n";
}

void part2(T input)
{
   for (auto& pNum : input.partNums) checkAdj(pNum, input.symbols);

   uint32_t sum{0};
   for (auto& symbol : input.symbols)
   {
      if (symbol.c == '*' && symbol.pNums.size() == 2)
      {
         sum += std::accumulate(symbol.pNums.begin(), symbol.pNums.end(), 1,
                                [](const uint32_t& a, const uint32_t& b) { return a * b; });
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