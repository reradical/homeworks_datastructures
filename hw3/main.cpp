#include "ArgumentManager.h"
#include <iostream>
#include <queue>
#include <string>
#include <fstream>

using namespace std;

struct PQNode {
    string command;
    int priority;
    PQNode(string cmd, int p) : command(cmd), priority(p) {}
};

struct ComparePriority {
    bool operator()(PQNode* lhs, PQNode* rhs) {
        return lhs->priority > rhs->priority;
    }
};

class PriorityQueue {
private:
    priority_queue<PQNode*, vector<PQNode*>, ComparePriority> pq;

public:
    void addToPQ(string command, int priority) {
        PQNode* node = new PQNode(command, priority);
        pq.push(node);
    }

    PQNode* popCommand() {
        if (pq.empty()) return nullptr;
        PQNode* node = pq.top();
        pq.pop();
        return node;
    }

    bool empty() {
        return pq.empty();
    }
};

class BSTNode {
public:
    string value;
    BSTNode *left, *right;

    BSTNode(string v) : value(v), left(nullptr), right(nullptr) {}
};

class BST {
private:
    BSTNode* root;

    void insert(BSTNode* &node, const string &val) {
        if (!node) {
            node = new BSTNode(val);
        } else if (val.size() < node->value.size()) {
            insert(node->left, val);
        } else if (val.size() > node->value.size()) {
            insert(node->right, val);
        } else {
            node->value = val;
        }
    }

    void preorder(BSTNode* node, ofstream &output) {
        if (node) {
            output << node->value << endl;
            preorder(node->left, output);
            preorder(node->right, output);
        }
    }

    void inorder(BSTNode* node, ofstream &output) {
        if (node) {
            inorder(node->left, output);
            output << node->value << endl;
            inorder(node->right, output);
        }
    }

    void postorder(BSTNode* node, ofstream &output) {
        if (node) {
            postorder(node->left, output);
            postorder(node->right, output);
            output << node->value << endl;
        }
    }

public:
    BST() : root(nullptr) {}

    void insert(string val) {
        insert(root, val);
    }

    void print(ofstream &output, const string &order) {
        if (order == "Preorder") {
            preorder(root, output);
        } else if (order == "Inorder") {
            inorder(root, output);
        } else if (order == "Postorder") {
            postorder(root, output);
        }
    }
};

string replaceChar(string str, char oldChar, char newChar) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == oldChar) {
            str[i] = newChar;
        }
    }
    return str;
}

string addCharAfter(string str, char target, char toAdd) {
    string result = "";
    for (int i = 0; i < str.length(); i++) {
        result += str[i];
        if (str[i] == target) {
            result += toAdd;
        }
    }
    return result;
}

string removeChar(string str, char target) {
    string result = "";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] != target) {
            result += str[i];
        }
    }
    return result;
}

string swapChars(string str, char first, char second) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == first) {
            str[i] = second;
        } else if (str[i] == second) {
            str[i] = first;
        }
    }
    return str;
}

int main(int argc, char *argv[]) {
    ArgumentManager am(argc, argv);
    ifstream input(am.get("input"));
    ofstream output(am.get("output"));

    PriorityQueue pq;
    BST bst;
    queue<string> messages;
    string line;
    string order;

    while (getline(input, line)) {
        size_t start, end, bstart, bend;
        int priority;

        if (line.find("DECODE") != string::npos) {
            bstart = line.find("[");
            bend = line.find("]");
            string decoded = line.substr(bstart + 1, bend - bstart - 1);
            messages.push(decoded);
        } else if (line.find("REPLACE") != string::npos) {
            start = line.find("(");
            end = line.find(")");
            priority = stoi(line.substr(start + 1, end - start - 1));
            pq.addToPQ(line, priority);
        } else if (line.find("ADD") != string::npos) {
            start = line.find("(");
            end = line.find(")");
            priority = stoi(line.substr(start + 1, end - start - 1));
            pq.addToPQ(line, priority);
        } else if (line.find("REMOVE") != string::npos) {
            start = line.find("(");
            end = line.find(")");
            priority = stoi(line.substr(start + 1, end - start - 1));
            pq.addToPQ(line, priority);
        } else if (line.find("SWAP") != string::npos) {
            start = line.find("(");
            end = line.find(")");
            priority = stoi(line.substr(start + 1, end - start - 1));
            pq.addToPQ(line, priority);
        } else if (line.find("BST") != string::npos) {
            start = line.find("(");
            end = line.find(")");
            priority = stoi(line.substr(start + 1, end - start - 1));
            pq.addToPQ(line, priority);
        } else if (line == "Preorder" || line == "Inorder" || line == "Postorder") {
            order = line;
        }
    }

    while (!pq.empty()) {
        PQNode* command = pq.popCommand();
        string msg = messages.front();
        messages.pop();

        if (command->command.find("REPLACE") != string::npos) {
            char oldChar = command->command[command->command.find("[") + 1];
            char newChar = command->command[command->command.find(",") + 1];
            msg = replaceChar(msg, oldChar, newChar);
        } else if (command->command.find("ADD") != string::npos) {
            char target = command->command[command->command.find("[") + 1];
            char toAdd = command->command[command->command.find(",") + 1];
            msg = addCharAfter(msg, target, toAdd);
        } else if (command->command.find("REMOVE") != string::npos) {
            char target = command->command[command->command.find("[") + 1];
            msg = removeChar(msg, target);
        } else if (command->command.find("SWAP") != string::npos) {
            char first = command->command[command->command.find("[") + 1];
            char second = command->command[command->command.find(",") + 1];
            msg = swapChars(msg, first, second);
        } else if (command->command.find("BST") != string::npos) {
            bst.insert(msg);
        }

        messages.push(msg);
    }

    bst.print(output, order);

    return 0;
}
