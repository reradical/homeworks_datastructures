#include "ArgumentManager.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <string>

using namespace std;

// pq nodes
struct node {
    string val;
    int priority;
    node *next;
    node(string v, int p) : val(v), priority(p), next(nullptr) {}
};

// bst nodes
class BSTNode {
public:
    string value;
    BSTNode *left, *right;

    BSTNode(string v) : value(v), left(nullptr), right(nullptr) {}
};

// bst
class BST {
private:
    BSTNode *root;

public:
    BST() : root(nullptr) {}

    void insert(string val) {
        root = insertIteratively(root, val);
    }

    BSTNode *insertIteratively(BSTNode *root, string val) {
        BSTNode *temp = new BSTNode(val);
        if (root == nullptr) {
            return temp;
        }
        BSTNode *cur = root;
        while (cur != nullptr) {
            if (val < cur->value) {
                if (cur->left == nullptr) {
                    cur->left = temp;
                    return root;
                } else {
                    cur = cur->left;
                }
            } else if (val > cur->value) {
                if (cur->right == nullptr) {
                    cur->right = temp;
                    return root;
                } else {
                    cur = cur->right;
                }
            } else {
                break;
            }
        }
        return root;
    }

    void preorderTraversal(BSTNode* node, ofstream &file) {
        if (node) {
            file << node->value << endl;
            preorderTraversal(node->left, file);
            preorderTraversal(node->right, file);
        }
    }

    void inorderTraversal(BSTNode* node, ofstream &file) {
        if (node) {
            inorderTraversal(node->left, file);
            file << node->value << endl;
            inorderTraversal(node->right, file);
        }
    }

    void postorderTraversal(BSTNode* node, ofstream &file) {
        if (node) {
            postorderTraversal(node->left, file);
            postorderTraversal(node->right, file);
            file << node->value << endl;
        }
    }

    BSTNode* getRoot() {
        return root;
    }
};

// priority Queue class
class priorityQueue {
private:
    node *front;
    BST bst; // bst
    queue<string> decode; // regular queue (rq) for dequeue
    string order; // pre post in

public:
    priorityQueue() : front(nullptr) {}

    void addToPQ(string val, int pri) {
        node* tmp = new node(val, pri);

        if ((!front) || (pri < front->priority)) {
            tmp->next = front;
            front = tmp;
        }
        else {
            node* cu = front;
            while (cu->next && cu->next->priority <= pri) {
                cu = cu->next;
            }
            tmp->next = cu->next;
            cu->next = tmp;
        }
    }

    void actuallyReplace(string &change, string og, string r) {
        for (int i = 0; i < change.length(); i++) {
            if (change[i] == og[0]) {
                change[i] = r[0];
            }
        }
        decode.push(change);
    }

    void actuallyAdd(string &or_string, string og, string r) {
        string new_string;
        for (int i = 0; i < or_string.size(); i++) {
            new_string += or_string[i];
            if (or_string[i] == og[0]) {
                new_string += r[0];
            }
        }
        decode.push(new_string);
    }

    void actuallyRemove(string &or_string, string r) {
        string new_string;
        char to_rem = r[0];

        for (int i = 0; i < or_string.size(); i++) {
            if (or_string[i] != to_rem) {
                new_string += or_string[i];
            }
        }
        decode.push(new_string);
    }

    void actuallySwap(string &or_string, string o, string t) {
        for (int i = 0; i < or_string.size(); i++) {
            if (or_string[i] == o[0]) {
                or_string[i] = t[0];
            }
            else if (or_string[i] == t[0]) {
                or_string[i] = o[0];
            }
        }
        decode.push(or_string);
    }

    void goThrough() {
        node* cu = front;
        while (cu->next) {
            commands(cu->val);
            cu = cu->next;
        }
        commands(cu->val);
    }

    void commands(string line) {
        if (decode.empty()) {
            return;
        }
        size_t start, end, bstart, bend;
        int priority;

        if (line.find("REPLACE") != string::npos) {
            bstart = line.find("[");
            bend = line.find(",");
            string rep = line.substr(bstart + 1, bend - bstart - 1);
            bstart = line.find(",", bend);
            bend = line.find("]", bstart);
            string lace = line.substr(bstart + 1, bend - bstart - 1);

            string d = decode.front();
            decode.pop();
            actuallyReplace(d, rep, lace);
        }
        else if (line.find("ADD") != string::npos) {
            bstart = line.find("[");
            bend = line.find(",");
            string target = line.substr(bstart + 1, bend - bstart - 1);
            bstart = line.find(",", bend);
            bend = line.find("]", bstart);
            string n = line.substr(bstart + 1, bend - bstart - 1);

            string msg = decode.front();
            decode.pop();
            actuallyAdd(msg, target, n);
        }
        else if (line.find("REMOVE") != string::npos) {
            bstart = line.find("[");
            bend = line.find("]");
            string target = line.substr(bstart + 1, bend - bstart - 1);

            string msg = decode.front();
            decode.pop();
            actuallyRemove(msg, target);
        }
        else if (line.find("SWAP") != string::npos) {
            bstart = line.find("[");
            bend = line.find(",");
            string first = line.substr(bstart + 1, bend - bstart - 1);
            bstart = line.find(",", bend);
            bend = line.find("]", bstart);
            string second = line.substr(bstart + 1, bend - bstart - 1);

            string msg = decode.front();
            decode.pop();
            actuallySwap(msg, first, second);
        }
        else if (line.find("BST") != string::npos) {
            if (!decode.empty()) {
                string msg = decode.front();
                decode.pop();
                bst.insert(msg);
            }
        }
    }

    void readInput(string line, ofstream &file) {
        size_t start, end, bstart, bend;
        int priority;

        if (line.find("DECODE") != string::npos) {
            bstart = line.find("[");
            bend = line.find("]");
            string decoded = line.substr(bstart + 1, bend - bstart - 1);
            decode.push(decoded);
        }
        else if (line.find("ADD") != string::npos) {
            start = line.find("(");
            end = line.find(")");
            priority = stoi(line.substr(start + 1, end - start - 1));
            addToPQ(line, priority);
        }

        else if (line.find("REPLACE") != string::npos) {
            start = line.find("(");
            end = line.find(")");
            priority = stoi(line.substr(start + 1, end - start - 1));
            addToPQ(line, priority);
        }

        else if (line.find("SWAP") != string::npos) {
            start = line.find("(");
            end = line.find(")");
            priority = stoi(line.substr(start + 1, end - start - 1));
            addToPQ(line, priority);
        }

        else if (line == "Preorder" || line == "Inorder" || line == "Postorder") {
            order = line;
        }

        else if (line.find("BST") != string::npos) {
            start = line.find("(");
            end = line.find(")");
            if (start != string::npos && end != string::npos) {
                priority = stoi(line.substr(start + 1, end - start - 1));
                addToPQ(line, priority);
            }
        }
    }

    void outputFile(ofstream &file) {
        if (order == "Preorder") {
            bst.preorderTraversal(bst.getRoot(), file);
        } else if (order == "Inorder") {
            bst.inorderTraversal(bst.getRoot(), file);
        } else if (order == "Postorder") {
            bst.postorderTraversal(bst.getRoot(), file);
        }
    }
};

int main(int argc, char *argv[]) {
    ArgumentManager am(argc, argv);
    ifstream input(am.get("input"));
    ofstream output(am.get("output"));

    priorityQueue pq;
    string line;

    while (getline(input, line)) {
        pq.readInput(line, output);
    }

    pq.goThrough();
    pq.outputFile(output);

    return 0;
}
