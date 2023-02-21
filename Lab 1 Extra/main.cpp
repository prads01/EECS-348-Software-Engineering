#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

const int NUM_DEPARTMENTS = 5;
const int NUM_PROGRAMMERS = 5;

struct Programmer {
    int id;
    vector<int> preferences;
};

struct Department {
    int id;
    vector<int> preferences;
    Programmer* programmer;
};

void readData(vector<Programmer>& programmers, vector<Department>& departments, const string& filename) {
    ifstream input(filename);
    if (input.is_open()) {
        for (int i = 0; i < NUM_PROGRAMMERS; i++) {
            Programmer p;
            p.id = i;
            for (int j = 0; j < NUM_DEPARTMENTS; j++) {
                int pref;
                input >> pref;
                p.preferences.push_back(pref);
            }
            programmers.push_back(p);
        }
        for (int i = 0; i < NUM_DEPARTMENTS; i++) {
            Department d;
            d.id = i;
            for (int j = 0; j < NUM_PROGRAMMERS; j++) {
                int pref;
                input >> pref;
                d.preferences.push_back(pref);
            }
            departments.push_back(d);
        }
        input.close();
    } else {
        cerr << "Error opening input file!" << endl;
        exit(1);
    }
}

bool prefers(Programmer* p, Department* d1, Department* d2) {
    for (int i = 0; i < NUM_DEPARTMENTS; i++) {
        if (p->preferences[i] == d1->id) {
            return true;
        } else if (p->preferences[i] == d2->id) {
            return false;
        }
    }
    cerr << "Error: Programmer " << p->id << " has no preference between departments " << d1->id << " and " << d2->id << endl;
    exit(1);
}

void match(vector<Programmer>& programmers, vector<Department>& departments) {
    vector<bool> programmerFree(NUM_PROGRAMMERS, true);

    while (find(programmerFree.begin(), programmerFree.end(), true) != programmerFree.end()) {
        int freeProgrammerIndex = find(programmerFree.begin(), programmerFree.end(), true) - programmerFree.begin();
        Programmer* freeProgrammer = &programmers[freeProgrammerIndex];

        for (int i = 0; i < NUM_DEPARTMENTS; i++) {
            int preferredDepartmentId = freeProgrammer->preferences[i];
            Department* preferredDepartment = &departments[preferredDepartmentId];

            if (preferredDepartment->programmer == nullptr) {
                preferredDepartment->programmer = freeProgrammer;
                programmerFree[freeProgrammerIndex] = false;
                break;
            }
                else {
                Programmer* currentProgrammer = preferredDepartment->programmer;
                if (prefers(currentProgrammer, preferredDepartment, *(departments.data() + currentProgrammer->preferences[i]))) {

                    preferredDepartment->programmer = freeProgrammer;
                    programmerFree[freeProgrammerIndex] = false;
                    programmerFree[currentProgrammer->id] = true;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < NUM_DEPARTMENTS; i++) {
        cout << "Department " << departments[i].id << " is assigned programmer " << departments[i].programmer->id << endl;
    }

}

int main() {
    vector<Programmer> programmers;
    vector<Department> departments;
    readData(programmers, departments, "input.txt");
    match(programmers, departments);
    return 0;
}