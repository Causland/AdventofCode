#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <sstream>

typedef std::vector<std::vector<int64_t>> History;
typedef std::vector<History> T;

void printHistory(const History& h)
{
   for (const auto& sequence : h){
      for (const auto& val : sequence)
      {
           std::cout << val << " ";
      }
      std::cout << "\n";
   }
   std::cout << "\n";
}

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string line;

   while(std::getline(f, line))
   {
      auto& history{vals.emplace_back()};
      auto& entry{history.emplace_back()};
      int64_t val{0};
      std::stringstream ss{line};
      while(ss >> val)
      {
         entry.push_back(val);
      }
   }
   f.close();

   // Calculate histories
   for (auto& history : vals)
   {
      // Check all 0's
      while (std::find_if(history.back().begin(), history.back().end(), 
                        [](const int64_t& x){ return x != 0; }) != history.back().end())
      {
         auto& newSeq{history.emplace_back()};
         const auto& oldSeqIt{std::prev(history.end(), 2)};
         
         for (auto it=std::next(oldSeqIt->begin()); it != oldSeqIt->end(); ++it)
         {
            newSeq.push_back(*it - *std::prev(it));
         }
      }
   }
}

void part1(T& input)
{
   int64_t sum{0};
   for (auto& history : input)
   {
      // Calculate the next entry in the sequence
      history.back().push_back(0);

      for (auto rit=std::next(history.rbegin()); rit != history.rend(); ++rit)
      {
         const auto& lowIt{std::prev(rit)};
         rit->push_back(rit->back() + lowIt->back());
      }

      sum += history.front().back(); // Last val of first sequence
   }

   std::cout << sum << "\n";
}

void part2(T& input)
{
   int64_t sum{0};
   for (auto& history : input)
   {
      // Calculate the next entry in the sequence
      history.back().insert(history.back().begin(), 0);

      for (auto rit=std::next(history.rbegin()); rit != history.rend(); ++rit)
      {
         const auto& lowIt{std::prev(rit)};
         rit->insert(rit->begin(), rit->front() - lowIt->front());
      }

      sum += history.front().front(); // First val of first sequence
   }

   std::cout << sum << "\n";
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
