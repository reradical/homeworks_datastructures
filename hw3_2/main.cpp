#include "ArgumentManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <vector>
#include <algorithm>


using namespace std;

class idk {

private:
    int size;
    vector<int> arr;    
    string type;


public:

idk(int s = 0) : size(s) {
    if (size > 0) {
        arr.resize(size, -1);
    }
}

vector<int>returnARR(){
    return arr;
}

void setSize(int s){
    size = s;
    arr.resize(size, -1);
}

bool isDuplicate(int key){
    for (int i = 0; i < size; i++){
        if (key == arr[i]){
            return true;
        }
    }
    return false;
}



void quadraticProbing(int key){
    int index = 0;
    int c1 = 3;
    int c2 = 5;
    for(int i = 0; i < size; i++){
        index = (key % size + c1 * i + c2 * i * i) % size;
        if(arr[index] == -1){
            arr[index] = key;
            break;
        }
    }
}

void linearProbing(int key){
    int index = 0;
    int c1 = 1;
    for(int i = 0; i < size; i++){
        index = (key % size + c1 * i) % size;
        if(arr[index] == -1){
            arr[index] = key;
            break;
        }
    }
}



void readType(string t){
    type = t;
}

void readInput(string input) {
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == ',') {
            input[i] = ' ';
        }
    }

    stringstream ss(input);
    int num;

    while (ss >> num){
        if (!isDuplicate(num)){
            if (type == "Linear probing"){
                linearProbing(num);
            }
            else if (type == "Quadratic probing"){
                quadraticProbing(num);
            }
        }
    }
}

void printTable(ofstream &file){
    bool first = true;

    for (int i = 0; i < size; i++) {
        if (arr[i] != -1) {
            if (!first) {
                file << ',';
            }
            file << arr[i];
            first = false;
        }
    }
    file << endl;
}


};


class heap {
private:
    vector<int> arr;

public:
    void setArray(const vector<int>& a){
        arr.clear();
        for (int i = 0; i < a.size(); i++) {
            if (a[i] != -1) {
                arr.push_back(a[i]);
            }
        }
        make_heap(arr.begin(), arr.end(), greater<int>());
    }

    void printHeap(ofstream &file){
        bool first = true;
        for (int i = 0; i < arr.size(); i++) {
            if (!first) {
                file << ',';
            }
            file << arr[i];
            first = false;
        }
        file << endl;
    }
};



int main(int argc, char *argv[]) {
    ArgumentManager am(argc, argv);
    ifstream input(am.get("input"));
    ofstream output(am.get("output"));

    idk table;
    heap Heap;

    int size;
    string ssize, type, next;

    getline(input, ssize);
    stringstream ss(ssize);
    ss >> size;
    table.setSize(size);

    getline(input, type);
    table.readType(type);

    getline(input, next);
    table.readInput(next);

    Heap.setArray(table.returnARR());

    table.printTable(output);
    Heap.printHeap(output);

    return 0;
}