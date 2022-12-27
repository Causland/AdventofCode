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

uint32_t findMaxPressure(Valve* valve, Valve* lastValve, std::set<std::string>& openedValves, int32_t hour)
{
   // Check if time has run out
   if (hour <= 0)
   {
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
         auto pressure = findMaxPressure(neighbor, valve, openedValves, hour - 1);
         if (pressure > justMovePressure) justMovePressure = pressure;
      }
      else
      {
         // Open then move
         openedValves.insert(valve->id);
         auto pressure = findMaxPressure(neighbor, valve, openedValves, hour - 2);
         if (pressure > openThenMovePressure) openThenMovePressure = pressure;
         openedValves.erase(valve->id);
      }
   }

   // Return the better value
   return std::max(openPressure + openThenMovePressure, justMovePressure);
}

bool detectDeadEnd(Valve* currentValve, Valve* valveToCheck)
{
   Valve* l = currentValve;
   Valve* v = valveToCheck;
   do
   {
      if (v->paths.size() > 2) return false;
      if (v->paths.size() == 1) return true;

      // Move to next valve
      auto it = std::find_if_not(v->paths.begin(), v->paths.end(), [l](auto path){ return path == l; });
      l = v;
      v = *it;
   } while (true);
}

uint32_t findMaxPressureWithTwo(Valve* valveMe, Valve* valveElephant, Valve* lastValveMe, Valve* lastValveElephant, std::set<std::string>& openedValves, int32_t hourMe, int32_t hourElephant)
{
   bool meTimeExpired = hourMe <= 0;
   bool elephantTimeExpired = hourElephant <= 0;

   // Check if time has run out
   if (meTimeExpired && elephantTimeExpired)
   {
      return 0;
   }

   // Check if this valve has already been opened (Saves time in calculating later on)
   // Say its already opened if the flowRate is 0
   bool meAlreadyOpen = openedValves.find(valveMe->id) != openedValves.end() || valveMe->flowRate == 0;
   bool elephantAlreadyOpen = openedValves.find(valveElephant->id) != openedValves.end() || valveElephant->flowRate == 0;

   // Calculate pressure released by opening this valve
   uint32_t meOpenPressure = 0;
   uint32_t elephantOpenPressure = 0;
   if (!meAlreadyOpen && !meTimeExpired)
   {
      meOpenPressure = valveMe->flowRate * (hourMe - 1);
   }
   if (!elephantAlreadyOpen && !elephantTimeExpired)
   {
      elephantOpenPressure = valveElephant->flowRate * (hourElephant - 1);
   }

   // If the hours are different, we want to only move/open the one that has more time until they are
   // at the same time period again. This prevents valves from being read/opened out of chronologic order
   if (hourElephant < hourMe)
   {
      // Only perform me actions
      uint32_t openThenMovePressure = 0;
      uint32_t justMovePressure = 0;
      for (auto* neighbor : valveMe->paths)
      {
         if (meAlreadyOpen)
         {
            // If the neighbor was the immediate last neighbor skip. We don't want
            // to go immediately back to where we were
            if (lastValveMe != nullptr)
            {
               if (neighbor == lastValveMe) continue;
            }

            // Just move
            auto pressure = findMaxPressureWithTwo(neighbor, valveElephant, valveMe, lastValveElephant, openedValves, hourMe - 1, hourElephant);
            if (pressure > justMovePressure) justMovePressure = pressure;
         }
         else
         {
            // Open then move
            openedValves.insert(valveMe->id);
            auto pressure = findMaxPressureWithTwo(neighbor, valveElephant, valveMe, lastValveElephant, openedValves, hourMe - 2, hourElephant);
            if (pressure > openThenMovePressure) openThenMovePressure = pressure;
            openedValves.erase(valveMe->id);
         }     
      }
      return std::max(meOpenPressure + openThenMovePressure, justMovePressure);
   }
   else if (hourMe < hourElephant)
   {
      // Only perform elephant actions
      uint32_t openThenMovePressure = 0;
      uint32_t justMovePressure = 0;
      for (auto* neighbor : valveElephant->paths)
      {
         if (elephantAlreadyOpen)
         {
            // If the neighbor was the immediate last neighbor skip. We don't want
            // to go immediately back to where we were
            if (lastValveElephant != nullptr)
            {
               if (neighbor == lastValveElephant) continue;
            }

            // Just move
            auto pressure = findMaxPressureWithTwo(valveMe, neighbor, lastValveMe, valveElephant, openedValves, hourMe, hourElephant - 1);
            if (pressure > justMovePressure) justMovePressure = pressure;
         }
         else
         {
            // Open then move
            openedValves.insert(valveElephant->id);
            auto pressure = findMaxPressureWithTwo(valveMe, neighbor, lastValveMe, valveElephant, openedValves, hourMe, hourElephant - 2);
            if (pressure > openThenMovePressure) openThenMovePressure = pressure;
            openedValves.erase(valveElephant->id);
         }     
      }
      return std::max(elephantOpenPressure + openThenMovePressure, justMovePressure);
   }
   else
   {
      // Perform actions for both
      uint32_t openOpenPressure = 0;
      uint32_t openMovePressure = 0;
      uint32_t moveOpenPressure = 0;
      uint32_t moveMovePressure = 0;
      for (auto* meNeighbor : valveMe->paths)
      {
         if (meNeighbor->deadEndPathHit) continue; // Do not go down same dead end that someone else has already gone down
         meNeighbor->deadEndPathHit = detectDeadEnd(valveMe, meNeighbor);
         for (auto* elephantNeighbor : valveElephant->paths)
         {
            if (elephantNeighbor->deadEndPathHit) continue; // Do not go down same dead end that someone else has already gone down
            elephantNeighbor->deadEndPathHit = detectDeadEnd(valveElephant, elephantNeighbor);

            if (meNeighbor == elephantNeighbor)
            {
               // Always move me, choose to open or move elephant
               if (elephantAlreadyOpen)
               {
                  // Move both
                  auto pressure = findMaxPressureWithTwo(meNeighbor, elephantNeighbor, valveMe, valveElephant, openedValves, hourMe - 1, hourElephant - 1);
                  if (pressure > moveMovePressure) moveMovePressure = pressure;
               }
               else
               {
                  // Move me, open elephant
                  openedValves.insert(valveElephant->id);
                  auto pressure = findMaxPressureWithTwo(meNeighbor, elephantNeighbor, valveMe, valveElephant, openedValves, hourMe - 1, hourElephant - 2);
                  if (pressure > moveOpenPressure) moveOpenPressure = pressure;
                  openedValves.erase(valveElephant->id);
               }      
            }
            else
            {
               if (meAlreadyOpen)
               {
                  if (elephantAlreadyOpen)
                  {
                     // Move both
                     auto pressure = findMaxPressureWithTwo(meNeighbor, elephantNeighbor, valveMe, valveElephant, openedValves, hourMe - 1, hourElephant - 1);
                     if (pressure > moveMovePressure) moveMovePressure = pressure;
                  }
                  else
                  {
                     // Move me, open elephant
                     openedValves.insert(valveElephant->id);
                     auto pressure = findMaxPressureWithTwo(meNeighbor, elephantNeighbor, valveMe, valveElephant, openedValves, hourMe - 1, hourElephant - 2);
                     if (pressure > moveOpenPressure) moveOpenPressure = pressure;
                     openedValves.erase(valveElephant->id);
                  }
               }
               else
               {
                  if (elephantAlreadyOpen)
                  {
                     // Open me, move elephant
                     openedValves.insert(valveMe->id);
                     auto pressure = findMaxPressureWithTwo(meNeighbor, elephantNeighbor, valveMe, valveElephant, openedValves, hourMe - 2, hourElephant - 1);
                     if (pressure > openMovePressure) openMovePressure = pressure;
                     openedValves.erase(valveMe->id);
                  }
                  else
                  {
                     // Open me, open elephant
                     openedValves.insert(valveMe->id);
                     openedValves.insert(valveElephant->id);
                     auto pressure = findMaxPressureWithTwo(meNeighbor, elephantNeighbor, valveMe, valveElephant, openedValves, hourMe - 2, hourElephant - 2);
                     if (pressure > openOpenPressure) openOpenPressure = pressure;
                     openedValves.erase(valveMe->id);      
                     openedValves.erase(valveElephant->id);  
                  }
               }
            }
         }
      }
      return std::max({meOpenPressure + elephantOpenPressure + openOpenPressure,
                       meOpenPressure + openMovePressure,
                       elephantOpenPressure + moveOpenPressure,
                       moveMovePressure});
   }
}

void part1(T& input)
{
   Valve* startValve = &(*std::find_if(input.begin(), input.end(), [](auto v){ return v.id == "AA"; }));
   std::set<std::string> openedValves;

   auto result = findMaxPressure(startValve, nullptr, openedValves, 30);
   std::cout << result << "\n";
}

void part2(T& input)
{
   Valve* startValve = &(*std::find_if(input.begin(), input.end(), [](auto v){ return v.id == "AA"; }));
   std::set<std::string> openedValves;

   auto result = findMaxPressureWithTwo(startValve, startValve, nullptr, nullptr, openedValves, 26, 26);
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