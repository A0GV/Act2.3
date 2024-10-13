#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Function to split a string by a delimiter
vector<string> split(const string &str, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to extract the IP address from a log entry
string extractIP(const string &logEntry) {
    vector<string> parts = split(logEntry, ' ');
    return parts[3];
}

// Function to convert an IP address to a numerical value for comparison
unsigned long ipToLong(const string &ip) {
    vector<string> parts = split(ip, '.');
    unsigned long ipNum = 0;
    for (const string &part : parts) {
        ipNum = ipNum * 256 + stoi(part);
    }
    return ipNum;
}

// Structure to represent a log entry in the linked list
struct Log {
    string entry;
    Log* next;

    Log(const string& e) : entry(e), next(nullptr) {}
};

// Class to represent a linked list of log entries
class LinkedList {
public:
    LinkedList() : head(nullptr) {}

    void append(const string& entry) {
        Log* newLog = new Log(entry);
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
            cout << temp->entry << endl;
            temp = temp->next;
        }
    }

    void sort() {
        head = mergeSort(head);
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

public:
    Log* getHead() const {
        return head;
    }

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

        if (ipToLong(extractIP(left->entry)) <= ipToLong(extractIP(right->entry))) {
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
    ifstream inputFile("bitacora.txt");
    string line;
    LinkedList logs;
    
    string beginningInput;
    string lastInput;

    cout << "Rango Inicial de IPs: ";
    cin >> beginningInput;
    cout << "Rango Final de IPs: ";
    cin >> lastInput;

    while (getline(inputFile, line)) {
        unsigned long ipNum = ipToLong(extractIP(line));
        if (ipNum >= ipToLong(beginningInput) && ipNum <= ipToLong(lastInput)) {
            logs.append(line);
        }
    }

    logs.sort();

    ofstream outputFile("bitacoraPorIP.txt");
    Log* temp = logs.getHead();
    while (temp) {
        string ip = extractIP(temp->entry);
        if (ip >= beginningInput && ip <= lastInput) {
            outputFile << temp->entry << endl;
        }
        temp = temp->next;
    }

    outputFile.close();

    return 0;
}
