#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <vector>

using namespace std;

void countStream(istream& stream, const string& label, bool countLines, bool countWords, bool countBytes, bool countChars) {
    size_t lineCount = 0, wordCount = 0, charCount = 0, byteCount = 0;
    string line;

    while (getline(stream, line)) {
        ++lineCount;
        wordCount += count_if(line.begin(), line.end(), [](char c) { return isspace(c); }) + 1;
        charCount += line.length();
        byteCount += line.length() + 1; // +1 for newline character
    }

    if (countLines) cout << lineCount << " ";
    if (countWords) cout << wordCount << " ";
    if (countBytes) cout << byteCount << " ";
    if (countChars) cout << charCount << " ";

    if (!label.empty()) cout << label;  // Print filename only if it's a file
    cout << "\n";
}

void countFile(const string& filename, bool countLines, bool countWords, bool countBytes, bool countChars) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file " << filename << "\n";
        return;
    }
    countStream(file, filename, countLines, countWords, countBytes, countChars);
}

int main(int argc, char* argv[]) {
    bool countLines = false, countWords = false, countBytes = false, countChars = false;
    vector<string> files;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-l") countLines = true;
        else if (arg == "-w") countWords = true;
        else if (arg == "-c") countBytes = true;
        else if (arg == "-m") countChars = true;
        else files.push_back(arg); // Only add filenames to the list
    }

    // If no flags are given, count everything by default (like GNU wc)
    if (!(countLines || countWords || countBytes || countChars)) {
        countLines = countWords = countBytes = true;
    }

    if (files.empty()) {
        // No files provided, read from stdin
        countStream(cin, "", countLines, countWords, countBytes, countChars);
    } else {
        // Process each file
        for (const auto& file : files) {
            countFile(file, countLines, countWords, countBytes, countChars);
        }
    }

    return 0;
}
