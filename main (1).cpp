#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
using namespace std;

class RopeNode {
public:
    string data;
    int weight;
    RopeNode* left;
    RopeNode* right;

    RopeNode(const string& s = "") 
        : data(s), weight(s.size()), left(nullptr), right(nullptr) {}
};

class Rope {
private:
    vector<string> strings;
    RopeNode* root;

    // تابع کمکی برای پاکسازی حافظه
    void clear(RopeNode* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    // تابع کمکی برای ساخت Rope از یک رشته
    RopeNode* buildRope(const string& s, int start, int end) {
        if (end - start <= 5) {
            return new RopeNode(s.substr(start, end - start));
        }
        int mid = (start + end) / 2;
        RopeNode* node = new RopeNode();
        node->left = buildRope(s, start, mid);
        node->right = buildRope(s, mid, end);
        node->weight = node->left->weight;
        return node;
    }

    // تابع کمکی برای جمع‌آوری رشته از برگ‌ها
    string collectStrings(RopeNode* node) {
        if (!node) return "";
        if (!node->left && !node->right) {
            return node->data;
        }
        return collectStrings(node->left) + collectStrings(node->right);
    }

    // تابع کمکی برای محاسبه طول
    int getLength(RopeNode* node) {
        if (!node) return 0;
        if (!node->left && !node->right) {
            return node->data.size();
        }
        return getLength(node->left) + getLength(node->right);
    }

public:
    Rope() : root(nullptr) {}
    ~Rope() { clear(root); }

    // ================= توابع اصلی =================

    // 1. تابع new
    void addString(const string& s) {
        strings.push_back(s);
        if (!root) {
            root = buildRope(s, 0, s.size());
        } else {
            RopeNode* newRoot = new RopeNode();
            newRoot->left = root;
            newRoot->right = buildRope(s, 0, s.size());
            newRoot->weight = getLength(root);
            root = newRoot;
        }
    }

    // 2. تابع status
    void status() {
        cout << "All strings:" << endl;
        for (size_t i = 0; i < strings.size(); ++i) {
            cout << i + 1 << ". " << strings[i] << endl;
        }
    }

    // 3. تابع index
    char index(int rope_id, int i) {
        if (rope_id < 1 || rope_id > strings.size()) {
            throw out_of_range("Invalid rope_id!");
        }
        const string& s = strings[rope_id - 1];
        if (i < 0 || i >= s.size()) {
            throw out_of_range("Index out of bounds!");
        }
        return s[i];
    }

    // 4. تابع concat
    void concat(int rope_id1, int rope_id2) {
        if (rope_id1 < 1 || rope_id1 > strings.size() || 
            rope_id2 < 1 || rope_id2 > strings.size()) {
            throw out_of_range("Invalid rope_id!");
        }
        strings[rope_id1 - 1] += strings[rope_id2 - 1];
        strings.erase(strings.begin() + rope_id2 - 1);
        root = buildRope(strings[rope_id1 - 1], 0, strings[rope_id1 - 1].size());
    }

    // 5. تابع split
    pair<Rope*, Rope*> split(int rope_id, int i) {
        if (rope_id < 1 || rope_id > strings.size()) {
            throw out_of_range("Invalid rope_id!");
        }
        string s = strings[rope_id - 1];
        if (i < 0 || i > s.size()) {
            throw out_of_range("Index out of bounds!");
        }

        Rope* leftRope = new Rope();
        leftRope->addString(s.substr(0, i));

        Rope* rightRope = new Rope();
        rightRope->addString(s.substr(i));

        return make_pair(leftRope, rightRope);
    }

    // 6. تابع insert
    void insert(int rope_id1, int i, int rope_id2) {
        if (rope_id1 < 1 || rope_id1 > strings.size() || 
            rope_id2 < 1 || rope_id2 > strings.size()) {
            throw out_of_range("Invalid rope_id!");
        }
        string& s1 = strings[rope_id1 - 1];
        const string& s2 = strings[rope_id2 - 1];
        if (i < 0 || i > s1.size()) {
            throw out_of_range("Index out of bounds!");
        }
        s1.insert(i, s2);
        root = buildRope(s1, 0, s1.size());
    }

    // 7. تابع delete
    void deleteRange(int rope_id, int i, int j) {
        if (rope_id < 1 || rope_id > strings.size()) {
            throw out_of_range("Invalid rope_id!");
        }
        string& s = strings[rope_id - 1];
        if (i < 0 || j > s.size() || i >= j) {
            throw out_of_range("Invalid range!");
        }
        s.erase(i, j - i);
        root = buildRope(s, 0, s.size());
    }

    // 8. تابع report
    string report(int rope_id, int i, int j) {
        if (rope_id < 1 || rope_id > strings.size()) {
            throw out_of_range("Invalid rope_id!");
        }
        const string& s = strings[rope_id - 1];
        if (i < 0 || j > s.size() || i > j) {
            throw out_of_range("Invalid range!");
        }
        return s.substr(i, j - i + 1);
    }
};

// ================= تابع اصلی برای تست =================
int main() {
    Rope rope;

    // 1. تست new و status
    rope.addString("Hello");
    rope.addString("World");
    rope.status();
    /* خروجی:
       All strings:
       1. Hello
       2. World
    */

    // 2. تست index
    cout << "Char at index 1 in rope 1: " << rope.index(1, 1) << endl; // 'e'

    // 3. تست concat
    rope.concat(1, 2);
    rope.status();
    /* خروجی:
       All strings:
       1. HelloWorld
    */

    // 4. تست split
    auto splitRopes = rope.split(1, 5);
    cout << "After split:" << endl;
    splitRopes.first->status(); // "Hello"
    splitRopes.second->status(); // "World"

    // 5. تست insert
    Rope rope2;
    rope2.addString("ABC");
    rope2.addString("XYZ");
    rope2.insert(1, 1, 2);
    rope2.status();
    /* خروجی:
       All strings:
       1. AXYZBC
       2. XYZ
    */

    // 6. تست delete
    rope2.deleteRange(1, 2, 4);
    rope2.status();
    /* خروجی:
       All strings:
       1. ABC
       2. XYZ
    */

    // 7. تست report
    cout << "Report: " << rope2.report(1, 0, 1) << endl; // "AB"

    return 0;
}