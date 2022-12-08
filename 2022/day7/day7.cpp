#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

struct File
{
   File(std::string name, uint32_t size) : name(name), size(size) {};
   File() = delete;

   std::string name;
   uint32_t size;
};

struct Directory
{
   Directory(std::string name, std::string path, Directory* parent) : name(name), path(path), parent(parent) {};
   Directory() = delete;

   std::string name; // name should end with /
   std::string path;
   Directory* parent{nullptr};
   std::vector<Directory> dirs;
   std::vector<File> files;
   uint32_t dirSize{0};

   void addFile(std::string name, uint32_t size)
   {
      files.emplace_back(name, size);
      dirSize += size;
   }

   void addDirectory(std::string n)
   {
      dirs.emplace_back(n, path + name, this);
   }
};

uint32_t calcTotalDirSize(Directory& dir)
{
   uint32_t totalDirSize = 0;
   for (auto& d : dir.dirs)
   {
      totalDirSize += calcTotalDirSize(d);
   }
   totalDirSize += dir.dirSize; // Just sum of files until after this function is called
   dir.dirSize = totalDirSize;
   return totalDirSize;
}

typedef Directory T;

void parseInputFile(std::string filename, T& fileSystem)
{
   std::fstream f(filename);
   std::string line;

   Directory* cwd = &fileSystem;
   Directory* parent = nullptr;

   while(std::getline(f, line))
   { 
      if (line.front() == '$')
      {
         if (line.find("cd ") != std::string::npos)
         {
            auto posSpace = line.find_last_of(' ');
            std::string dir = line.substr(posSpace + 1);
            if (dir == "..")
            {
               if (cwd->name == "/") continue;
               cwd = parent;
               parent = cwd->parent;
            }
            else
            {
               if (dir == "/") continue;
               auto it = std::find_if(cwd->dirs.begin(), cwd->dirs.end(), [dir](auto& d){ return d.name == dir + "/"; });
               if (it != cwd->dirs.end())
               {
                  parent = cwd;
                  cwd = &(*it); // Weird but I'll allow it
               }
               else
               {
                  cwd->addDirectory(dir + "/");
                  parent = cwd;
                  cwd = &(cwd->dirs.back());
               }
            }
         }
         if (line == "$ ls")
         {
            while(std::getline(f, line))
            {
               if (line.find("dir ") != std::string::npos)
               {
                  auto posSpace = line.find_last_of(' ');
                  std::string dir = line.substr(posSpace + 1);

                  auto it = std::find_if(cwd->dirs.begin(), cwd->dirs.end(), [dir](auto& d){ return d.name == dir + "/"; });
                  if (it == cwd->dirs.end())
                  {
                     cwd->addDirectory(dir + "/");
                  }
               }
               else
               {
                  auto posSpace = line.find_last_of(' ');
                  uint32_t size = std::stol(line.substr(0, posSpace));
                  std::string name = line.substr(posSpace + 1);
                  auto it = std::find_if(cwd->files.begin(), cwd->files.end(), [name](auto& f){ return f.name == name; });
                  if (it == cwd->files.end())
                  {
                     cwd->addFile(name, size);
                  }
               }
               if (f.peek() == '$') break;
            }
         }
      }
   }
   calcTotalDirSize(fileSystem);
   f.close();
}

void printDirectory(Directory& dir, uint32_t depth)
{
   std::string padding;
   for (auto i=0; i<depth; ++i) padding += "  ";
   std::cout << padding << "- " << dir.name << " (dir)\n";

   for (auto& d : dir.dirs)
   {
      printDirectory(d, depth + 1);
   }
   for (auto& f : dir.files)
   {
      std::cout << padding << "  - " << f.name << " (file, size=" << f.size << ")\n"; 
   }
}

void findSizesUnder100000(Directory& dir, std::vector<uint32_t>& sizes)
{
   for(auto& d : dir.dirs)
   {
      findSizesUnder100000(d, sizes);
   }
   
   if (dir.dirSize < 100000)
   {
      sizes.push_back(dir.dirSize);
   }
}

void part1(T& fileSystem)
{
   std::vector<uint32_t> sizes;
   findSizesUnder100000(fileSystem, sizes);
   uint32_t result = std::accumulate(sizes.begin(), sizes.end(), 0);
   std::cout << result << "\n";
}

void findSmallestForRequired(Directory& dir, uint32_t& required, uint32_t& smallest)
{
   for(auto& d : dir.dirs)
   {
      findSmallestForRequired(d, required, smallest);
   }
   
   if (dir.dirSize > required && dir.dirSize < smallest)
   {
      smallest = dir.dirSize;
   }
}

void part2(T fileSystem)
{
   uint32_t requiredSpace = 30000000 - (70000000 - fileSystem.dirSize);
   uint32_t smallest = static_cast<uint32_t>(0xFFFFFFFF);
   findSmallestForRequired(fileSystem, requiredSpace, smallest);

   std::cout << smallest << "\n";
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

   T fileSystem("/", "", nullptr);
   parseInputFile(part1File, fileSystem);
   part1(fileSystem);

   T fileSystem2("/", "", nullptr);

   parseInputFile(part2File, fileSystem2);
   part2(fileSystem2);
   
   return 0;
}