#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct Log {
    string month, day, time, ip, issue;
    Log* next;

    Log(string m, string d, string t, string i, string iss)
            : month(m), day(d), time(t), ip(i), issue(iss), next(nullptr) {}
};

int main() {
    ifstream file("bitacora-1.txt");
    string lineContent, word, year, month, day, time, ip, issue;
    Log* head = nullptr;
    Log* tail = nullptr;

    if (file) {
        while (getline(file, lineContent)) {
            stringstream lineStream(lineContent);
            lineStream >> month >> day >> year >> time >> ip;
            issue.clear();
            while (lineStream >> word) {
                issue += (issue.empty() ? "" : " ") + word;
            }
            Log* newLog = new Log(month, day, time, ip, issue);
            if (!head) {
                head = newLog;
                tail = newLog;
            } else {
                tail->next = newLog;
                tail = newLog;
            }
            }
        file.close();
    }

    // Liberar memoria
    Log* current = head;
    while (current) {
        Log* next = current->next;
        delete current;
        current = next;
    }

    return 0;
}