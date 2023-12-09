#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

enum Type
{
   HIGH_CARD,
   ONE_PAIR,
   TWO_PAIR,
   THREE_OF_A_KIND,
   FULL_HOUSE,
   FOUR_OF_A_KIND,
   FIVE_OF_A_KIND,
};

const std::map<char, uint8_t> labelVal1{{'2', 2},
                                       {'3', 3},
                                       {'4', 4},
                                       {'5', 5},
                                       {'6', 6},
                                       {'7', 7},
                                       {'8', 8},
                                       {'9', 9},
                                       {'T', 10},
                                       {'J', 11},
                                       {'Q', 12},
                                       {'K', 13},
                                       {'A', 14}};

const std::map<char, uint8_t> labelVal2{{'J', 1},
                                        {'2', 2},
                                        {'3', 3},
                                        {'4', 4},
                                        {'5', 5},
                                        {'6', 6},
                                        {'7', 7},
                                        {'8', 8},
                                        {'9', 9},
                                        {'T', 10},
                                        {'Q', 12},
                                        {'K', 13},
                                        {'A', 14}};

bool part1{true};

struct Hand
{
   Hand(const std::string& hand, const uint32_t& bid) : hand(hand), bid(bid)
   {
      calcType();
   }

   Hand(const Hand& h) : hand(h.hand), bid(h.bid), type(h.type) {}
   void operator=(const Hand& h)
   {
      hand = h.hand;
      bid = h.bid;
      type = h.type;
   }

   bool operator<(const Hand& h) const { return (type < h.type) || 
                                                ((type == h.type) && (compareLabels(h))); }

   bool compareLabels(const Hand& h) const
   {
      auto lhit{hand.begin()};
      auto rhit{h.hand.begin()};
      while (lhit != hand.end() && rhit != h.hand.end())
      {
         if (part1)
         {
            if (labelVal1.at(*lhit) < labelVal1.at(*rhit)) return true;
            else if (labelVal1.at(*lhit) > labelVal1.at(*rhit)) return false;
         }
         else
         {
            if (labelVal2.at(*lhit) < labelVal2.at(*rhit)) return true;
            else if (labelVal2.at(*lhit) > labelVal2.at(*rhit)) return false;
         }

         ++lhit;
         ++rhit;
      }

      return false;
   }

   void calcType()
   {
      std::map<char, uint8_t> cardCount;
      for (const auto& c : hand)
      {
         auto it{cardCount.find(c)};
         if (it == cardCount.end()) cardCount.insert({c, 1});
         else it->second = it->second + 1;
      }

      if (part1) 
      {
         type = calcTypeNormal(cardCount);
      }
      else
      {
         type = calcTypeWild(cardCount);
      }
   }

   Type calcTypeNormal(const std::map<char, uint8_t>& cardCount)
   {
      if (cardCount.size() == 1)
      {
         return FIVE_OF_A_KIND;
      }
      else if (cardCount.size() == 2)
      {
         // Check 4 of kind
         // Check full house
         const auto p{cardCount.begin()};
         if (p->second == 4 || p->second == 1)
         {
            return FOUR_OF_A_KIND;
         }
         else
         {
            return FULL_HOUSE;
         }
      }
      else if (cardCount.size() == 3)
      {
         // Check 3 of kind
         // Check 2 pair
         for (const auto& p : cardCount)
         {
            if (p.second == 2)
            {
               return TWO_PAIR;
            }
            else if (p.second == 3)
            {
               return THREE_OF_A_KIND;
            }
         }
      }
      else if (cardCount.size() == 4)
      {
         return ONE_PAIR;
      }
      
      return HIGH_CARD;
   }

   Type calcTypeWild(std::map<char, uint8_t>& cardCount)
   {
      auto i{cardCount.find('J')};
      auto jCount{0};
      if (i != cardCount.end()) jCount = i->second;
      if (jCount == 5) return FIVE_OF_A_KIND;

      cardCount.erase('J');

      auto it{std::max_element(std::begin(cardCount), std::end(cardCount),
                                 [](const std::pair<char, uint8_t>& a, const std::pair<char, uint8_t>& b)
                                       { return a.second < b.second; })};

      it->second += jCount;
      return calcTypeNormal(cardCount);
   }

   std::string hand;
   uint32_t bid;
   Type type;
};

typedef std::vector<Hand> T;

void parseInputFile(std::string filename, T& vals)
{
   std::fstream f(filename);
   std::string hand;
   std::string bid;

   while(f >> hand >> bid)
   {
      vals.emplace_back(hand, std::stoul(bid));
   }
   f.close();
}

void func(T input)
{
   std::sort(input.begin(), input.end());

   uint64_t winnings{0};
   for (auto i=0; i<input.size(); ++i)
   {
      winnings += (input[i].bid * static_cast<uint64_t>(i+1));
   }

   std::cout << winnings << "\n";
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
   //func(vals);

   vals.clear();
   part1 = false;

   parseInputFile(part2File, vals);
   func(vals);
   
   return 0;
}