#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <stack>
#include <set>

int main()
{
    char g_type, s_type;
    std::string vertex, ver1, ver2;
    unsigned int n = 1000;
    std::unordered_map<std::string, std::set<std::string>> mtr;
    std::unordered_map<std::string, std::string> ::iterator it_mtr;
    std::cin >> g_type >> vertex >> s_type;

    while (std::cin >> ver1) {
        std::cin >> ver2;
        mtr[ver1].insert(ver2);
        if (g_type == 'u') 
            mtr[ver2].insert(ver1);
    }

    if (s_type == 'b') {
        std::queue<std::string> q;
        q.push(vertex);
        std::unordered_map<std::string, size_t> used;
        used[vertex] = 1;

        while (!q.empty()) {
            std::string v = q.front();
            q.pop();
            if (used[v] == 2)
                continue;
            std::cout << v << std::endl;
            used[v] = 2;

            for (auto x : mtr[v]) {
                if (used[x] != 2) {
                    used[x] = 1;
                    q.push(x);
                }
            }
        }
    } 

    if (s_type == 'd') {
        std::stack<std::string> st;
        st.push(vertex);
        std::unordered_map<std::string, size_t> used;
        used[vertex] = 1;

        while (!st.empty()) {
            std::string v = st.top();
            st.pop();
            if (used[v] == 2)
                continue;
            std::cout << v << std::endl;
            used[v] = 2;

            for (auto it = mtr[v].rbegin(); it != mtr[v].rend(); it++) {
                if (used[*it] != 2) {
                    used[*it] = 1;
                    st.push(*it);
                }
            }
        }
    }
}
