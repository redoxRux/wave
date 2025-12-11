#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <thread>
#include <unistd.h>
#include <sys/ioctl.h>

using namespace std;

// ANSI color codes for GitHub heatmap colors
const string COLOR_LEVEL1 = "\033[38;2;13;57;34m";
const string COLOR_LEVEL2 = "\033[38;2;26;111;77m";
const string COLOR_LEVEL3 = "\033[38;2;38;166;65m";
const string COLOR_LEVEL4 = "\033[38;2;57;211;83m";
const string COLOR_RESET = "\033[0m";

const string SQUARE = "█";

struct TerminalSize {
    int width;
    int height;
};

TerminalSize getTerminalSize() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return {w.ws_col, w.ws_row};
}

string getCacheFile() {
    const char* homeDir = getenv("HOME");
    if (!homeDir) homeDir = ".";
    return string(homeDir) + "/.wave-cache";
}

bool isFirstLaunchOfDay() {
    string cacheFile = getCacheFile();
    auto now = chrono::system_clock::now();
    auto time = chrono::system_clock::to_time_t(now);
    tm* timeinfo = localtime(&time);
    
    string today = to_string(1900 + timeinfo->tm_year) + "-" + 
                   to_string(1 + timeinfo->tm_mon) + "-" + 
                   to_string(timeinfo->tm_mday);
    
    ifstream file(cacheFile);
    string lastDate;
    
    if (file.is_open()) {
        getline(file, lastDate);
        file.close();
        
        if (lastDate == today) {
            return false;
        }
    }
    
    ofstream outFile(cacheFile);
    outFile << today;
    outFile.close();
    
    return true;
}

string getColorForLevel(int level) {
    switch(level) {
        case 1: return COLOR_LEVEL1;
        case 2: return COLOR_LEVEL2;
        case 3: return COLOR_LEVEL3;
        case 4: return COLOR_LEVEL4;
        default: return COLOR_RESET;
    }
}

void displayAnimation() {
    TerminalSize size = getTerminalSize();
    int cols = size.width;
    int rows = size.height * 2;
    
    if (cols < 10) cols = 10;
    if (rows < 5) rows = 5;
    
    srand(time(0));
    
    // Print wave scrolling naturally (no clearing)
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int centerCol = cols / 2;
            int distFromCenter = abs(col - centerCol);
            int maxDist = centerCol;
            int baseIntensity = max(1, 4 - (distFromCenter * 3) / maxDist);
            int randomLevel = (rand() % 100 < 60) ? baseIntensity : max(0, baseIntensity - 1);
            randomLevel = max(0, min(4, randomLevel));
            
            cout << getColorForLevel(randomLevel) << SQUARE << COLOR_RESET;
        }
        cout << "\n";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int main() {
    if (isFirstLaunchOfDay()) {
        displayAnimation();
    }
    
    return 0;
}
