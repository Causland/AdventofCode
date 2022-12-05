#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <stack>
#include <cstdio>

struct Instruction
{
   Instruction(uint32_t n, uint32_t s, uint32_t d) : num(n), src(s), dst(d) {};
   uint32_t num;
   uint32_t src;
   uint32_t dst;
};

void parseInputFile(std::string filename, std::vector<std::stack<char>>& stacks, std::vector<Instruction>& instructions)
{
   std::fstream f(filename);
   std::string line;
   std::vector<std::string> lines;

   while(std::getline(f, line))
   {
      if (line.find('1') != std::string::npos) break; // Stop at line under stacks
      lines.push_back(line); // Push to vector so I can reverse through
   } 

   // Populate the stacks
   auto rit = lines.rbegin();
   for (; rit != lines.rend(); ++rit)
   {
      auto& line = *rit;
      for (auto strIndex = 0; strIndex < line.length(); strIndex += 4)
      {
         std::string entry = line.substr(strIndex, 4);
         if (entry.find('[') == std::string::npos) continue;
         char item = entry[1]; // We know it isn't an empty spot

         auto stackPos = strIndex / 4;
         while (stackPos >= stacks.size())
         {
            stacks.emplace_back();
         }

         stacks[stackPos].push(item);
      }
   }

   std::getline(f, line); // Skip empty line

   while(std::getline(f, line))
   {
      uint32_t num = 0xFF;
      uint32_t src = 0xFF;
      uint32_t dst = 0xFF;

      sscanf(line.c_str(), "move %u from %u to %u", &num, &src, &dst);

      instructions.emplace_back(num, src, dst);
   }

   f.close();
}

void part1(std::string filename)
{
   std::vector<std::stack<char>> stacks;
   std::vector<Instruction> instructions;
   parseInputFile(filename, stacks, instructions);

   for (auto& i : instructions)
   {
      for (auto count = 0; count < i.num; ++count)
      {
         auto val = stacks[i.src - 1].top();
         stacks[i.dst - 1].push(val);
         stacks[i.src - 1].pop();
      }
   }

   std::string result;
   for (auto& stack : stacks)
   {
      result.push_back(stack.top());
   }

   std::cout << result << "\n";
}

void part2(std::string filename)
{
   std::vector<std::stack<char>> stacks;
   std::vector<Instruction> instructions;
   parseInputFile(filename, stacks, instructions);

   for (auto& i : instructions)
   {
      std::stack<char> temp;
      for (auto count = 0; count < i.num; ++count)
      {
         auto val = stacks[i.src - 1].top();
         temp.push(val);
         stacks[i.src - 1].pop();
      }

      while (!temp.empty())
      {
         auto val = temp.top();
         stacks[i.dst - 1].push(val);
         temp.pop();
      }
   }

   std::string result;
   for (auto& stack : stacks)
   {
      result.push_back(stack.top());
   }

   std::cout << result << "\n";
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

   part1(part1File);
   part2(part2File);
   
   return 0;
}