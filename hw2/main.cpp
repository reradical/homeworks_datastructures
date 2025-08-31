#include "ArgumentManager.h"
#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

struct Song {
    string title;
    int time;
    bool fav;
    Song *prev;
    Song *next;
};


class Playlist {
private:
    Song *head;

public:

    void AddSong(string title, int duration, bool fav_status) {
        Song *tmp = new Song;
        tmp->title = title;
        tmp->time = duration;
        tmp->fav = fav_status;

        if (head == nullptr){
            head = tmp;
        } 
        else{
            Song *cu = head;
            while (cu->next){
                cu = cu->next;
            }
            cu->next = tmp;
            tmp->prev = cu;
        }
    }

    void Print(ofstream &file){
        Song *cu = head;
        file << "[";
        while (cu != nullptr){
            file << cu->title;
            if (cu->next != nullptr){
                file << ", ";
            }
            cu = cu->next;
        }
        file << "]\n";
    }

    void PrintReversedPlaylist(ofstream &file){
        Song *cu = head;
        while (cu->next){
            cu = cu->next;
        }
        file << "[";
        if (cu){
            file << cu->title;
            cu = cu->prev;
        }
        while (cu){
            file << ", " << cu->title;
            cu = cu->prev;
        }
        file << "]\n";
    }


    void DeleteSong(string title){
        Song *cu = head;
        Song *prev = nullptr;

        while (cu){
            if (cu->title == title){
                if (prev == nullptr) {
                    head = cu->next;
                } 
                else{
                    prev->next = cu->next;
                    if (cu->next) {
                        cu->next->prev = prev;
                    }
                }
                delete cu;
            } 
            prev = cu;
            cu = cu->next;
        }
    }

    void createSmartPlaylist(int limit, ofstream &file){
        int total = 0;
        Song *cu = head;
        queue<string> smart;

        while (cu) {
            if (cu->fav && (total + cu->time <= limit)){
                smart.push(cu->title);
                total += cu->time;
            }
            cu = cu->next;
        }

        cu = head;

        while (cu){
            if (!cu->fav && (total + cu->time <= limit)){
                smart.push(cu->title);
                total += cu->time;
            }
            cu = cu->next;
        }
        printQueue(file, smart);
    }

    void printQueue(ofstream &file, queue<string> pl){
        file << "[";
        while (!pl.empty()) {
            file << pl.front();
            pl.pop();
            if (!pl.empty()) {
                file << ", ";
            }
        }
        file << "]\n";
    }



    void readInput(string line, ofstream &file){
        size_t start, end;
        string t;
        int d;
        string f;
        bool fv;
        int l;

        if (line.find("AddSong") != string::npos){
            start = line.find("(");
            end = line.find(", ");
            t = line.substr(start + 1, end - start - 1);

            start = end + 2;
            end = line.find(", ", start);
            d = stoi(line.substr(start, end - start));

            start = end + 2;
            end = line.find(")", start);
            f = (line.substr(start, end - start));
            if (f.find("True") != string::npos){
                fv = true;
            }
            else {
                fv = false;
            }

            AddSong(t, d, fv);
            
        }
        if (line.find("DeleteSong") != string::npos){
            start = line.find("(");
            end = line.find(")");
            t = line.substr(start + 1, end - start - 1);
            DeleteSong(t);
        }
        if (line.find("PrintPlaylist") != string::npos){
            Print(file);
        }

        if (line.find("PrintReversedPlaylist") != string::npos){
            PrintReversedPlaylist(file);
        }
        if (line.find("CreateSmartPlaylist") != string::npos){
            start = line.find("(");
            end = line.find(")");
            l = stoi(line.substr(start + 1, end - start - 1));
            createSmartPlaylist(l, file);
        }
    }


};

int main(int argc, char *argv[]) {
    ArgumentManager am(argc, argv);
    ifstream input(am.get("input"));
    ofstream output(am.get("output"));

    Playlist list;

    string line;

    while (getline(input, line)) {
        list.readInput(line, output);
    }




    return 0;
}
