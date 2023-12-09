#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <sstream>

typedef std::vector<std::pair<uint32_t, uint32_t>> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);

   std::string timeLine;
   std::getline(f, timeLine);
   std::string distanceLine;
   std::getline(f, distanceLine);
   timeLine = timeLine.substr(10);
   distanceLine = distanceLine.substr(10);

   while (!timeLine.empty() && !distanceLine.empty())
   {
      timeLine.erase(timeLine.begin(), 
                     std::find_if(timeLine.begin(), timeLine.end(), 
                                    [](char c) { return c != ' '; }));
      distanceLine.erase(distanceLine.begin(), 
                     std::find_if(distanceLine.begin(), distanceLine.end(), 
                                    [](char c) { return c != ' '; }));

      auto nextTimeSpace{timeLine.find(' ')};
      auto nextDistanceSpace{distanceLine.find(' ')};

      std::string timeStr{timeLine};
      std::string distanceStr{distanceLine};
      if (nextTimeSpace != std::string::npos) 
      {
         timeStr = timeLine.substr(0, nextTimeSpace);
         timeLine = timeLine.substr(nextTimeSpace + 1);
      }
      else
      {
         timeLine.clear();
      }
      if (nextDistanceSpace != std::string::npos) 
      {
         distanceStr = distanceLine.substr(0, nextDistanceSpace);
         distanceLine = distanceLine.substr(nextDistanceSpace + 1);
      }
      vals.push_back({std::stoul(timeStr), std::stoul(distanceStr)});
   }

   f.close();
}

void part1(T input)
{
   uint32_t result{1};
   for (const auto& race : input)
   {
      uint32_t c{0};

      for (auto tw=1; tw<race.first; ++tw)
      {
         auto d = tw * (race.first - tw);
         if (d > race.second) ++c;
      }

      result *= c;
   }

   std::cout << result << "\n";
}

void part2(T input)
{
   std::string timeStr;
   std::string distanceStr;

   for (const auto& pair : input)
   {
      timeStr += std::to_string(pair.first);
      distanceStr += std::to_string(pair.second);
   }

   uint64_t time = std::stoull(timeStr);
   uint64_t distance = std::stoull(distanceStr);

   uint64_t c{0};

   for (auto tw=1; tw<time; ++tw)
   {
      auto d = tw * (time - tw);
      if (d > distance) ++c;
   }

   std::cout << c << "\n";
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