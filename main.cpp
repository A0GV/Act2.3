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

// Merge function to merge two halves
void merge(vector<string>& logs, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<string> leftLogs(n1);
    vector<string> rightLogs(n2);

    for (int i = 0; i < n1; i++)
        leftLogs[i] = logs[left + i];
    for (int i = 0; i < n2; i++)
        rightLogs[i] = logs[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (ipToLong(extractIP(leftLogs[i])) <= ipToLong(extractIP(rightLogs[j]))) {
            logs[k] = leftLogs[i];
            i++;
        } else {
            logs[k] = rightLogs[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        logs[k] = leftLogs[i];
        i++;
        k++;
    }

    while (j < n2) {
        logs[k] = rightLogs[j];
        j++;
        k++;
    }
}

// Merge sort function
void mergeSort(vector<string>& logs, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(logs, left, mid);
        mergeSort(logs, mid + 1, right);

        merge(logs, left, mid, right);
    }
}

int main() {
    ifstream inputFile("bitacora.txt");
    string line;
    vector<string> logs;
    
    string beginningInput;
    string lastInput;

    cout << "Rango Inicial de IPs: ";
    cin >> beginningInput;
    cout << "Rango Final de IPs: ";
    cin >> lastInput;

    while (getline(inputFile, line)) {
        unsigned long ipNum = ipToLong(extractIP(line));
        if (ipNum >= ipToLong(beginningInput) && ipNum <= ipToLong(lastInput)) {
            logs.push_back(line);
        }
    }

    inputFile.close();

    mergeSort(logs, 0, logs.size() - 1);

    ofstream outputFile("bitacoraPorIP.txt");
    for (const string &log : logs) {
        string ip = extractIP(log);
        if (ip >= beginningInput && ip <= lastInput) {
            outputFile << log << endl;
        }
    }

    outputFile.close();

    return 0;
}
