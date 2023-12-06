#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <sstream>

struct Conversion
{
   Conversion(const uint64_t& s, const uint64_t& d, const uint64_t& r) :
               src(s), dst(d), range(r) {}

   bool inRange(const uint64_t& s) const { return s >= src && s < (src + range); }
   uint64_t getDst(const uint64_t& s) const { return inRange(s) ? (s - src + dst) : s; }

   uint64_t src{0};
   uint64_t dst{0};
   uint64_t range{0};
};

struct Mapping
{
   uint64_t getDst(const uint64_t& s) const
   {
      for (const auto& conv : conversions)
      {
         const auto d{conv.getDst(s)};
         if (s != d) return d; 
      }

      return s;
   }

   std::vector<Conversion> conversions;
};

typedef std::pair<std::vector<uint64_t>, std::vector<Mapping>> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string strVal;

   // Get seeds
   std::getline(f, strVal);
   std::stringstream ss{strVal};
   std::string seed;
   std::getline(ss, seed, ' '); // Get rid of "seeds:"
   while(std::getline(ss, seed, ' ')) vals.first.push_back(std::stoull(seed));

   // Ignore first empty line
   std::getline(f, strVal);
   vals.second.emplace_back();

   while(std::getline(f, strVal))
   {
      if (strVal.find_last_of(':') != std::string::npos) continue; // Map title
      if (strVal.empty()) // New section
      {
         vals.second.emplace_back();
         continue;
      }

      std::stringstream mapping{strVal};
      uint64_t d;
      uint64_t s;
      uint64_t r;
      mapping >> d >> s >> r;
      vals.second.back().conversions.emplace_back(s, d, r);
   }
   f.close();
}

void part1(T input)
{
   uint64_t minLocation{UINT64_MAX};
   for (const auto& seed : input.first)
   {
      uint64_t val{seed};
      for (const auto& map : input.second)
      {
         val = map.getDst(val);
      }

      if (val < minLocation) minLocation = val;
   }

   std::cout << minLocation << "\n";
}

void part2(T input)
{
   uint64_t minLocation{UINT64_MAX};
   for (auto i=0; i<input.first.size(); i+=2)
   {
      for (auto seed=input.first[i]; seed < input.first[i] + input.first[i+1]; ++seed)
      {
         uint64_t val{seed};
         for (const auto& map : input.second)
         {
            val = map.getDst(val);
         }

         if (val < minLocation) minLocation = val;
      }
   }

   std::cout << minLocation << "\n";
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

   vals.first.clear();
   vals.second.clear();

   parseInputFile(part2File, vals);
   part2(vals);
   
   return 0;
}