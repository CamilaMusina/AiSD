#include<iostream>
#include<string>
#include<cstdio>
#include <regex>


int main()
{
    std::regex size("^set_size \\d+$");
    std::regex push("^push [^\\s+]+$");
    std::regex pop("^pop$");
    std::regex print("^print$");

    std::string line;
    long long cap = 0, count = -1;
    std::string* data;
    line.reserve(10000000);
    while (std::getline(std::cin, line)) {
        if (line.empty())
            continue;
        else {
            if (std::regex_match(line, size)) {
                try
                {
                    cap = std::stoll(line.substr(9));
                    break;
                }
                catch (std::invalid_argument& err)
                {
                    std::cout << "error" << std::endl;
                }
            }
            else
                std::cout << "error" << std::endl;
        }
    }
    data = new std::string[cap];
    while (std::getline(std::cin, line)) {
        if (line.empty())
        {
            continue;
        }
        else {
            if (std::regex_match(line, push))
            {
                if (count == cap - 1)
                    std::cout << "overflow" << std::endl;
                else {
                    count++;
                    data[count] = line.substr(5);
                }
            }
            else if (std::regex_match(line, pop))
            {
                if (count == -1)
                    std::cout << "underflow" << std::endl;
                else 
                    std::cout << data[count--] << std::endl;
            }
            else if (std::regex_match(line, print))
            {
                if (count == -1)
                    std::cout << "empty" << std::endl;
                else {
                    std::cout << data[0];
                    for (size_t i = 1; i <= count; i++)
                        std::cout << " " << data[i];
                    std::cout << std::endl;
                }
            }
            else 
                std::cout << "error" << std::endl;
        }
    }
    delete[] data;
    return 0;
}

