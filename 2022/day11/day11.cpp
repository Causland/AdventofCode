#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>
#include <functional>
#include <cstdio>
#include <sstream>
#include <cmath>

static uint32_t itemId{0};

struct Item
{
   Item(const uint64_t& w) : worryLevel(w) {};
   Item(const Item& i) : worryLevel(i.worryLevel) {};
   Item() = delete;

   uint64_t worryLevel{0};
};

struct Monkey
{
   void performOp(Item& i)
   {
      i.worryLevel = operation(i);
   }

   void throwItem(Monkey& m, Item& i)
   {
      m.catchItem(i);
   }

   void getBored(Item& i)
   {
      i.worryLevel /= 3;
   }

   void catchItem(Item& i)
   {
      items.emplace_back(i);
   }

   uint64_t numInspections{0};
   uint32_t divisor{1};
   std::vector<Item> items;
   std::function<uint64_t(const Item&)> operation;
   std::function<Monkey&(std::vector<Monkey>&, const Item&)> test;
};

typedef std::vector<Monkey> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string block;
   std::string line;

   while(std::getline(f, line))
   {
      if (f.eof())
      {
         block += line + "\n";
      }
      if (line.empty() || f.eof())
      {
         vals.emplace_back();
         std::stringstream ss{block};
         std::string subLine;
         for (auto i=0; i<4; ++i)
         {
            std::getline(ss, subLine);
            if (i == 0) continue;
            else if (i == 1)
            {
               auto posColon = subLine.find(':');
               auto numsStr = subLine.substr(posColon + 2);
               while(!numsStr.empty())
               {
                  auto posComma = numsStr.find(',');
                  vals.back().items.push_back(std::stoul(numsStr.substr(0, posComma)));
                  if (posComma == std::string::npos) numsStr.clear();
                  else numsStr.erase(0, posComma + 2);
               }
            }
            else if (i == 2)
            {
               auto posOld = subLine.find("old");
               auto opStr = subLine.substr(posOld + 4);
               auto op = opStr.front();
               opStr.erase(0,2); // Remove operation character and space
               if (opStr == "old")
               {
                  if (op == '+')
                  {
                     vals.back().operation = [](const Item& i) -> uint64_t { return i.worryLevel + i.worryLevel; };
                  }
                  else
                  {
                     vals.back().operation = [](const Item& i) -> uint64_t { return i.worryLevel * i.worryLevel; };
                  }
               }
               else
               {
                  uint64_t num = std::stoull(opStr);
                  if (op == '+')
                  {
                     vals.back().operation = [num](const Item& i) -> uint64_t { return i.worryLevel + num; };
                  }
                  else
                  {
                     vals.back().operation = [num](const Item& i) -> uint64_t { return i.worryLevel * num; };
                  }
               }
            }
            else if (i == 3)
            {
               auto lastSpacePos = subLine.find_last_of(' ');
               auto divisorStr = subLine.substr(lastSpacePos + 1);
               uint32_t divisor = std::stoul(divisorStr);
               vals.back().divisor = divisor;
               std::getline(ss, subLine);
               lastSpacePos = subLine.find_last_of(' ');
               auto trueMonkeyStr = subLine.substr(lastSpacePos + 1);
               uint32_t trueMonkey = std::stoul(trueMonkeyStr);
               std::getline(ss, subLine);
               lastSpacePos = subLine.find_last_of(' ');
               auto falseMonkeyStr = subLine.substr(lastSpacePos + 1);
               uint32_t falseMonkey = std::stoul(falseMonkeyStr);

               vals.back().test = [divisor, trueMonkey, falseMonkey](std::vector<Monkey>& monkeys, const Item& i) -> Monkey& 
                                       { return (i.worryLevel % divisor == 0) ? monkeys[trueMonkey] : monkeys[falseMonkey]; };
            }
            else
            {
               std::cout << "We really shouldn't get here\n";
            }
         }
         block.clear();
      }
      else
      {
         block += line + "\n";
      }
   }
   f.close();
}

void printMonkeys(T& monkeys)
{
   for (const auto& monkey : monkeys)
   {
      if (monkey.items.empty())
      {
         std::cout << "-----------------------------------";
      }
      for (const auto & item : monkey.items)
      {
         std::cout << item.worryLevel << " ";
      }
      std::cout << "\n";
   }
}

void part1(T input)
{
   for (auto i=0; i<20; ++i)
   {
      for (auto& monkey : input)
      {
         for (auto& item : monkey.items)
         {
            monkey.performOp(item);
            monkey.getBored(item);
            auto& catchMonkey = monkey.test(input, item);
            monkey.throwItem(catchMonkey, item);
            ++monkey.numInspections;
         }
         monkey.items.clear();
      }
   }

   std::sort(input.begin(), input.end(), [](const auto& a, const auto& b){ return a.numInspections > b.numInspections; });
   std::cout << input[0].numInspections * input[1].numInspections << "\n";
}

void part2(T input)
{
   uint64_t lcd{1};
   for (auto& monkey : input)
   {
      lcd *= monkey.divisor;
   }

   for (auto i=0; i<10000; ++i)
   {
      for (auto& monkey : input)
      {
         for (auto& item : monkey.items)
         {
            monkey.performOp(item);
            item.worryLevel %= lcd;
            auto& catchMonkey = monkey.test(input, item);
            monkey.throwItem(catchMonkey, item);
            ++monkey.numInspections;
         }
         monkey.items.clear();
      }
   }
   std::sort(input.begin(), input.end(), [](const auto& a, const auto& b){ return a.numInspections > b.numInspections; });
   std::cout << input[0].numInspections * input[1].numInspections << "\n";
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