#include <iostream>
#include <fstream>
#include <string>
#include <regex>


int main(int argc, char* argv[])
{
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);

    std::regex size("^set_size \\d+$");
    std::regex push("^push [^\\s+]+$");
    std::regex pop("^pop$");
    std::regex print("^print$");

    std::string line;
    long long cap = 0;
    int first = 0, last = 0;
    std::string* data;
    line.reserve(10000000);
    while (std::getline(fin, line))
    {
        if (line.empty())
        {
            continue;
        }
        else
        {
            if (std::regex_match(line, size))
            {
                try
                {
                    cap = std::stoll(line.substr(9));
                    break;
                }
                catch (std::invalid_argument& err)
                {
                    fout << "error" << std::endl;
                }
            }
            else {
                fout << "error" << std::endl;
            }
        }
    }

    data = new std::string[cap];
    while (std::getline(fin, line))
    {
        if (line.empty())
            continue;
        else
        {
            if (std::regex_match(line, push))
            {
                if (last < cap) {
                    data[(first + last) % cap] = line.substr(5);
                    last++;
                }
                else
                    fout << "overflow" << std::endl;
            }
            else if (std::regex_match(line, pop))
            {
                if (last == 0)
                    fout << "underflow" << std::endl;
                else {
                    fout << data[first] << std::endl;
                    last--;
                    first++;
                    if (first == cap)
                        first = 0;
                }
            }
            else if (std::regex_match(line, print))
            {
                if (last == 0)
                    fout << "empty" << std::endl;
                else {
                    for (int i = 0; i < last - 1; i++)
                        fout << data[(first + i) % cap] << " ";
                    fout << data[(first + last - 1) % cap] << std::endl;
                }
            }
            else
                fout << "error" << std::endl;
        }
    }
    delete[] data;

    return 0;
}
