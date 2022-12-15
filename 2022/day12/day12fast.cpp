#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>
#include <cmath>

struct Node
{
   Node(const uint32_t r, const uint32_t c, const char h) : row(r), col(c), height(h) {};
   Node() = delete;

   uint32_t p_row{0};
   uint32_t p_col{0};
   uint32_t row{0};
   uint32_t col{0};
   uint32_t f{0};
   uint32_t g{0};
   uint32_t h{0};
   char height;

   bool operator<(const Node& n) const { return this->row < n.row || (this->row == n.row && this->col < n.col); };
};

struct NodeComp
{
   bool operator()(const Node* lhs, const Node* rhs) const { return lhs->row < rhs->row || (lhs->row == rhs->row && lhs->col < rhs->col); };
};

typedef std::vector<std::vector<Node>> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string line;

   auto i=0;
   while(std::getline(f, line))
   {
      vals.emplace_back();
      for(auto j=0; j<line.length(); ++j)
      {
         vals.back().emplace_back(i, j, line[j]);
      }
      ++i;
   }
   f.close();
}

void findStartAndEnd(T& map, uint32_t& startRow, uint32_t& startCol, uint32_t& endRow, uint32_t& endCol)
{
   bool foundStart = false;
   bool foundEnd = false;

   for (auto i = 0; i < map.size(); ++i)
   {
      for (auto j = 0; j < map[0].size(); ++j)
      {
         if (map[i][j].height == 'S')
         {
            startRow = i;
            startCol = j;
            map[i][j].height = 'a';
            foundStart = true;
         }
         else if(map[i][j].height == 'E')
         {
            endRow = i;
            endCol = j;
            map[i][j].height = 'z';
            foundEnd = true;
         }

         if (foundStart && foundEnd)
         {
            return;
         }
      }
   }
}

uint32_t calculateH(const uint32_t& endRow, const uint32_t& endCol, const Node& node)
{
   return ((node.row > endRow) ? node.row - endRow : endRow - node.row) +
          ((node.col > endCol) ? node.col - endCol : endCol - node.col);
}

std::set<Node> walkPath(const T& map, const uint32_t& startRow, const uint32_t& startCol, const Node& endNode)
{
   std::set<Node> path;
   Node n = endNode;
   while(n.row != startRow || n.col != startCol)
   {
      path.insert(n);
      n = map[n.p_row][n.p_col];
   }
   return path;
}

std::set<Node> findShortestPath(T& map, const uint32_t& startRow, const uint32_t& startCol, const uint32_t& endRow, const uint32_t& endCol)
{
   // Initialize the open and closed list
   std::set<Node*, NodeComp> openList;
   std::set<Node*, NodeComp> closedList;

   map[startRow][startCol].f = calculateH(endRow, endCol, map[startRow][startCol]);

   // Put the starting node on the open list
   openList.insert(&map[startRow][startCol]);

   while(!openList.empty())
   {
      // Pop node with least f off the list
      auto minF = std::min_element(openList.begin(), openList.end(), [](const auto& a, const auto& b){ return a->h + a->g < b->h + b->g; });
      auto q = *minF;
      openList.erase(minF);

      auto& row = q->row;
      auto& col = q->col;

      // Check for end
      if (row == endRow && col == endCol)
      {
         return walkPath(map, startRow, startCol, *q);
      }

      // Generate q's successors and set their parent to q
      std::vector<Node*> successors;
      if (row != 0 && map[row-1][col].height <= map[row][col].height + 1)                 successors.push_back(&map[row-1][col]);
      if (row != map.size() - 1 && map[row+1][col].height <= map[row][col].height + 1)    successors.push_back(&map[row+1][col]);
      if (col != 0 && map[row][col-1].height <= map[row][col].height + 1)                 successors.push_back(&map[row][col-1]);
      if (col != map[0].size() - 1 && map[row][col+1].height <= map[row][col].height + 1) successors.push_back(&map[row][col+1]);

      for (auto& node : successors)
      {  
         auto tempG = q->g + 1;

         auto openIt = openList.find(node);
         auto closeIt = closedList.find(node);
         if (openIt != openList.end())
         {
            if (node->g <= tempG) continue;
         }
         else if (closeIt != closedList.end())
         {
            if (node->g <= tempG) continue;
            closedList.erase(node);
            openList.insert(node);
         }
         else
         {
            openList.insert(node);
            node->h = calculateH(endRow, endCol, *node);
         }

         // Assign the parent
         node->p_row = row;
         node->p_col = col;

         node->g = tempG;
      }
      closedList.insert(q);
   }
   return std::set<Node>();
}

void part1(T input)
{
   uint32_t startRow{0};
   uint32_t startCol{0}; 
   uint32_t endRow{0}; 
   uint32_t endCol{0};
   findStartAndEnd(input, startRow, startCol, endRow, endCol);
   
   auto path = findShortestPath(input, startRow, startCol, endRow, endCol);

   std::cout << path.size() << "\n";
}

void part2(T input)
{
   uint32_t startRow{0};
   uint32_t startCol{0}; 
   uint32_t endRow{0}; 
   uint32_t endCol{0};
   findStartAndEnd(input, startRow, startCol, endRow, endCol);

   std::vector<size_t> lengths;
   
   for (auto i = 0; i < input.size(); ++i)
   {
      for (auto j = 0; j < input[0].size(); ++j)
      {
         if (input[i][j].height == 'a')
         {
            auto path = findShortestPath(input, i, j, endRow, endCol);
            if (!path.empty())
            {
               lengths.push_back(path.size());
            }
         }
      }
   }

   std::cout << *(std::min_element(lengths.begin(), lengths.end())) << "\n";
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