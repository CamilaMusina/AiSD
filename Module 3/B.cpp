#include <iostream>
#include <vector>
#include <string>
#include <deque>

std::deque<int> toBinary(int cap) {
    std::deque<int> bin;
    while (cap != 0) {
        bin.push_back(cap % 2);
        cap /= 2;
    }
    return bin;
}

int test(std::deque<int> bin) {
    int count = 0;
    for (int i = 0; i < bin.size(); i++) 
        if (bin[i] == 1)
            count++;
    return count;
}

void run(int cap) {
    std::vector<std::string> q;
    bool flag = false;
    while (cap != 0) {
        if (cap % 2 == 0) {
            q.push_back("dbl");
            cap /= 2;
        }
        else {
            if (cap == 1 || cap == 3) {
                q.push_back("inc");
                cap--;
            }
            else {
                std::deque<int> inc = toBinary(cap + 1);
                std::deque<int> dec = toBinary(cap - 1);
                auto test1 = test(inc);
                auto test2 = test(dec);

                if (test1 <= test2) {
                    q.push_back("dec");
                    cap ++;
                } else {
                    q.push_back("inc");
                    cap--;
                }                
            }
        }
    }
    for (int i = q.size() - 1; i >= 0; i--)
        std::cout << q[i] << "\n";
}

int main()
{
    int cap;
    std::cin >> cap;
    run(cap);
}
