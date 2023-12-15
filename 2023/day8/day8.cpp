#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <map>

struct Node
{
   Node(const std::string& name) : name(name) {}

   std::string name;
   Node* left;
   Node* right;
};

typedef std::pair<std::string, std::map<std::string, Node*>> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string& instructions{vals.first};
   auto& map{vals.second};

   // Get the directions
   std::getline(f, instructions);

   // Create the map
   std::string line;
   while(std::getline(f, line))
   {
      if (line.empty()) continue;

      char nodeName[4];
      char leftName[4];
      char rightName[4];

      sscanf(line.c_str(), "%3s = (%3s, %3s)", &nodeName[0], &leftName[0], &rightName[0]);

      const auto it{map.find(nodeName)};
      Node* n;
      if (it == map.end())
      {
         n = new Node(nodeName);
         map.insert({nodeName, n});
      }
      else 
      {
         n = it->second; 
      }

      if (n->left == nullptr)
      {
         const auto lit{map.find(leftName)};
         if (lit != map.end())
         {
            n->left = lit->second;
         }
         else
         {
            auto ln{new Node(leftName)};
            map.insert({leftName, ln});
            n->left = ln;
         }
      }
      
      if (n->right == nullptr)
      {
         const auto rit{map.find(rightName)};
         if (rit != map.end())
         {
            n->right = rit->second;
         }
         else
         {
            auto rn{new Node(rightName)};
            map.insert({rightName, rn});
            n->right = rn;
         }
      }
   }
   f.close();
}

void part1(T& input)
{
   const std::string& instructions{input.first};
   const auto& map{input.second};

   auto steps{0};
   std::string nodeName{"AAA"};

   bool found{false};
   while (!found)
   {
      for (const auto c : instructions)
      {
         ++steps;

         const auto n{map.find(nodeName)->second};

         if (c == 'L')
         {
            nodeName = n->left->name;
         }
         else
         {
            nodeName = n->right->name;
         }

         if (nodeName == "ZZZ")
         {
            found = true;
            break;
         }
      }
   }

   std::cout << steps << "\n";
}

void part2(T& input)
{
   const std::string& instructions{input.first};
   const auto& map{input.second};

   std::vector<std::string> nodeNames;
   for (const auto& pair : map)
   {
      if (pair.first.back() == 'A')
      {
         nodeNames.push_back(pair.first);
      }
   }

   for (auto& nodeName : nodeNames)
   {
      uint64_t steps{0};
      bool found{false};
      while (!found)
      {
         for (const auto c : instructions)
         {
            ++steps;
   
            const auto n{map.find(nodeName)->second};
   
            if (c == 'L')
            {
               nodeName = n->left->name;
            }
            else
            {
               nodeName = n->right->name;
            }

            if (nodeName.back() == 'Z') 
            {
               found = true;
               break;
            }
         }
      }
      std::cout << steps << " "; 
   }
   std::cout << "\n";
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

   //T vals2;
   //parseInputFile(part2File, vals2);
   part2(vals);
   
   return 0;
}
