#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <cstdio>

struct Blueprint
{
   uint32_t id{0};
   uint32_t oreRobotCostOre{0};
   uint32_t clayRobotCostOre{0};
   uint32_t obsidianRobotCostOre{0};
   uint32_t obsidianRobotCostClay{0};
   uint32_t geodeRobotCostOre{0};
   uint32_t geodeRobotCostObsidian{0};
};

struct Workers
{
   Workers() {};
   Workers(const uint32_t& a, const uint32_t& b, const uint32_t& c, const uint32_t& d) : 
            numOreRobots(a), numClayRobots(b), numObsidianRobots(c), numGeodeRobots(d) {};

   enum class Type
   {
      ORE,
      CLAY,
      OBSIDIAN,
      GEODE,
      NONE,
   };

   uint32_t numOreRobots{1}; // Start with 1
   uint32_t numClayRobots{0};
   uint32_t numObsidianRobots{0};
   uint32_t numGeodeRobots{0};
};

struct Inventory
{
   Inventory() {};
   Inventory(const uint32_t& a, const uint32_t& b, const uint32_t& c, const uint32_t& d) :
            numOre(a), numClay{b}, numObsidian{0}, numGeode{0} {};

   uint32_t numOre{0};
   uint32_t numClay{0};
   uint32_t numObsidian{0};
   uint32_t numGeode{0};
};

typedef std::vector<Blueprint> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string line;

   while(std::getline(f, line))
   {
      Blueprint b;
      sscanf(line.c_str(),
             "Blueprint %u: Each ore robot costs %u ore. Each clay robot costs %u ore. Each obsidian robot costs %u ore and %u clay. Each geode robot costs %u ore and %u obsidian.",
             &b.id, &b.oreRobotCostOre, &b.clayRobotCostOre, &b.obsidianRobotCostOre,
             &b.obsidianRobotCostClay, &b.geodeRobotCostOre, &b.geodeRobotCostObsidian);
      vals.push_back(b);
   }
   f.close();
}

void addPendingRobot(Workers& w, Workers::Type worker)
{
   switch(worker)
   {
      case Workers::Type::ORE:
         ++w.numOreRobots;
         break;
      case Workers::Type::CLAY:
         ++w.numClayRobots;
         break;
      case Workers::Type::OBSIDIAN:
         ++w.numObsidianRobots;
         break;
      case Workers::Type::GEODE:
         ++w.numGeodeRobots;
         break;
      case Workers::Type::NONE: // Fallthrough
      default:
         break;
   }
}

void collectFromRobots(Workers& w, Inventory& i)
{
   i.numOre += w.numOreRobots;
   i.numClay += w.numClayRobots;
   i.numObsidian += w.numObsidianRobots;
   i.numGeode += w.numGeodeRobots;
}

uint32_t maxGeodes(Blueprint& b, Workers w, Inventory i, Workers::Type factory, int32_t minute, uint32_t& maxNumGeodes)
{
   if (minute <= 0) 
   {
      if (i.numGeode > maxNumGeodes) maxNumGeodes = i.numGeode;
      return i.numGeode;
   }

   // Early terminate if we know that the numGeodes will not be greater than the most
   // Current number + realized number + estimated number
   if ((i.numGeode) + (w.numGeodeRobots * minute) + ((minute / 1.5) * minute) < maxNumGeodes)
   {
      return 0;
   }

   // Add the pending robot
   addPendingRobot(w, factory);

   uint32_t numGeodes = 0;
   // Build any robots you can except there is no point in last minute
   if (minute > 1)
   {
      if (i.numOre >= b.oreRobotCostOre)
      {
         // Start building ore robot
         Inventory tempInv = i;
         tempInv.numOre -= b.oreRobotCostOre;
         collectFromRobots(w, tempInv);
         auto out = maxGeodes(b, w, tempInv, Workers::Type::ORE, minute - 1, maxNumGeodes);
         if (out > numGeodes) numGeodes = out; 
      }
      if (i.numOre >= b.clayRobotCostOre)
      {
         // Start building clay robot
         Inventory tempInv = i;
         tempInv.numOre -= b.clayRobotCostOre;
         collectFromRobots(w, tempInv);
         auto out = maxGeodes(b, w, tempInv, Workers::Type::CLAY, minute - 1, maxNumGeodes);
         if (out > numGeodes) numGeodes = out;
      }
      if (i.numOre >= b.obsidianRobotCostOre && i.numClay >= b.obsidianRobotCostClay)
      {
         // Start building obsidian robot
         Inventory tempInv = i;
         tempInv.numOre -= b.obsidianRobotCostOre;
         tempInv.numClay -= b.obsidianRobotCostClay;
         collectFromRobots(w, tempInv);
         auto out = maxGeodes(b, w, tempInv, Workers::Type::OBSIDIAN, minute - 1, maxNumGeodes);
         if (out > numGeodes) numGeodes = out;
      }
      if (i.numOre >= b.geodeRobotCostOre && i.numObsidian >= b.geodeRobotCostObsidian)
      {
         // Start building geode robot
         Inventory tempInv = i;
         tempInv.numOre -= b.geodeRobotCostOre;
         tempInv.numObsidian -= b.geodeRobotCostObsidian;
         collectFromRobots(w, tempInv);
         auto out = maxGeodes(b, w, tempInv, Workers::Type::GEODE, minute - 1, maxNumGeodes);
         if (out > numGeodes) numGeodes = out;    
      }
   }

   collectFromRobots(w, i);
   auto out = maxGeodes(b, w, i, Workers::Type::NONE, minute - 1, maxNumGeodes);
   if (out > numGeodes) numGeodes = out;

   return numGeodes;
}

void part1(T input)
{
   const int32_t minutes = 24;

   uint32_t qualityLevel = 0;

   for (auto& b : input)
   {
      uint32_t numGeodes = 0;
      Workers w;
      Inventory i;
      auto out = maxGeodes(b, w, i, Workers::Type::NONE, minutes, numGeodes);
      qualityLevel += b.id * out;
   }

   std::cout << qualityLevel << "\n";
}

void part2(T input)
{
   const int32_t minutes = 32;

   uint64_t result = 1;

   for (auto i=0; i<3 && i<input.size(); ++i)
   {
      uint32_t numGeodes = 0;
      Workers w;
      Inventory inv;
      auto out = maxGeodes(input[i], w, inv, Workers::Type::NONE, minutes, numGeodes);
      result *= out;
      std::cout << out << " ";
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

   T vals;
   parseInputFile(part1File, vals);
   part1(vals);

   vals.clear();

   parseInputFile(part2File, vals);
   part2(vals);
   
   return 0;
}