#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <omp.h>
#include <atomic>
#include <mutex>

struct Record
{
   std::string sequence;
   std::vector<uint64_t> groups;
};

typedef std::vector<Record> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string strVal;

   std::string sequence;
   std::string groupsStr;
   while(f >> sequence >> groupsStr)
   {
      std::vector<uint64_t> groups;
      std::stringstream ss{groupsStr};
      std::string group;
      while (std::getline(ss, group, ',')) groups.push_back(std::stoul(group));
      vals.push_back(Record{sequence, groups});
   }
   f.close();
}

uint64_t repairSequence(const std::string& seq, const std::vector<uint64_t>& groups)
{
   uint64_t contiguousCount{0};
   uint64_t groupsIdx{0};
   for (auto i=0; i<seq.size() ; ++i)
   {
      if (seq[i] == '.') continue;
      else if (seq[i] == '#') ++contiguousCount;
      else
      {
         // Try with index as '.'
         std::string attempt{seq};
         attempt[i] = '.';
         uint64_t res{repairSequence(attempt, groups)};
         
         attempt[i] = '#';
         return res + repairSequence(attempt, groups);
      }

      // Check failure (End of group not at group count)
      if (contiguousCount < groups[groupsIdx])
      {
         if (i+1 < seq.size())
         {
            if (seq[i+1] == '.') return 0; // No point in continuing
            else continue; // Go onto next index
         }
         return 0; // At the end and we didn't reach the group count
      }

      if (contiguousCount == groups[groupsIdx])
      {
         // Check failure (End of group count not at end of group)
         if (i+1 < seq.size())
         {
            if (seq[i+1] == '#') return 0;
         }

         ++groupsIdx;
         contiguousCount = 0;
      }
      
      if (groupsIdx >= groups.size())
      {
         if (i + 1 >= seq.size()) return 1; // Made it to the end

         // Check for more #. (This is a failure)
         if (seq.find('#', i+1) != std::string::npos) return 0;

         // All should be .'s or ? remaining and therefore return 1
         return 1; 
      }
   }

   return 0; // Shouldn't get here. If we do, we have failed to reach the group requirements
}

void part1(T input)
{
   std::atomic<uint64_t> sum{0};

#pragma omp parallel for
   for (auto i=0; i<input.size(); ++i)
   {
      sum += repairSequence(input[i].sequence, input[i].groups);
   }
   std::cout << sum << "\n";
}

void part2(T input)
{
   auto start{std::chrono::system_clock::now()};
   for (auto& r : input)
   {
      auto origSize{r.sequence.size()};
      r.sequence.reserve(5 * origSize + 4); // +4 for delim by ?
      for (auto i=0; i<4; ++i)
      {
         r.sequence.push_back('?');
         std::copy_n(r.sequence.begin(), origSize, std::back_inserter(r.sequence));
      }

      origSize = r.groups.size();
      r.groups.reserve(5 * origSize);
      for (auto i=0; i<4; ++i)
      {
         std::copy_n(r.groups.begin(), origSize, std::back_inserter(r.groups));
      }
   }

   std::atomic<uint64_t> sum{0};
   std::mutex outputLock;

#pragma omp parallel for
   for (auto i=0; i<input.size(); ++i)
   {
      {
         std::unique_lock<std::mutex> lk{outputLock};
         std::cout << i << " " << input[i].sequence << "\n";
      }
      sum += repairSequence(input[i].sequence, input[i].groups);
   }

   auto end{std::chrono::system_clock::now()};
   std::cout << "P2 took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
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
