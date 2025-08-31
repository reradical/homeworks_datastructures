#include "ArgumentManager.h"
#include <iostream>
#include <fstream>
#include <queue>


using namespace std;

struct node{
    int value;
    node *left;
    node *right;
    node(){
        value = -999;
        left = nullptr;
        right = nullptr;
    }
    node(int v){
        value = v;
        left = nullptr;
        right = nullptr;
    }
};

class AVL {
private:
    node *root;

public:

    void insertVal(int value) {
        root = Insert(root, value);
    }

    void removeVal(int value) {
        root = Remove(root, value);
    }

    int getHeight(node *root){
        if (root == nullptr)
            return -1;
            int lh = getHeight(root->left);
            int rh = getHeight(root->right);
            return 1 + max(lh, rh);
    }

    int getBalanceFactor(node *root){
        if (root == nullptr)
            return 0;
            return getHeight(root->left) - getHeight(root->right);
    }

    node *singleLeftRotation(node *root){
        node *newRoot = root->right;
        node *newRight = newRoot->left;
        newRoot->left = root;
        root->right = newRight;
        return newRoot;
    }

    node *singleRightRotation(node *root){
        node *newRoot = root->left;
        node *newLeft = newRoot->right;
        newRoot->right = root;
        root->left = newLeft;
        return newRoot;
    }

    
    void printLevelOrder(ofstream &output){
        if (root == nullptr){
            output << "AVL Tree is empty." << endl;
            return;
        }
        queue<node *> q;
        q.push(root);
        while (!q.empty()){
        int levelSize = q.size();
        for (int i = 0; i < levelSize; i++){
            node *cu = q.front();
            q.pop();
            output << cu->value << " ";
            if (cu->left != nullptr)
                q.push(cu->left);
            if (cu->right != nullptr)
                q.push(cu->right);
        }
        output << endl;
        }
    }

    bool isDuplicate(node *root, int value) {
        if (root == nullptr)
            return false;
        if (root->value == value)
            return true;
        else if (value < root->value)
            return isDuplicate(root->left, value);
        else
            return isDuplicate(root->right, value);
    }

    node *Insert(node *root, int value){
        if (!isDuplicate(root, value)){
            if (root == nullptr){
                node *newNode = new node(value);
                return newNode;
            }
            else if (value < root->value){
                root->left = Insert(root->left, value);
            }
            else if (value > root->value){
                root->right = Insert(root->right, value);
            }
            else{
                return root;
            }
        }
        int bf = getBalanceFactor(root);
        if (bf > 1 && value < root->left->value){
            return singleRightRotation(root);
        }
        else if (bf < -1 && value > root->right->value){
            return singleLeftRotation(root);
        }
        else if (bf > 1 && value > root->left->value){
            root->left = singleLeftRotation(root->left);
            return singleRightRotation(root);
        }
        else if (bf < -1 && value < root->right->value){
            root->right = singleRightRotation(root->right);
            return singleLeftRotation(root);
        }
        return root;
    }

    node* findMin(node* root){
        while (root->left != nullptr) {
            root = root->left;
        }
        return root;
    }
    
    node* Remove(node* root, int value){
        if (root == nullptr) {
            return nullptr;
        }
        if (value < root->value) {
            root->left = Remove(root->left, value);
        } 
        else if (value > root->value) {
            root->right = Remove(root->right, value);
        } 
        else {
            if (root->left == nullptr && root->right == nullptr) {
                delete root;
                return nullptr;
            } 
            else if (root->left == nullptr) {
                node* temp = root->right;
                delete root;
                return temp;
            } 
            else if (root->right == nullptr) {
                node* temp = root->left;
                delete root;
                return temp;
            } 
            else{
                node* temp = findMin(root->right);
                root->value = temp->value;
                root->right = Remove(root->right, temp->value);
            }
        }
        int bf = getBalanceFactor(root);
        if (bf > 1){
            if (getBalanceFactor(root->left) >= 0) {
                return singleRightRotation(root);
            } 
            else{
                root->left = singleLeftRotation(root->left);
                return singleRightRotation(root);
            }
        }
        if (bf < -1){
            if (getBalanceFactor(root->right) <= 0) {
                return singleLeftRotation(root);
            } 
            else{
                root->right = singleRightRotation(root->right);
                return singleLeftRotation(root);
            }
        }
    
        return root;
    }


    void readInput(string line, ofstream &file){
        size_t start, end;
        string t;
        int n;

        if (line.find("Insert") != string::npos){
            start = line.find("(");
            end = line.find(")");
            n = stoi(line.substr(start + 1, end - start - 1));

            insertVal(n);
            
        }
        if (line.find("Remove") != string::npos){
            start = line.find("(");
            end = line.find(")");
            n = stoi(line.substr(start + 1, end - start - 1));
            removeVal(n);
        }
    }


};


int main(int argc, char *argv[]) {
    ArgumentManager am(argc, argv);
    ifstream input(am.get("input"));
    ofstream output(am.get("output"));

    AVL tree;
    string line;

    while (getline(input, line)) {
        tree.readInput(line, output);
    }

    tree.printLevelOrder(output);


    return 0;
}
