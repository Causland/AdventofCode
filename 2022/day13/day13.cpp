#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

enum class ElementType
{
   LIST,
   INT,
};

struct Element
{
   ElementType type{ElementType::LIST};
   std::vector<Element> l;
   uint32_t i;
   Element* parent{nullptr};
};

typedef std::vector<Element> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string line;

   Element* e = nullptr;
   while(std::getline(f, line))
   {
      if (line.empty()) continue;
      
      vals.emplace_back();
      e = &(vals.back());

      bool firstElement = true;
      for (auto i = 0; i < line.size(); ++i)
      {
         if (line[i] == '[')
         {
            if (!firstElement)
            {
               e->type = ElementType::LIST;
               e->l.emplace_back();
               e->l.back().parent = e;
               e = &(e->l.back());
            }
         }
         else if (line[i] == ']')
         {
            e = e->parent;
         }
         else if (line[i] == ',')
         {
            continue;
         }
         else // Must be a number, find the ',' or stop before next ']'
         {
            auto posComma = line.find(',', i);
            auto posCloseBracket = line.find(']', i);

            std::string numStr;
            auto shift = 0;
            if (posComma < posCloseBracket)
            {
               numStr = line.substr(i, posComma - i);
               shift = posComma;
            }
            else
            {
               numStr = line.substr(i, posCloseBracket - i);
               shift = posCloseBracket - 1;
            }

            e->l.emplace_back();
            e->l.back().type = ElementType::INT;
            e->l.back().i = std::stoul(numStr);
            e->l.back().parent = e;
            i = shift;
         }
         firstElement = false;
      }
   }
   f.close();
}

void printElement(Element& element)
{
   if (element.type == ElementType::LIST)
   {
      std::cout << "[";
      bool firstElement = true;
      for (Element& e : element.l)
      {
         if (!firstElement)
         {
            std::cout << ",";
         }
         printElement(e);
         firstElement = false;
      }
      std::cout << "]";
   }
   else
   {
      std::cout << element.i;
   }
}

void printPairs(T& input)
{
   bool pair = false;
   for (auto& list : input)
   {
      printElement(list);
      std::cout << "\n";
      if (pair == true)
      {
         pair = false;
         std::cout << "\n";
      }
      else
      {
         pair = true;
      }
   }
}

enum class Order
{
   CORRECT,
   SAME,
   WRONG,
};

Order comparePairs(Element& left, Element& right)
{
   auto i = 0;
   while(i < left.l.size() && i < right.l.size())
   {
      if (left.l[i].type == ElementType::LIST && right.l[i].type == ElementType::LIST)
      {
         auto result = comparePairs(left.l[i], right.l[i]);
         if (result != Order::SAME)
         {
            return result;
         }
      }
      else if (left.l[i].type == ElementType::INT && right.l[i].type == ElementType::INT)
      {
         if (left.l[i].i > right.l[i].i)
         {
            return Order::WRONG;
         }
         else if (left.l[i].i < right.l[i].i)
         {
            return Order::CORRECT;
         }
      }
      else
      {
         // Convert one to list and try again
         bool changedLeft = true;
         if (left.l[i].type == ElementType::INT)
         {
            left.l[i].type = ElementType::LIST;
            left.l[i].l.emplace_back();
            left.l[i].l.back().type = ElementType::INT;
            left.l[i].l.back().i = left.l[i].i;
         }
         else
         {
            right.l[i].type = ElementType::LIST;
            right.l[i].l.emplace_back();
            right.l[i].l.back().type = ElementType::INT;
            right.l[i].l.back().i = right.l[i].i;
            changedLeft = false;
         }
         auto result = comparePairs(left.l[i], right.l[i]);
         if (changedLeft)
         {
            left.l[i].type = ElementType::INT;
            left.l[i].l.clear();
         }
         else
         {
            right.l[i].type = ElementType::INT;
            right.l[i].l.clear();
         }
         if (result != Order::SAME)
         {
            return result;
         }
      }
      ++i;
   }
   if (i != left.l.size() && i == right.l.size()) // Right side is shorter than the left
   {
      return Order::WRONG; 
   }
   else if (i == left.l.size() && i != right.l.size())
   {
      return Order::CORRECT;
   }
   else
   {
      return Order::SAME;
   }
}

void part1(T& input)
{
   auto sum = 0;
   for (auto i=1; i<=input.size(); i+=2)
   {
      if (comparePairs(input[i-1], input[i]) != Order::WRONG)
      {
         sum += i/2 + 1;
      }
   }
   std::cout << sum << "\n";
}

void part2(T input)
{
   // Create temp parent (used as identifier)
   Element p;
   // Create the divider lists
   Element div2;
   div2.l.emplace_back();
   div2.l.back().l.emplace_back();
   div2.l.back().l.back().type = ElementType::INT;
   div2.l.back().l.back().i = 2;
   div2.l.back().l.back().parent = &p;
   input.push_back(div2);
   Element div6;
   div6.l.emplace_back();
   div6.l.back().l.emplace_back();
   div6.l.back().l.back().type = ElementType::INT;
   div6.l.back().l.back().i = 6;
   div6.l.back().l.back().parent = &p;
   input.push_back(div6);

   std::sort(input.begin(), input.end(), [](auto& a, auto& b){
      auto result = comparePairs(a, b);
      if (result == Order::CORRECT) return true;
      else return false;
   });

   printPairs(input);
   // Find the index of 2 and 6 using the parent identifier
   auto six = 0;
   auto two = 0;
   for (auto i=1; i<=input.size(); ++i)
   {
      if (!input[i-1].l.empty())
      {
         auto midBack = input[i-1].l[0];
         if (midBack.l.size() == 1 && midBack.l[0].type == ElementType::INT)
         {
            if (midBack.l[0].parent == &p)
            {
               // We've found one of the markers. Determine which
               if (midBack.l[0].i == 2)
               {
                  two = i;
               }
               else
               {
                  six = i;
               }
            }
         }
      }
      if (six != 0 && two != 0) break;
   }
   std::cout << two*six << "\n";
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