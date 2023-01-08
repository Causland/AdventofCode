#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>
#include <queue>

enum class Operation
{
   ADD,
   SUB,
   MULT,
   DIV,
   NONE,
};

char opToChar(Operation op)
{
   switch(op)
   {
      case Operation::ADD: return '+';
      case Operation::SUB: return '-';
      case Operation::MULT: return '*';
      case Operation::DIV: return '/';
      default: return '!';
   }
}

struct Monkey
{
   Monkey(const std::string& id) : id(id) {};
   bool operator<(const Monkey& m) const { return this->id < m.id; };

   bool calcMonkeyNum()
   {
      if (isMonkeyNumKnown) return true;
      if (operand1Monkey->monkeyNum != INT32_MAX && operand2Monkey->monkeyNum != INT32_MAX)
      {
         switch(op)
         {
            case Operation::ADD: monkeyNum = operand1Monkey->monkeyNum + operand2Monkey->monkeyNum; break;
            case Operation::SUB: monkeyNum = operand1Monkey->monkeyNum - operand2Monkey->monkeyNum; break;
            case Operation::MULT: monkeyNum = operand1Monkey->monkeyNum * operand2Monkey->monkeyNum; break;
            case Operation::DIV: monkeyNum = operand1Monkey->monkeyNum / operand2Monkey->monkeyNum; break;
            default: return false;
         }
         isMonkeyNumKnown = true;

         // Propagate through dependencies
         for (auto& m : dependencies)
         {
            m->calcMonkeyNum();
         }

         return true;
      }
      return false;
   };

   const std::string id;

   Operation op{Operation::NONE};
   Monkey* operand1Monkey{nullptr};
   Monkey* operand2Monkey{nullptr};

   std::vector<Monkey*> dependencies;

   bool isMonkeyNumKnown{false};
   int64_t monkeyNum{INT32_MAX};
};

typedef std::vector<Monkey> T;

void copyMonkeys(const T& sourceMonkeys, T& destMonkeys)
{
   for (auto& m : sourceMonkeys)
   {
      destMonkeys.emplace_back(m.id);
      destMonkeys.back().isMonkeyNumKnown = m.isMonkeyNumKnown;
      destMonkeys.back().monkeyNum = m.monkeyNum;
      destMonkeys.back().op = m.op;
   }

   for (auto& m : destMonkeys)
   {
      if (m.monkeyNum != INT32_MAX) continue;
      auto it = std::find_if(sourceMonkeys.begin(), sourceMonkeys.end(), [m](auto& srcM){ return m.id == srcM.id; });
      auto opMonkey1Name = it->operand1Monkey->id;
      auto opMonkey2Name = it->operand2Monkey->id;
      m.operand1Monkey = &(*std::find_if(destMonkeys.begin(), destMonkeys.end(), [opMonkey1Name](auto& destM){ return opMonkey1Name == destM.id; }));
      m.operand2Monkey = &(*std::find_if(destMonkeys.begin(), destMonkeys.end(), [opMonkey2Name](auto& destM){ return opMonkey2Name == destM.id; }));
      m.operand1Monkey->dependencies.push_back(&m);
      m.operand2Monkey->dependencies.push_back(&m);
   }
}

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);

   std::vector<std::string> lines;
   std::string line;
   while(std::getline(f, line)) lines.push_back(line);
   f.close();

   // Add all monkeys to the set
   for (auto& line : lines)
   {
      auto posColon = line.find(':');
      vals.emplace_back(line.substr(0, posColon));
   }

   // Populate all info about the monkeys
   for (auto& line : lines)
   {
      auto posColon = line.find(':');
      auto it = std::find_if(vals.begin(), vals.end(), [line, posColon](auto& m){ return m.id == line.substr(0, posColon); });
      line = line.substr(posColon + 2);

      // If number is first, this is the monkeyVal
      auto posSpace = line.find(' ');
      if (posSpace == std::string::npos)
      {
         (*it).monkeyNum = std::stoi(line);
         (*it).isMonkeyNumKnown = true;
         continue;
      }

      // This means we are looking for two monkeys and an op
      (*it).operand1Monkey = &(*std::find_if(vals.begin(), vals.end(), [line, posSpace](auto& m){ return m.id == line.substr(0, posSpace); }));
      (*it).operand1Monkey->dependencies.push_back(&(*it));
      line = line.substr(posSpace + 1);
      char op = line.front();
      switch(op)
      {
         case '+': (*it).op = Operation::ADD; break;
         case '-': (*it).op = Operation::SUB; break;
         case '*': (*it).op = Operation::MULT; break;
         case '/': (*it).op = Operation::DIV; break;
         default: std::cerr << "Operation " << op << " is not valid\n";  
      }
      line = line.substr(2);
      (*it).operand2Monkey = &(*std::find_if(vals.begin(), vals.end(), [line](auto& m){ return m.id == line; }));
      (*it).operand2Monkey->dependencies.push_back(&(*it));
   }
}


void findPath(const Monkey* m, std::vector<const Monkey*> path, std::vector<const Monkey*>& pathout)
{
   if (m->id == "root")
   {
      path.push_back(m);
      pathout = path;
      return;
   }

   if (!pathout.empty()) return; // We must have found the path

   path.push_back(m);

   for (auto& nextMonkey : m->dependencies)
   {
      findPath(nextMonkey, path, pathout);
   }
}

std::string getEquation(std::vector<const Monkey*>& path)
{
   std::string out;
   for(auto i=0; i < path.size(); ++i)
   {
      if (path[i]->id == "humn")
      {
         out += "x";
         continue;
      }
      if (path[i]->id == "root")
      {
         out.push_back('=');
         if (path[i-1] == path[i]->operand1Monkey)
         {
            out += std::to_string(path[i]->operand2Monkey->monkeyNum);
         }
         else
         {
            out += std::to_string(path[i]->operand1Monkey->monkeyNum);
         }
         continue;
      }
      // Figure out if its the left operand or right
      if (path[i-1] == path[i]->operand1Monkey)
      {
         // Add to right side of expression
         out.insert(0, "(");
         out.push_back(opToChar(path[i]->op));
         out += std::to_string(path[i]->operand2Monkey->monkeyNum);
         out.push_back(')');
      }
      else
      {
         // Add to left side of expression
         out.push_back(')');
         out.insert(0, 1, opToChar(path[i]->op));
         out.insert(0, std::to_string(path[i]->operand1Monkey->monkeyNum));
         out.insert(0, "(");
      }
   }
   return out;
}

void part1(T& input)
{
   // Do calculations to find all monkey values
   for (auto& m : input)
   {
      m.calcMonkeyNum();
   }

   auto it = std::find_if(input.begin(), input.end(), [](auto& m){ return m.id == "root"; });
   std::cout << it->monkeyNum << "\n";
}

void part2(T& input)
{
   for(auto& m : input)
   {
      m.calcMonkeyNum();
   }

   auto it = std::find_if(input.begin(), input.end(), [](auto& m){ return m.id == "humn"; });
   std::vector<const Monkey*> path;
   findPath(&(*it), path, path);

   std::string eq = getEquation(path);
   auto posEq = eq.find('=');
   std::string eq_lhs = eq.substr(0, posEq);
   std::string eq_rhs = eq.substr(posEq + 1);

   std::string command{"python3 -c 'from sympy.abc import x; from sympy import Eq, solve; "
                       "eqn = Eq(" + eq_lhs + ", " + eq_rhs + "); print(solve(eqn, x)[0])'"};
   system(command.c_str());

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