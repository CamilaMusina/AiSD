#include <iostream>
#include <sstream>
#include <string>
#include <queue>

struct Node {
    long long key;
    std::string value;
    Node* left;
    Node* right;
    Node* parent;

    Node() {
        this->key = NULL;
        this->value = "";
        this->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
    }

    Node(long long key, const std::string& value) :key(key), value(value) {
        this->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
    }

    /*~Node() {
        delete left;
        delete right;
    } */
};

class splayTree {
    Node* root;


public:
    splayTree() {
        this->root = nullptr;
    }

    ~splayTree() {
        deleter(root);
    }

    void deleter(Node* node) {
        if (node != nullptr) {
            deleter(node->left);
            deleter(node->right);
            delete node;
        }
    }

    Node* getRoot() {
        return root;
    }

    void zig(Node* node) {
        Node* p = node->parent;
        node->parent = nullptr;
        p->parent = node;
        Node* c = nullptr;

        if (p->left == node) {
            c = node->right;
            node->right = p;
            p->left = c;
        }
        else {
            c = node->left;
            node->left = p;
            p->right = c;
        }

        if (c != nullptr) {
            c->parent = p;
        }
    }

    void zigZig(Node* node) {
        Node* p = node->parent;
        Node* pp = p->parent;
        node->parent = pp->parent;
        p->parent = node;

        if (p->left == node) {
            Node* c = node->right;
            Node* pc = p->right;

            node->right = p;

            p->left = c;
            p->right = pp;

            pp->parent = p;
            pp->left = pc;

            if (node->parent != nullptr) {
                if (node->parent->left == pp)
                    node->parent->left = node;
                else
                    node->parent->right = node;
            }

            if (c != nullptr)
                c->parent = p;

            if (pc != nullptr)
                pc->parent = pp;
        }
        else {
            Node* c = p->left;
            Node* pc = node->left;

            node->left = p;

            p->left = pp;
            p->right = pc;

            pp->parent = p;
            pp->right = c;

            if (node->parent != nullptr) {
                if (node->parent->left == pp)
                    node->parent->left = node;
                else
                    node->parent->right = node;
            }

            if (c != nullptr)
                c->parent = pp;

            if (pc != nullptr)
                pc->parent = p;
        }
    }

    void zigZag(Node* node) {
        Node* p = node->parent;
        Node* pp = p->parent;
        Node* cl = node->left;
        Node* cr = node->right;
        node->parent = pp->parent;
        p->parent = node;
        pp->parent = node;

        if (p->right == node) {
            node->left = p;
            node->right = pp;
            p->right = cl;
            pp->left = cr;

            if (cl != nullptr)
                cl->parent = p;

            if (cr != nullptr)
                cr->parent = pp;
        }
        else {
            node->left = pp;
            node->right = p;
            p->left = cr;
            pp->right = cl;

            if (cl != nullptr)
                cl->parent = pp;

            if (cr != nullptr)
                cr->parent = p;
        }
        if (node->parent != nullptr) {
            if (node->parent->left == pp)
                node->parent->left = node;
            else
                node->parent->right = node;
        }
    }

    void Splay(Node* node) {
        while (node->parent != nullptr) {
            Node* p = node->parent;
            Node* pp = p->parent;
            if (pp == nullptr)
                zig(node);
            else if ((pp->left == p && p->left == node) || (pp->right == p && p->right == node))
                zigZig(node);
            else
                zigZag(node);
        }
        this->root = node;
    }

    Node* Search(long long key) {
        if (root == nullptr)
            return root;
        Node* node = this->root;
        Node* parent = nullptr;
        while (node != nullptr) {
            parent = node;
            if (node->key == key) {
                Splay(node);
                return node;
            }
            else {
                if (key < node->key)
                    node = node->left;
                else
                    node = node->right;
            }
        }
        Splay(parent);
        return nullptr;
    }

    Node* _search(Node* node, long long key) {
        if (!node || key == node->key)
            return node;
        if (key < node->key)
            return _search(node->left, key);
        else
            return _search(node->right, key);
    }

    bool Add(long long key, const std::string value) {
        if (this->root == nullptr) {
            delete this->root;
            this->root = new Node(key, value);
            return true;
        }
        if (Node* _node = _search(this->root, key)) {
            Splay(_node);
            return false;
        }
        else {
            Node* node = new Node(key, value);
            Node* cur = this->root;
            Node* prev = nullptr;

            while (cur) {
                prev = cur;
                if (node->key < cur->key)
                    cur = cur->left;
                else
                    cur = cur->right;
            }
            if (node->key < prev->key)
                prev->left = node;
            else
                prev->right = node;
            node->parent = prev;

            Splay(node);
            return true;
        }
    }

    bool Set(long long key, const std::string& value) {
        if (this->root == nullptr)
            return false;
        if (Search(key) != nullptr) {
            this->root->value = value;
            return true;
        }
        return false;
    }

    bool Delete(long long key) {
        if (root == nullptr)
            return false;
        Node* n = Search(key);
        if (n == nullptr)
            return false;
        else {
            if (this->root->right == nullptr && this->root->left == nullptr)
                this->root = nullptr;
            else if (this->root->right == nullptr) {
                this->root->left->parent = nullptr;
                this->root = this->root->left;
            }
            else if (this->root->left == nullptr) {
                this->root->right->parent = nullptr;
                this->root = this->root->right;
            }
            else {
                _max(this->root->left);
                this->root->right->parent = nullptr;
                this->root->right = this->root->right->right;
                this->root->right->parent = this->root;
            }
            delete n;
        }
        return true;
    }

    void Print(std::deque<Node*>* q) {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(NULL);

        std::deque<Node*> new_q;
        bool flag = false;
        std::string out;
        if (!root) {
            std::cout << "_\n";
            return;
        }

        while (q->size() != 0) {
            auto q_el = q->front();
            q->pop_front();

            if (q_el == nullptr) {
                new_q.push_back(nullptr);
                new_q.push_back(nullptr);
                out += "_ ";
                continue;
            }
            else if (q_el->parent)
                out += ('[' + std::to_string(q_el->key) + ' ' + q_el->value + ' ' +
                    std::to_string(q_el->parent->key) + "] ");
            else
                out += ('[' + std::to_string(q_el->key) + ' ' + q_el->value + "] ");



            if (q_el->left) {
                flag = true;
                new_q.push_back(q_el->left);
            }
            else
                new_q.push_back(nullptr);

            if (q_el->right) {
                flag = true;
                new_q.push_back(q_el->right);
            }
            else
                new_q.push_back(nullptr);
        }
        out.pop_back();
        std::cout << out << '\n';
        if (flag)
            Print(&new_q);
        return;
    }

    void _max(Node* node) {
        if (this->root == nullptr) {
            return;
        }
        while (node->right != nullptr)
            node = node->right;
        Splay(node);
    }

    bool max(Node* node) {
        if (this->root == nullptr) {
            return false;
        }
        while (node->right != nullptr)
            node = node->right;
        Splay(node);
        std::cout << node->key << " " << node->value << "\n";
        return true;
    }

    bool min(Node* node) {
        if (this->root == nullptr) {
            return false;
        }
        while (node->left != nullptr)
            node = node->left;
        Splay(node);
        std::cout << node->key << " " << node->value << "\n";
        return true;
    }
};

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    splayTree tree;
    std::string line;
    line.reserve(10000000);
    while (std::getline(std::cin, line)) {
        if (line.empty())
            continue;
        else {
            if (line.size() > 3 && line[0] == 'a') {
                std::vector<std::string> elems = split(line, ' ');
                if (elems.size() < 3)
                    elems.push_back("");
                if (!tree.Add(std::stoll(elems[1]), elems[2]))
                    std::cout << "error\n";             
            }
            else if (line.size() > 3 && line[2] == 't') {
                std::vector<std::string> elems = split(line, ' ');
                if (elems.size() < 3)
                    elems.push_back("");
                if (!tree.Set(std::stoll(elems[1]), elems[2]))
                    std::cout << "error\n";
            }
            else if (line.size() == 3 && line[1] == 'a') {
                if (!tree.max(tree.getRoot()))
                    std::cout << "error\n";
            }
            else if (line.size() == 3 && line[1] == 'i') {
                if (!tree.min(tree.getRoot()))
                    std::cout << "error\n";
            }

            else if (line.size() > 6 && line[0] == 'd') {
                std::vector<std::string> elems = split(line, ' ');
                if (!tree.Delete(std::stoll(elems[1])))
                    std::cout << "error\n";
            }
            else if (line.size() > 6 && line[0] == 's') {
                std::vector<std::string> elems = split(line, ' ');
                if (auto node = tree.Search(std::stoll(elems[1])))
                    std::cout << "1 " << node->value << std::endl;
                else
                    std::cout << "0\n";
            }
            else if (line.size() == 5 && line[0] == 'p') {
                std::deque<Node*> q;
                q.push_back(tree.getRoot());
                tree.Print(&q);
            }
            else
                std::cout << "error\n";
        }
    }
}
