// AoC21 Day 4: 
// P1 - Find the bingo winner
// P2 - Find the last bingo winner

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <algorithm>
#include <bitset>

class Board
{
public:
    void insertRow(unsigned char b, unsigned char i, unsigned char n, unsigned char g, unsigned char o)
    {
        board[rowCount][0] = b;
        board[rowCount][1] = i;
        board[rowCount][2] = n;
        board[rowCount][3] = g;
        board[rowCount][4] = o;
        ++rowCount;
    }
    bool isFull()
    {
        return rowCount == 5;
    }
    void clear()
    {
        for (auto& row : board)
        {
            for (auto& col : row)
            {
                col = 0;
            }
        }
        rowCount = 0;
    }
    void printBoard()
    {
        for (auto& row : board)
            std::cout << std::to_string(row[0]) << " " << std::to_string(row[1]) << " " << std::to_string(row[2]) << " "
                        << std::to_string(row[3]) << " " << std::to_string(row[4]) << "\n";
    }
    bool searchBoard(unsigned char checkVal) // Search for value and if found, check for a win
    {
        for (int i=0; i<board.size(); ++i)
        {
            for (int j=0; j<board.size(); ++j)
            {
                if (board[i][j] == checkVal)
                {
                    board[i][j] = 0xFF;
                    return checkWin(i, j);
                }
            }
        }
        return false;
    }
    bool checkWin(unsigned char row, unsigned char col)
    {
        // Check row win
        if (board[row][0] == 0xFF && board[row][1] == 0xFF && board[row][2] == 0xFF &&
             board[row][3] == 0xFF && board[row][4] == 0xFF)
             return true;
        // Check col win
        if (board[0][col] == 0xFF && board[1][col] == 0xFF && board[2][col] == 0xFF &&
             board[3][col] == 0xFF && board[4][col] == 0xFF)
             return true;
        return false;
    }
    unsigned short countUnmarked()
    {
        unsigned short sum = 0;
        for (auto& row : board)
        {
            for(auto& num : row)
            {
                if (num != 0xFF)
                    sum += num;
            }
        }
        return sum;
    }
private:
    unsigned char rowCount{0};
    std::array<std::array<unsigned char, 5>, 5> board;
};

int main()
{
    std::ifstream file("input.txt"); // Open the input file and store it in a vector
    std::string s;
    file >> s;
    std::stringstream s_stream(s); //create string stream from the string
    std::vector<unsigned char> drawNumbers;
    while(s_stream.good()) {
        std::string substr;
        unsigned char val;
        getline(s_stream, substr, ','); //get first string delimited by comma
        val = std::stoi(substr);
        drawNumbers.push_back(val);
    }

    std::vector<Board> boards;
    Board b;
    unsigned int pos_b;
    unsigned int pos_i;
    unsigned int pos_n;
    unsigned int pos_g;
    unsigned int pos_o;
    while (file >> pos_b >> pos_i >> pos_n >> pos_g >> pos_o)
    {
        b.insertRow(pos_b, pos_i, pos_n, pos_g, pos_o);
        if (b.isFull())
        {
            boards.push_back(b);
            b.clear();
        }
    }

//==============================================================================
// Part 1
    {
        bool win = false;
        unsigned short winCount = 0;
        unsigned short winDraw = 0;
        for (auto& draw : drawNumbers)
        {
            for (auto& board : boards)
            {
                if (board.searchBoard(draw))
                {
                    win = true;
                    winCount = board.countUnmarked();
                    winDraw = draw;
                    break;
                }
            }
            if (win)
                break;
        }
        std::cout << "score=" << std::to_string(winCount) << " draw=" << std::to_string(winDraw) << " mult=" << std::to_string(winCount*winDraw) << "\n";
    }
//==============================================================================
// Part 2
    {
        bool win = false;
        unsigned short lastWinCount = 0;
        unsigned short lastWinDraw = 0;
        for(auto& draw : drawNumbers)
        {
            for (unsigned int i=0; i < boards.size();)
            {
                if (boards[i].searchBoard(draw))
                {
                    if (boards.size() == 1)
                    {
                        win = true;
                        lastWinCount = boards[i].countUnmarked();
                        lastWinDraw = draw;
                        break;
                    }
                    else
                    {
                        boards.erase(boards.begin() + i);
                        continue;
                    }
                }
                ++i;
            }
            if (win)
                break;
        }
        std::cout << "score=" << std::to_string(lastWinCount) << " draw=" << std::to_string(lastWinDraw) << " mult=" << std::to_string(lastWinCount*lastWinDraw) << "\n";
    }
}