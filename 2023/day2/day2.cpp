#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <sstream>

struct Game
{
   Game(const uint32_t& id) : id(id) {}

   std::string to_string() const
   {
      std::string ret{"Game " + std::to_string(id) + ":"};
      for (auto set : sets) 
      {
         ret += " {" + std::to_string(std::get<0>(set)) + ","
                     + std::to_string(std::get<1>(set)) + ","
                     + std::to_string(std::get<2>(set)) + "}";
      }
      ret += "\n";
      return ret;
   }

   uint32_t id;
   std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> sets;  
};

typedef std::vector<Game> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string strVal;

   while(std::getline(f, strVal))
   {
      uint16_t blue{0};
      uint16_t red{0};
      uint16_t green{0};

      auto spaceFind{strVal.find(' ')};
      auto colonFind{strVal.find(':')};
      Game game(std::stoul(strVal.substr(spaceFind + 1, colonFind - spaceFind - 1)));
      strVal.erase(strVal.begin(), strVal.begin() + colonFind + 1);

      std::string set;
      std::stringstream gameStream{strVal};
      while (std::getline(gameStream, set, ';'))
      {
         std::string piece;
         std::stringstream setStream{set};
         while (std::getline(setStream, piece, ','))
         {
            if (piece.find("blue") != std::string::npos)
            {
               blue = std::stoul(piece.substr(1, piece.find(' ', 1)));
            }
            else if (piece.find("red") != std::string::npos)
            {
               red = std::stoul(piece.substr(1, piece.find(' ', 1)));
            }
            else if (piece.find("green") != std::string::npos)
            {
               green = std::stoul(piece.substr(1, piece.find(' ', 1)));
            }
            else
            {
               std::cout << piece << " does not contain a color\n";
               return;
            }
         }

         game.sets.push_back({blue, red, green});
         blue = 0;
         red = 0;
         green = 0;
      }
      vals.push_back(game);
   }
   f.close();
}

void part1(T input)
{
   uint32_t idSum{0};

   for (auto& game : input)
   {
      bool fail{false};
      for (auto& set : game.sets)
      {
         if (std::get<0>(set) > 14 ||
             std::get<1>(set) > 12 ||
             std::get<2>(set) > 13)
         {
            fail = true;
            break;
         }
      }
      if (!fail) idSum += game.id;
   }

   std::cout << idSum << "\n";
}

void part2(T input)
{
   uint32_t powerSum{0};

   for (auto& game : input)
   {
      uint32_t maxBlue{0};
      uint32_t maxRed{0};
      uint32_t maxGreen{0};
      for (auto& set : game.sets)
      {
         if (std::get<0>(set) > maxBlue) maxBlue = std::get<0>(set);
         if (std::get<1>(set) > maxRed) maxRed = std::get<1>(set);
         if (std::get<2>(set) > maxGreen) maxGreen = std::get<2>(set);
      }

      powerSum += (maxBlue * maxRed * maxGreen);
   }   

   std::cout << powerSum << "\n";
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