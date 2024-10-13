#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
using namespace std;

struct Log {
    string month, day, time, ip, issue;
    Log* next;

    Log(string m, string d, string t, string i, string iss)
            : month(m), day(d), time(t), ip(i), issue(iss), next(nullptr) {}
};

class LinkedList {
public:
    LinkedList() : head(nullptr) {}

    void append(const string& month, const string& day, const string& time, const string& ip, const string& issue) {
        Log* newLog = new Log(month, day, time, ip, issue);
        if (!head) {
            head = newLog;
        } else {
            Log* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newLog;
        }
    }

    void printList() const {
        Log* temp = head;
        while (temp) {
            cout << temp->month << " " << temp->day << " " << temp->time << " " << temp->ip << " " << temp->issue << endl;
            temp = temp->next;
        }
    }

    void sort() {
        head = mergeSort(head);
    }

    void searchByIPRange(const string& startIP, const string& endIP) const {
        Log* temp = head;
        bool found = false;
        while (temp) {
            if (temp->ip >= startIP && temp->ip <= endIP) {
                cout << temp->month << " " << temp->day << " " << temp->time << " " << temp->ip << " " << temp->issue << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found) {
            cout << "No records found in the IP range " << startIP << " to " << endIP << "." << endl;
        }
    }

    ~LinkedList() {
        Log* temp;
        while (head) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }

private:
    Log* head;

    Log* mergeSort(Log* head) {
        if (!head || !head->next) {
            return head;
        }

        Log* middle = getMiddle(head);
        Log* nextOfMiddle = middle->next;
        middle->next = nullptr;

        Log* left = mergeSort(head);
        Log* right = mergeSort(nextOfMiddle);

        return merge(left, right);
    }

    Log* getMiddle(Log* head) {
        if (!head) return head;

        Log* slow = head;
        Log* fast = head->next;

        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        return slow;
    }

    Log* merge(Log* left, Log* right) {
        if (!left) return right;
        if (!right) return left;

        Log* result = nullptr;

        if (left->ip <= right->ip) {
            result = left;
            result->next = merge(left->next, right);
        } else {
            result = right;
            result->next = merge(left, right->next);
        }

        return result;
    }
};

int main() {
    ifstream file("bitacora-1.txt");
    string lineContent, word, year, month, day, time, ip, issue;
    LinkedList list;

    if (file) {
        while (getline(file, lineContent)) {
            stringstream lineStream(lineContent);
            lineStream >> month >> day >> year >> time >> ip;
            issue.clear();
            while (lineStream >> word) {
                issue += (issue.empty() ? "" : " ") + word;
            }
            list.append(month, day, time, ip, issue);
        }
        file.close();
    } else {
        cerr << "No se pudo abrir el archivo bitacora-1.txt" << endl;
        return 1;
    }

    list.sort();

    string startIP, endIP;
    cout << "Enter the start IP: ";
    cin >> startIP;
    cout << "Enter the end IP: ";
    cin >> endIP;

    cout << "Records in the IP range " << startIP << " to " << endIP << ":" << endl;
    list.searchByIPRange(startIP, endIP);

    return 0;
}
