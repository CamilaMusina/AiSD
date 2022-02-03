#include <iostream>
#include <string>
#include <map>
#include <regex>
#include <deque>
#include <vector>

struct Node {
    int key;
    std::string value;

    Node() :key(NULL), value("") {}
    Node(int key, std::string& value) :key(key), value(std::move(value)) {}
};

class BinMinHeap {
    std::vector<Node> heap;
    std::vector<int> index;
    //std::map<int, int> index;
    //int cap;
    int size;

public:
    BinMinHeap():size(0) {}

    static int parent(int i) {
        return (i - 1) / 2;
    }

    static int leftChild(int i) {
        return 2 * i + 1;
    }

    static int rightChild(int i) {
        return 2 * i + 2;
    }

    Node* first() {
        return &heap[0];
    }

    void swap(int first, int second) {
        index[heap[first].key] = second;
        index[heap[second].key] = first;
        auto temp = heap[first];
        heap[first] = heap[second];
        heap[second] = temp;
    } 

    void heapify(int index) {
        
        int left = leftChild(index);
        int right = rightChild(index);
        int temp = index;
        if (left < heap.size() && heap[left].key < heap[temp].key)
            temp = left;
        if (right < heap.size() && heap[right].key < heap[temp].key)
            temp = right;
        if (temp != index) {
            swap(heap[index].key, heap[temp].key);
            heapify(temp);
        }
        

    }

    bool Add(int key, std::string& value) {
       /* std::map<int, int>::iterator findKey = index.find(key);
        if (findKey == index.end()) {
            Node node(key, value);
            heap.push_back(node);
            index[key] = heap.size() - 1;
            heapify(index[key], true, false);
            return true;
        }
        return false; */

        std::vector<int>::iterator findKey = std::find(index.begin(), index.end(), key);
        if (findKey == index.end()) {
            Node node(key, value);
            heap.push_back(node);
            int _index = heap.size() - 1;
            index.resize(heap.size());
            index[_index] = key;
            while (_index != 0 && heap[parent(_index)].key > heap[_index].key) {
                swap(heap[_index].key, heap[parent(_index)].key);
                _index = parent(_index);
            }
            return true;
        }
        return false;
    }

    bool Set(int key, std::string& value) {
        std::vector<int>::iterator findKey = std::find(index.begin(), index.end(), key);
        if (findKey == index.end())
            return false;
        heap[index[key]].value = value;
        return true;
    }

    bool Delete(int key) {
        std::vector<int>::iterator findKey = std::find(index.begin(), index.end(), key);
        heap[key].key = INT_MIN;
        if (findKey == index.end())
            return false;
        while (key != 0 && heap[(parent(key))].key > heap[key].key) {
            swap(heap[key].key, heap[(parent(key))].key);
            key = parent(key);
        }
        index[0] = index[heap.size() - 1];
        heap.pop_back();
        heapify(0);
        return true;
    }

    Node* Search(int key) {
        std::vector<int>::iterator findKey = std::find(index.begin(), index.end(), key);
        if (findKey == index.end())
            return nullptr;
        else {
            Node* node = new Node(index[key], heap[index[key]].value);
            //node->key = index[key];
            //node->value = heap[index[key]].value;
            return node;
        }
    }

    bool min() {
        if (heap.size() == 0)
            return false;
        std::cout << heap[0].key << " 0 " << heap[0].value << "\n";
        return true;
    }

    bool max() {
        if (heap.size() == 0)
            return false;
        auto maxKey = *std::max_element(index.begin(), index.end());
        std::cout << maxKey << ' ' << index[maxKey] << ' ' << heap[index[maxKey]].value << "\n";
        return true;
    }

    bool Extract() {
        if (heap.size() == 0)
            return false;
        std::cout << heap[0].key << " " << heap[0].value << "\n";
        Delete(heap[0].key);
        return true;
    }

    void Print(std::deque<int>* Q) {
        if (heap.size() == 0) {
            std::cout << "_\n";
            return;
        }

        std::deque<int> newQ;
        std::string out;
        bool flag = false;
        while (Q->size() != 0) {
            auto Q_el = Q->front();
            Q->pop_front();
            if (Q_el == -1) {
                out += "_ ";
                continue;
            }
            else if (Q_el == 0)
                out += ('[' + std::to_string(heap[Q_el].key) + ' '
                    + heap[Q_el].value + "] ");
            else
                out += ('[' + std::to_string(heap[Q_el].key) + ' '
                    + heap[Q_el].value + ' ' + std::to_string(heap[parent(Q_el)].key) + "] ");

            if (leftChild(Q_el) <= heap.size() - 1) {
                flag = true;
                newQ.push_back(leftChild(Q_el));
            }
            else
                newQ.push_back(-1);
            if (rightChild(Q_el) <= heap.size() - 1) {
                flag = true;
                newQ.push_back(rightChild(Q_el));
            }
            else
                newQ.push_back(-1);
        }
        out.pop_back();
        std::cout << out << '\n';
        if (flag)
            Print(&newQ);
        return;
    }
};

int main()
{
    std::regex add("^add$");
    std::regex set("^set$");
    std::regex reg_delete("^delete$");
    std::regex search("^search$");
    std::regex min("^min$");
    std::regex max("^max$");
    std::regex extract("^extract$");
    std::regex print("^print$");

    BinMinHeap binheap;
    std::string line;
    int key;
    line.reserve(10000000);
    while (std::cin >> line) {
        if (line.empty())
            continue;
        else {
            if (std::regex_match(line, add)) {
                int key;
                std::string value;
                std::cin >> key >> value;
                if (!binheap.Add(key, value))
                    std::cout << "error\n";
            }
            else if (std::regex_match(line, set)) {
                int key;
                std::string value;
                std::cin >> key >> value;
                if (!binheap.Set(key, value))
                    std::cout << "error\n";
            }
            else if (std::regex_match(line, max)) {
                if (!binheap.max())
                    std::cout << "error\n";
            }
            else if (std::regex_match(line, min)) {
                if (!binheap.min())
                    std::cout << "error\n";
            }

            else if (std::regex_match(line, reg_delete)) {
                int key;
                std::cin >> key;
                if (!binheap.Delete(key))
                    std::cout << "error\n";
            }
            else if (std::regex_match(line, search)) {
                int key;
                std::cin >> key;
                if (auto node = binheap.Search(key))
                    std::cout << "1 " << node->key << ' ' << node->value << std::endl;
                else
                    std::cout << "0\n";
            }
            else if (std::regex_match(line, extract)) {
                if (!binheap.Extract())
                    std::cout << "error\n";
            }
            else if (std::regex_match(line, print)) {
                std::deque<int> q;
                q.push_back(0);
                binheap.Print(&q);
            }
            else
                std::cout << "error\n";
        }
    }
}

