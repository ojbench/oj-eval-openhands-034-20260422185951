#ifndef SKIP_LIST_HPP
#define SKIP_LIST_HPP

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>

template<typename T>
class SkipList {
private:
    struct Node {
        T value;
        std::vector<Node*> forward;
        explicit Node(int level) : forward(level, nullptr) {}
        Node(const T& val, int level) : value(val), forward(level, nullptr) {}
    };

    // Maximum number of levels (sufficient for up to ~2^32 elements)
    static constexpr int MAX_LEVEL = 32;
    // Probability factor for level increase (1/2)
    static constexpr int P_DEN = 2;

    int level;           // Current highest level (1-based)
    Node* header;        // Header sentinel node

    int randomLevel() {
        int lvl = 1;
        while (lvl < MAX_LEVEL && (std::rand() % P_DEN) == 0) ++lvl;
        return lvl;
    }

    static bool less(const T& a, const T& b) { return a < b; }
    static bool equal(const T& a, const T& b) { return !less(a, b) && !less(b, a); }

public:
    SkipList() : level(1) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        header = new Node(MAX_LEVEL);
    }

    ~SkipList() {
        Node* cur = header->forward[0];
        while (cur) {
            Node* nxt = cur->forward[0];
            delete cur;
            cur = nxt;
        }
        delete header;
    }

    // Insert a value into the skip list. If the value already exists, do nothing.
    void insert(const T & item) {
        std::vector<Node*> update(MAX_LEVEL, nullptr);
        Node* cur = header;
        for (int i = level - 1; i >= 0; --i) {
            while (cur->forward[i] && less(cur->forward[i]->value, item)) cur = cur->forward[i];
            update[i] = cur;
        }
        cur = cur->forward[0];
        if (cur && equal(cur->value, item)) return; // already exists

        int lvl = randomLevel();
        if (lvl > level) {
            for (int i = level; i < lvl; ++i) update[i] = header;
            level = lvl;
        }
        Node* newNode = new Node(item, lvl);
        for (int i = 0; i < lvl; ++i) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }

    // Search for a value in the skip list
    bool search(const T & item) {
        Node* cur = header;
        for (int i = level - 1; i >= 0; --i) {
            while (cur->forward[i] && less(cur->forward[i]->value, item)) cur = cur->forward[i];
        }
        cur = cur->forward[0];
        return (cur && equal(cur->value, item));
    }

    // Delete a value from the skip list. If the value does not exist in the skip list, do nothing.
    void deleteItem(const T & item) {
        std::vector<Node*> update(MAX_LEVEL, nullptr);
        Node* cur = header;
        for (int i = level - 1; i >= 0; --i) {
            while (cur->forward[i] && less(cur->forward[i]->value, item)) cur = cur->forward[i];
            update[i] = cur;
        }
        cur = cur->forward[0];
        if (!cur || !equal(cur->value, item)) return; // not found

        for (int i = 0; i < level; ++i) {
            if (update[i]->forward[i] == cur) {
                update[i]->forward[i] = cur->forward[i];
            }
        }
        delete cur;
        while (level > 1 && header->forward[level - 1] == nullptr) --level;
    }
};

#endif
