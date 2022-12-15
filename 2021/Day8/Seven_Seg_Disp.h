#include <string>

struct Seven_Seg_Disp
{
    Seven_Seg_Disp(std::string s)
    {
        for (unsigned int i=0; i<s.size(); ++i)
        {
            switch(s[i])
            {
                case 'a':
                    segments |= (1 << 0);
                    break;
                case 'b':
                    segments |= (1 << 1);
                    break;
                case 'c':
                    segments |= (1 << 2);
                    break;
                case 'd':
                    segments |= (1 << 3);
                    break;
                case 'e':
                    segments |= (1 << 4);
                    break;
                case 'f':
                    segments |= (1 << 5);
                    break;
                case 'g':
                    segments |= (1 << 6);
                    break;
                default:
                    break;
            }
        }
        size = s.size();
        segmentStr = s;
    }
    std::string toString()
    {
        bool seg_a = (segments >> 0) & 1;
        bool seg_b = (segments >> 1) & 1;
        bool seg_c = (segments >> 2) & 1;
        bool seg_d = (segments >> 3) & 1;
        bool seg_e = (segments >> 4) & 1;
        bool seg_f = (segments >> 5) & 1;
        bool seg_g = (segments >> 6) & 1;
        std::string result;
        if (seg_a)
            result += " aaaa \n";
        else
            result += " .... \n";
        if (seg_b && seg_c)
            result += "b    c\nb    c\n";
        else if (seg_b)
            result += "b    .\nb    .\n";
        else if (seg_c)
            result += ".    c\n.    c\n";
        else
            result += ".    .\n.    .\n";
        if (seg_d)
            result += " dddd \n";
        else
            result += " .... \n";
        if (seg_e && seg_f)
            result += "e    f\ne    f\n";
        else if (seg_e)
            result += "e    .\ne    .\n";
        else if (seg_f)
            result += ".    f\n.    f\n";
        else
            result += ".    .\n.    .\n";
        if (seg_g)
            result += " gggg \n";
        else
            result += " .... \n";
        return result;
    }
    uint8_t toInteger()
    {
        if (segments == 0b01110111)
            return 0;
        if (segments == 0b00100100)
            return 1;
        if (segments == 0b01011101)
            return 2;
        if (segments == 0b01101101)
            return 3;
        if (segments == 0b00101110)
            return 4;
        if (segments == 0b01101011)
            return 5;
        if (segments == 0b01111011)
            return 6;
        if (segments == 0b00100101)
            return 7;
        if (segments == 0b01111111)
            return 8;
        if (segments == 0b01101111)
            return 9;
        return UINT8_MAX;
    }
    static std::string codeToString(uint8_t code)
    {
        bool seg_a = (code >> 0) & 1;
        bool seg_b = (code >> 1) & 1;
        bool seg_c = (code >> 2) & 1;
        bool seg_d = (code >> 3) & 1;
        bool seg_e = (code >> 4) & 1;
        bool seg_f = (code >> 5) & 1;
        bool seg_g = (code >> 6) & 1;
        std::string result;
        if (seg_a)
            result += "a";
        if (seg_b)
            result += "b";
        if (seg_c)
            result += "c";
        if (seg_d)
            result += "d";
        if (seg_e)
            result += "e";
        if (seg_f)
            result += "f";
        if (seg_g)
            result += "g";
        return result;
    }
    uint8_t segments{0};
    std::string segmentStr;
    unsigned char size{0};

};
