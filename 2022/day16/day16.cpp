#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>
#include <cstdio>
#include <sstream>
#include <iomanip>

struct Valve
{
   Valve(const std::string& i, const uint32_t& f) : id(i), flowRate(f) {};
   Valve() = delete;

   bool operator<(const Valve& v) const { return this->id < v.id; };

   void open() { isOpened = true; };

   std::string id;
   uint32_t flowRate{0};
   bool isOpened{false};
   bool deadEndPathHit{false};
   std::vector<Valve*> paths;
};

typedef std::vector<Valve> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string line;

   // Add all the valves to the list
   while(std::getline(f, line))
   {
      auto posSemi = line.find(';');
      std::string rateStr = line.substr(0, posSemi);
      char id[3]; id[2] = '\0';
      uint32_t rate;
      sscanf(rateStr.c_str(), "Valve %s has flow rate=%u", id, &rate);
      vals.emplace_back(std::string(id), rate);
      
   }
   f.close();

   // Go through again and add all paths
   f.open(filename);
   auto i=0;
   while(std::getline(f, line))
   {
      auto posSemi = line.find(';');
      std::string pathStr = line.substr(posSemi + 1);
      auto posValveStr = pathStr.find("valve");
      auto posSpace = pathStr.find(' ', posValveStr);
      std::string paths = pathStr.substr(posSpace + 1);
      std::stringstream ss{paths};
      std::string valve;
      while(std::getline(ss, valve, ','))
      {
         if (valve.front() == ' ') valve = valve.substr(1);
         auto it = std::find_if(vals.begin(), vals.end(), [valve](auto& v){ return v.id == valve; });
         vals[i].paths.push_back(&(*it));
      }
      ++i;
   }
}

void printValves(T& valves)
{
   for (auto& valve : valves)
   {
      std::cout << valve.id << ", " << std::setw(2) << valve.flowRate << " | ";
      for (auto& valvePtr : valve.paths)
      {
         std::cout << valvePtr->id << " ";
      }
      std::cout << "\n";
   }
}

uint32_t findMaxPressure(Valve* valve, Valve* lastValve, std::set<std::string>& openedValves, int32_t hour, uint32_t prevVal, std::vector<std::pair<std::set<std::string>, uint32_t>>* finishedStates)
{
   // Check if time has run out
   if (hour <= 0)
   {
      if (finishedStates != nullptr) // If a states vector is provided, populate it
      {
         auto it = std::find_if(finishedStates->begin(), finishedStates->end(), [openedValves](auto& aPair){ return openedValves == aPair.first; });
         if (it != finishedStates->end())
         {
            if ((*it).second < prevVal) (*it).second = prevVal;
         }
         else
         {
            finishedStates->push_back({openedValves, prevVal});
         }
      }
      return 0;
   }

   // Check if this valve has already been opened (Saves time in calculating later on)
   // Say its already opened if the flowRate is 0
   bool alreadyOpen = openedValves.find(valve->id) != openedValves.end() || valve->flowRate == 0;

   // Calculate pressure released by opening this valve
   uint32_t openPressure = 0;
   if (!alreadyOpen)
   {
      openPressure = valve->flowRate * (hour - 1);
   }

   // Calculate the max pressure released by opening then moving on
   // and the max pressure released by just moving on
   uint32_t openThenMovePressure = 0;
   uint32_t justMovePressure = 0;
   for (auto* neighbor : valve->paths)
   {
      if (alreadyOpen)
      {
         // If the neighbor was the immediate last neighbor skip. We don't want
         // to go immediately back to where we were
         if (lastValve != nullptr)
         {
            if (neighbor == lastValve) continue;
         }

         // Just move
         auto pressure = findMaxPressure(neighbor, valve, openedValves, hour - 1, prevVal, finishedStates);
         if (pressure > justMovePressure) justMovePressure = pressure;
      }
      else
      {
         // Open then move
         openedValves.insert(valve->id);
         auto pressure = findMaxPressure(neighbor, valve, openedValves, hour - 2, prevVal + openPressure, finishedStates);
         if (pressure > openThenMovePressure) openThenMovePressure = pressure;
         openedValves.erase(valve->id);
      }
   }

   // Return the better value
   return std::max(openPressure + openThenMovePressure, justMovePressure);
}

void part1(T& input)
{
   Valve* startValve = &(*std::find_if(input.begin(), input.end(), [](auto v){ return v.id == "AA"; }));
   std::set<std::string> openedValves;

   auto result = findMaxPressure(startValve, nullptr, openedValves, 30, 0, nullptr);
   std::cout << result << "\n";
}

void part2(T& input)
{
   Valve* startValve = &(*std::find_if(input.begin(), input.end(), [](auto v){ return v.id == "AA"; }));
   std::set<std::string> openedValves;
   std::vector<std::pair<std::set<std::string>, uint32_t>> meFinishedStates;

   // Find all the different combinations of valve openings with their point values
   findMaxPressure(startValve, nullptr, openedValves, 26, 0, &meFinishedStates);

   uint32_t result = 0;
   for (auto& pair : meFinishedStates)
   {
      // Pass the finished state of the valves to find the max pressure released by the elephant
      uint32_t elephantMax = findMaxPressure(startValve, nullptr, pair.first, 26, 0, nullptr);
      if (pair.second + elephantMax > result) result = pair.second + elephantMax;
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