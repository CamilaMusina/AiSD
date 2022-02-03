#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <queue>
#include <algorithm>
#include <cctype>
#include <clocale>
#include <locale>
#include <codecvt>
#include <cwctype>
#include <functional>
#include <cassert>
#include <sstream>

int DamerauLevenstein(std::wstring word1, std::wstring word2);
std::wstring toLower(const std::wstring& line);
std::ostream& operator<<(std::ostream& stream, std::wstring s);


class Tree {

    struct Word {
        std::wstring word;
        std::unordered_map<int, Word*> childnodes;

        Word() {}
        Word(std::wstring word) :word(word) {}

        void __setmap(int dist, Word* word) {
            childnodes[dist] = word;
        }
    };

    Word* root;
    bool _root;

    void deleter(Word* node) {
        if (node != nullptr) {
            for (auto it : node->childnodes) {
                deleter(it.second);
            }
            delete node;
        }
    }

public:
    Tree() {
        this->root = nullptr;
        this->_root = false;
    }

    ~Tree() {
        deleter(this->root);
    }

    void Add(std::wstring word, Word* node) {
        /*
            Алгоритм рекурсивно добавляет новое слово в дерево.
            Работает за O(K) в худшем случае, где K - кол-во слов в дереве.
            При каждом вызове вызывает алгоритм Дамерау-Левенштейна,
            сложность которого O(N*M), где N и M - длины сравниваемых слов
        */
        if (!node) {
            if (!this->_root) {
                this->root = new Word(word);
                this->_root = true;
                return;
            }
            Add(word, this->root);
            return;
        }

        int dist = DamerauLevenstein(word, node->word);
        if (dist == 0)
            return;

        bool flag = false;
        for (auto x : node->childnodes)
            if (dist == x.first) {
                Add(word, x.second);
                flag = true;
            }
        if (!flag) {
            auto _word = new Word(word);
            node->__setmap(dist, _word);
            return;
        }
    }

    std::vector<std::wstring> correction(std::wstring word) {
        /*
            Алгоритм находит массив слов, содержащих не более одной ошибки с заданным словом.
            При поиске похожих по написанию слов на исходное сначала посещаем вершины,
            которые могут содержать это слово: для текущей вершины в дереве - это вершина
            с таким же расстоянием Дамерау-Левенштейна.
            В худшем случае за O(K) мы проверяем каждую вершину дерева [K - кол-во слов в дереве],
            для каждой такой вершины вызывается алгоритм Дамерау-Левенштейна,
            сложность которого O(N*M), где N, M - длины сравнивааемых слов.
        */

        if (!this->root)
            return std::vector<std::wstring>();
        word = toLower(word);
        std::vector<std::wstring> incorrect;
        std::deque<Word*> Q;
        Q.push_back(this->root);
        int k = 2;

        while (Q.size() != 0) {
            auto cur = Q.front();
            Q.pop_front();
            if (cur->word == word) {
                std::vector<std::wstring> out(1);
                out[0] = word;
                return out;
            }

            auto dist = DamerauLevenstein(word, cur->word);
            if (dist <= k)
                incorrect.push_back(cur->word);
            for (auto x : cur->childnodes) {
                if ((dist - k <= x.first) && (x.first <= dist + k)) {
                    if (x.first == dist)
                        Q.push_front(cur->childnodes[x.first]);
                    else
                        Q.push_back(cur->childnodes[x.first]);
                }
            }
        }
        if (incorrect.size() == 0)
            return std::vector<std::wstring>();
        else
            return incorrect;
    }
};

int DamerauLevenstein(std::wstring word1, std::wstring word2) {
    /*
        Алгоритм Дамерау-Левенштейна находит минимальное число операций
        вставки, удаления, замены одного символа и транспозиции двух соседних символов,
        необходимых для перевода одной строки в другую.
        Пусть M и N - длины слов m и n соответственно.
        Мы будем хранить матрицу размера [M*N], элементы которой являются
        расстояниями между первыми i символами слова m и j символами слова n.
        Сложность такого алгоритма O(M*N)
    */

    int size1 = word1.size();
    int size2 = word2.size();
    std::vector<std::vector<int>>dist(size1 + 2, std::vector<int>(size2 + 2, 0));
    int maxdist = size1 + size2;
    int dist1, dist2, dist3;
    std::map<wchar_t, int> mp;

    dist[0][0] = maxdist;
    for (auto i = 1; i < size1 + 2; i++) {
        dist[i][1] = i;
        dist[i][0] = maxdist;
    }
    for (auto j = 1; j < size2 + 2; j++) {
        dist[1][j] = j;
        dist[0][j] = maxdist;
    }
    for (auto i = 1; i < size1 + 1; i++) {
        dist1 = 0;
        for (auto j = 1; j < size2 + 1; j++) {
            dist2 = mp[word2[j - 1]];
            dist3 = dist1;
            int temp = 1;
            if (word1[i - 1] == word2[j - 1]) {
                temp = 0;
                dist1 = j;
            }

            dist[i + 1][j + 1] = std::min(std::min(dist[i][j] + temp, dist[i + 1][j] + 1),
                std::min(dist[i][j + 1] + 1, dist[dist2][dist3] + (i - dist2 - 1) + (j - dist3 - 1) + 1));
        }
        mp[word1[i - 1]] = i;
    }
    return dist[size1 + 1][size2 + 1];
}

void correctionControl(std::wstring word, std::vector<std::wstring> out) {
    /*
        Вывод введенного слова и результат проверки на наличие в словаре.
    */
    std::ios_base::sync_with_stdio(false);
    //std::locale loc{ "" };
    std::wcout.imbue(std::locale("rus_rus.1251"));
    std::wcin.imbue(std::locale(""));

    if (out.size() == 0) {
        std::cout << word << " -?\n";
        return;
    }
    std::wstring _word = word;
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    word = toLower(word);
    if (out[0] == word)
        std::cout << _word << " - ok\n";
    else {
        std::sort(out.begin(), out.end());
        std::wstring incorr;
        for (auto x : out) {
            incorr += x;
            incorr += converter.from_bytes(", ");
        }
        incorr.pop_back();
        incorr.pop_back();
        std::cout << _word << " -> " << incorr << "\n";

    }
}

std::wstring toLower(const std::wstring& line) {
    /*
        Алгоритм переводит символы из верхнего регистра в нижний.
        Сложность O(k), где k - длина слова
    */

    std::wstring word = line;
    for (auto i = 0; i < line.size(); i++) {
        if (isupper((wchar_t)line[i])) // ASCII
            word[i] = std::towlower(line[i]);
        else if ((wchar_t)line[i] > 127 && (wchar_t)line[i] < 144)
            word[i] = (wchar_t)line[i] + 32;
        else if ((wchar_t)line[i] > 143 && (wchar_t)line[i] < 160)
            word[i] = (wchar_t)line[i] + 80;
        else if ((wchar_t)line[i] == 240)
            word[i] = (wchar_t)line[i] + 1;
    }
    return word;
}

std::ostream& operator<<(std::ostream& stream, std::wstring line) {
    for (auto i = 0; i < line.size(); i++) {
      //  if ((wchar_t)line[i]) // ASCII
      //      stream << (char)line[i];
        if ((wchar_t)line[i] > 127 && (wchar_t)line[i] < 242)
            stream << (char)line[i];
       // else if ((wchar_t)line[i] > 143 && (wchar_t)line[i] < 160)
       //     stream << (char)line[i] + 80;
       // else if ((wchar_t)line[i] == 240 || (wchar_t)line[i] == 241)
       //     stream << (char)line[i];
        else
            stream << (char)line[i];
    }

   /* for (auto c : line) {
        if (c == 'ё')
            stream << wchar_t(241);
        else if (c == 'Ё')
            stream << wchar_t(240);
        else if (c >= -64 && c <= -17)
            stream << wchar_t(c + 64 + 128);
        else if (c < 0)
            stream << wchar_t(c + 64 + 176);
        else
            stream << c; 
    } */
    return stream;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::locale loc{ "" };
    std::cin.imbue(loc);
    std::cout.imbue(loc);

    Tree tree;
    std::wstring line;
    int size = 0, i = 0;
    while (std::getline(std::wcin, line)) {
        if (line.empty())
            continue;
        if (size == 0) {
            auto it = line.begin();
            while (it != line.end() && std::isdigit(*it)) ++it;
            if (it == line.end())
                size = std::stoi(line);
            else
                std::cout << "error\n";
        }
        else {
            if (i < size) {
                line = toLower(line);
                tree.Add(line, nullptr);
                i++;
            }
            else
                correctionControl(line, tree.correction(line));
        }
    }
}
