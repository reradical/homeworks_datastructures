#include "ArgumentManager.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>

using namespace std;

struct node {
    string val;
    node* left;
    node* right;
    node(string v) : val(v), left(nullptr), right(nullptr) {}
};

class BST {
private:
    node* root;
    ofstream* file;
    bool isMirrored;

public:
    BST() : root(nullptr), file(nullptr), isMirrored(false) {}
    
    node* getRoot() {
        return root;
    }

    void insert(node*& root, string v) {
        if (!root) {
            root = new node(v);
        } 
        else if ((!isMirrored && v < root->val) || (isMirrored && v > root->val)) {
            insert(root->left, v);
        } 
        else {
            insert(root->right, v);
        }
    }


    void inorderTraversal(node* root) {
        if (!root) return;
        inorderTraversal(root->left);
        *file << root->val << " ";
        inorderTraversal(root->right);
    }

/*    void preorderTraversal(node* root) {
        if (!root) return;
        *file << root->val << " ";
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }

    void postorderTraversal(node* root) {
        if (!root) return;
        postorderTraversal(root->left);
        postorderTraversal(root->right);
        *file << root->val << " ";
    }

    */

    void PrintLR() {
        inorderTraversal(root);
        *file << endl;
    }

    void printLevel(int n) {
        if (!root) {
            *file << "Does Not Exist!" << endl;
            return;
        }

        queue<node*> q;
        q.push(root);
        int level = 0;

        while (!q.empty()) {
            int size = q.size();

            if (level == n) {
                while (size > 0) {
                    node* cu = q.front();
                    q.pop();
                    *file << cu->val << " ";
                    size--;
                }
                *file << endl;
                return;
            }

            while (size > 0) {
                node* cu = q.front();
                q.pop();
                if (cu->left) q.push(cu->left);
                if (cu->right) q.push(cu->right);
                size--;
            }

            level++;
        }

        *file << "Does Not Exist!" << endl;
    }

    void mirrorTree(node*& root) {
        if (!root) return;
        node* temp = root->left;
        root->left = root->right;
        root->right = temp;
        mirrorTree(root->left);
        mirrorTree(root->right);
    }

    void Mirror() {
        mirrorTree(root);
        isMirrored = !isMirrored;
    }


    void assignOutput(ofstream* output) {
        file = output;
    }

    void readInput(string line) {
        if (line.find("Insert") != string::npos) {
            size_t s_start = line.find('(');
            size_t s_end = line.find(')');
            string s = line.substr(s_start + 1, s_end - s_start - 1);
            insert(root, s);
        } 
        else if (line.find("PrintLR") != string::npos) {
            PrintLR();
        } 
        else if (line.find("PrintLevel") != string::npos) {
            size_t s_start = line.find('(');
            size_t s_end = line.find(')');
            string s = line.substr(s_start + 1, s_end - s_start - 1);
            int level = stoi(s);
            printLevel(level);
        } 
        else if (line.find("Mirror") != string::npos) {
            Mirror();
        }
    }
};

int main(int argc, char* argv[]) {
    ArgumentManager am(argc, argv);
    ifstream input(am.get("input"));
    ofstream output(am.get("output"));

    BST bst;
    bst.assignOutput(&output);

    string line;
    while (getline(input, line)) {
        if (line.empty()) continue;
        bst.readInput(line);
    }

    return 0;
}
