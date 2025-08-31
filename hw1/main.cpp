#include <iostream>
#include <fstream>
#include <string>
#include "ArgumentManager.h"
using namespace std;

struct Cred{
    string name;
    int age;
    int deposit;
    int numdrinks;
    Cred *next;
};

class Cred_List {

private:
    Cred *head;

public:
    Cred_List() {
        head = nullptr;
    }

    void Add(int index, string n, int a, int d, int nd) {
        int size = getSize();
        Cred *cu = new Cred;
        cu->name = n;
        cu->age = a;
        cu->deposit = d;
        cu->numdrinks = nd;
        cu->next = nullptr;

        if (!isDuplicate(n, a, d, nd)) {
            if (index == 0 || head == nullptr) { // head
                cu->next = head;
                head = cu;
            } else {
                Cred *tmp = head;
                int i = 0;
                while (tmp->next && i < index - 1) {
                    tmp = tmp->next;
                    i++;
                }
                cu->next = tmp->next;
                tmp->next = cu;
            }
        }
    }

    void addToList(string n, int a, int d, int nd) {
        Cred *cu = new Cred;
        cu->name = n;
        cu->age = a;
        cu->deposit = d;
        cu->numdrinks = nd;
        cu->next = nullptr;

        if (!head) {
            head = cu;
        } else if (!isDuplicate(n, a, d, nd)) {
            Cred *tmp = head;
            while (tmp->next) {
                tmp = tmp->next;
            }
            tmp->next = cu;
        }
    }

    void Remove(string rem, string val) {
        Cred *tmp = head;
        Cred *prev = nullptr;

        int value = 0;

        if (rem == "age") {
            value = stoi(val);
        } 
        else if (rem == "deposit") {
            value = stoi(val);
        } 
        else if (rem == "number of drinks") {
            value = stoi(val);
        }

        while (tmp) {
            if ((rem == "name" && tmp->name == val) || 
                (rem == "age" && tmp->age == value) ||
                (rem == "deposit" && tmp->deposit == value) ||
                (rem == "number of drinks" && tmp->numdrinks == value)) {
                if (prev == nullptr) {
                    head = tmp->next;
                } 
                else {
                    prev->next = tmp->next;
                }
                Cred *del = tmp;
                tmp = tmp->next;
                delete del;
            } 
            else {
                prev = tmp;
                tmp = tmp->next;
            }
        }
    }



    void Sort(string comp) {
        if (!head || !head->next){
            return;
        }

        bool swapped;
        Cred *tmp;
        Cred *prev = nullptr;

        do {
            swapped = false;
            tmp = head;

            while (tmp->next) {
                bool condition = false;
                if (comp == "name") {
                    condition = tmp->name > tmp->next->name;
                } 
                else if (comp == "age") {
                    condition = tmp->age > tmp->next->age;
                } 
                else if (comp == "deposit") {
                    condition = tmp->deposit > tmp->next->deposit;
                } 
                else if (comp == "number of drinks") {
                    condition = tmp->numdrinks > tmp->next->numdrinks;
                }

                if (condition) {
                    swapped = true;
                    string tmp_name = tmp->name;
                    int tmp_age = tmp->age;
                    int tmp_deposit = tmp->deposit;
                    int tmp_numdrinks = tmp->numdrinks;

                    tmp->name = tmp->next->name;
                    tmp->age = tmp->next->age;
                    tmp->deposit = tmp->next->deposit;
                    tmp->numdrinks = tmp->next->numdrinks;

                    tmp->next->name = tmp_name;
                    tmp->next->age = tmp_age;
                    tmp->next->deposit = tmp_deposit;
                    tmp->next->numdrinks = tmp_numdrinks;
                }
                tmp = tmp->next;
            }
        } 
        while (swapped);
    }

    bool isDuplicate(string n, int a, int d, int nd) {
        Cred *tmp = head;
        while (tmp) {
            if (tmp->name == n && tmp->age == a){
                tmp->deposit = d;
                tmp->numdrinks = nd;
                return true;
            }
            tmp = tmp->next;
        }
        return false;
    }

    int getSize() {
        int size = 0;
        Cred *tmp = head;
        while (tmp) {
            size++;
            tmp = tmp->next;
        }
        return size;
    }

    void readInput(string line) {
        size_t start = line.find('[') + 1;
        size_t end = line.find(']');

        size_t name_start = line.find("name: ");
        size_t name_end = line.find("; age");
        string n = line.substr(name_start + 6, name_end - name_start - 6);

        size_t age_start = line.find("age: ");
        size_t age_end = line.find("; deposit");
        int a = stoi(line.substr(age_start + 5, age_end - age_start - 5));

        size_t dep_start = line.find("deposit: ");
        size_t dep_end = line.find("; number of drinks");
        int d = stoi(line.substr(dep_start + 9, dep_end - dep_start - 9));
        
        size_t num_start = line.find("number of drinks: ");
        size_t num_end = end;
        int nd = stoi(line.substr(num_start + 18, num_end - num_start - 18));

        addToList(n, a, d, nd);
    }

    void readCommand(string line) {
        size_t start, end;
        string val;

        if (line.find("Add") != string::npos) {
            size_t add_start = line.find('(');
            size_t add_end = line.find(')');
            int pos = stoi(line.substr(add_start + 1, add_end - add_start - 1));

            start = line.find("name: ");
            end = line.find("; age");
            string n = line.substr(start + 6, end - start - 6);

            start = line.find("age: ");
            end = line.find("; deposit");
            int a = stoi(line.substr(start + 5, end - start - 5));

            start = line.find("deposit: ");
            end = line.find("; number of drinks");
            int d = stoi(line.substr(start + 9, end - start - 9));
            
            start = line.find("number of drinks: ");
            end = line.find(')');
            int nd = stoi(line.substr(start + 18, end - start - 18));

            Add(pos, n, a, d, nd);
        }

        else if (line.find("Remove") != string::npos) {
            size_t rem_start = line.find('(');
            size_t rem_end = line.find(':');
            string rem = line.substr(rem_start + 1, rem_end - rem_start - 1);

            size_t val_start = rem_end + 1;
            size_t val_end = line.find(')');
            string val = line.substr(val_start, val_end - val_start);

            Remove(rem, val); 
        }

        else if (line.find("Sort") != string::npos) {
            size_t s_start = line.find('(');
            size_t s_end = line.find(')');
            string s = line.substr(s_start + 1, s_end - s_start - 1);

            Sort(s);
        }
    }


    void Output(ofstream &file) {
        Cred *cu = head;
        while (cu != nullptr) {
            file << "[name: " << cu->name << "; age: " << cu->age << "; deposit: "
                 << cu->deposit << "; number of drinks: " << cu->numdrinks << "]\n";
            cu = cu->next;
        }
    }
};

int main(int argc, char *argv[]) {
    ArgumentManager am(argc, argv);
    ifstream input(am.get("input"));
    ifstream command(am.get("command"));
    ofstream output(am.get("output"));

    Cred_List list;

    string line;
    string cline;

    while (getline(input, line)) {
        list.readInput(line);
    }

    while (getline(command, cline)) {
        list.readCommand(cline);
    }

    list.Output(output);

    return 0;
}
