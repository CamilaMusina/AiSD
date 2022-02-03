#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
#include <regex>

struct Result {
    int weight;
    int value;
    std::vector<int> result;

    void print() {
        std::cout << this->weight << ' ' << this->value << "\n";
        for (auto x : this->result)
            std::cout << x << "\n";
    }
};

class Knapsack {
    int max_weight;
    int value;
    int weight;
    int nod;
    std::vector<int> result;

    int _nod(int x, int y) {
        while (x && y) {
            if (x >= y)
                x %= y;
            else
                y %= x;
        }
        return x | y;
    }

    int get_nod(std::vector<int>& w) {
        if (w.size() == 1)
            return w[0];
        int res = _nod(w[0], w[1]);
        for (int i = 2; i < w.size(); i++)
            res = _nod(res, w[i]);
        return res;
    }

    std::vector<int> sort(std::vector<int>& w, int _nod) {
        for (int i = 0; i < w.size(); i++)
            w[i] = w[i] / _nod;
        return w;
    }

    void res(std::vector<std::vector<int>> mtr, int k, int max_w, std::vector<int> w) {
        if (mtr[k][max_w] == 0)
            return;
        if (mtr[k - 1][max_w] == mtr[k][max_w])
            this->res(mtr, k - 1, max_w, w);
        else {
            this->res(mtr, k - 1, max_w - w[k - 1], w);
            this->weight += w[k - 1] * this->nod;
            result.push_back(k);
        }
    }

public:

    Knapsack() {
        this->value = 0;
        this->weight = 0;
        this->nod = 0;
        this->max_weight = -1;
    }
    Knapsack(int max_weight) :max_weight(max_weight), nod(1) {}

    int getMax() {
        return this->max_weight;
    }

    void run(std::vector<int>& v, std::vector<int>& w) {
        this->nod = this->get_nod(w);
        int max_w = this->max_weight / this->nod;
        w = sort(w, this->nod);

        std::vector<std::vector<int>> mtr;
        mtr.reserve(w.size());
        std::vector<int> temp(max_w + 1, 0);
        for (int i = 0; i <= w.size(); i++)
            mtr.push_back(temp);

        for (int i = 1; i <= w.size(); i++)
            for (int j = 0; j <= max_w; j++) {
                if (w[i - 1] <= j)
                    mtr[i][j] = std::max(mtr[i - 1][j], mtr[i - 1][j - w[i - 1]] + v[i - 1]);
                else
                    mtr[i][j] = mtr[i - 1][j];
            }

        this->value = mtr[w.size()][max_w];
        this->weight = 0;
        res(mtr, w.size(), max_w, w);
    }

    Result res() {
        Result answer;
        answer.weight = this->weight;
        answer.value = this->value;
        answer.result = this->result;
        return answer;
    }
};

std::vector<std::string> split(const std::string& str, char del)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, del))
        tokens.push_back(token);
    return tokens;
}

int main()
{
    Knapsack knapsack;
    std::vector<int> values;
    std::vector<int> weights;
    std::string line;
    std::regex reg1("^\\d+$");
    std::regex reg2("^\\d+ \\d+$");

    while (std::getline(std::cin, line)) {
        if (line.empty())
            continue;
        if (knapsack.getMax() == -1) {
            if (std::regex_match(line, reg1))
                knapsack = Knapsack(std::stoi(line));
            else
                std::cout << "error\n";
        }
        else {
            if (std::regex_match(line, reg2)) {
                std::vector<std::string> tokens;
                tokens = split(line, ' ');
                weights.push_back(stoi(tokens[0]));
                values.push_back(stoi(tokens[1]));
            }
            else
                std::cout << "error\n";
        }
    }
    Result answer;
    knapsack.run(values, weights);
    answer = knapsack.res();
    answer.print();
}
