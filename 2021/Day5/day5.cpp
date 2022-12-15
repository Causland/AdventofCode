// AoC21 Day 5: 
// P1 - Find number of overlapping lines horizontal and vertical

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <bitset>
#include <map>

struct Point
{
    Point(unsigned short x, unsigned short y) : x(x), y(y) {}
    std::string toString() const { return std::to_string(x) + "," + std::to_string(y); }
    bool operator<(const Point& p) const { return x < p.x || x == p.x && y < p.y; }
    unsigned short x;
    unsigned short y;
};

class Line
{
public:
    Line(Point p1, Point p2) : endPoint1(p1), endPoint2(p2)
    {
        points.push_back(endPoint1);
        // Calculate all intermediate points
        if (endPoint1.x != endPoint2.x && endPoint1.y != endPoint2.y)
        {
            if (p1.x > p2.x)
                std::swap(p1, p2);
            int slope = p1.y > p2.y ? -1 : 1;
            unsigned int x=p1.x+1;
            unsigned int y=p1.y+slope;
            for (;x<p2.x;)
            {
                points.emplace_back(x, y);
                ++x;
                y += slope;
            }
        }
        else if (endPoint1.x != endPoint2.x)
        {
            unsigned int max = std::max(endPoint1.x, endPoint2.x);
            unsigned int min = max == endPoint1.x ? endPoint2.x : endPoint1.x;
            for(unsigned int i=min+1; i<max; ++i)
                points.emplace_back(i, endPoint1.y);
        }
        else if (endPoint1.y != endPoint2.y)
        {
            unsigned int max = std::max(endPoint1.y, endPoint2.y);
            unsigned int min = max == endPoint1.y ? endPoint2.y : endPoint1.y;
            for(unsigned int i=min+1; i<max; ++i)
                points.emplace_back(endPoint1.x, i);
        }
        points.push_back(endPoint2);
    }
    std::vector<Point> points;
    Point endPoint1;
    Point endPoint2;
};

int main()
{
    std::vector<Line> lines;
    std::ifstream file("input.txt"); // Open the input file and store it in a vector
    std::string str1;
    std::string unused;
    std::string str2;
    unsigned short x;
    unsigned short y;
    while(file >> str1 >> unused >> str2) {
        std::size_t posComma = str1.find(",");
        std::string xStr = str1.substr(0,posComma);
        std::string yStr = str1.substr(posComma+1, std::string::npos);
        x = std::stoi(xStr);
        y = std::stoi(yStr);
        Point p1(x, y);
        posComma = str2.find(",");
        xStr = str2.substr(0,posComma);
        yStr = str2.substr(posComma+1, std::string::npos);
        x = std::stoi(xStr);
        y = std::stoi(yStr);
        Point p2(x,y);
        lines.emplace_back(p1, p2);
    }
//==============================================================================
// Part 1
    {
        std::map<Point, unsigned int> coveredPoints;
        for (auto& line : lines)
        {
            for (auto& point : line.points)
            {
                if(coveredPoints.contains(point))
                    coveredPoints.at(point) += 1;
                else
                    coveredPoints.insert({point, 1});
            }
        }
        unsigned int count = 0;
        for (auto & [point, val] : coveredPoints)
        {
            if (val >= 2)
                ++count;
        }
        std::cout << "Number greater or equal to 2 is " << std::to_string(count) << "\n";
    }
//==============================================================================
// Part 2
    {
        // Implemented in P1 except count diagnal lines
    }
}