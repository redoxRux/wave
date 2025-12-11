#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <thread>
#include <unistd.h>
#include <sys/ioctl.h>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

using namespace std;

// ANSI color codes for GitHub heatmap colors
const string COLOR_DARK = "\033[38;2;14;17;23m";      // #0e1117
const string COLOR_LEVEL1 = "\033[38;2;13;57;34m";    // #0d3922
const string COLOR_LEVEL2 = "\033[38;2;26;111;77m";   // #1a6f4d
const string COLOR_LEVEL3 = "\033[38;2;38;166;65m";   // #26a641
const string COLOR_LEVEL4 = "\033[38;2;57;211;83m";   // #39d353
const string COLOR_RESET = "\033[0m";
const string BG_DARK = "\033[48;2;13;17;23m";
const string BG_RESET = "\033[49m";

const string SQUARE = "█";

struct TerminalSize {
    int width;
    int height;
};

TerminalSize getTerminalSize() {
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        return {csbi.srWindow.Right - csbi.srWindow.Left + 1, 
                csbi.srWindow.Bottom - csbi.srWindow.Top + 1};
    #else
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return {w.ws_col, w.ws_row};
    #endif
}

string getLastLaunchDate() {
    string home = getenv("HOME") ? getenv("HOME") : ".";
    return home + "/.terminal_last_launch";
}

bool isFirstLaunchOfDay() {
    string dateFile = getLastLaunchDate();
    auto now = chrono::system_clock::now();
    auto time = chrono::system_clock::to_time_t(now);
    tm* timeinfo = localtime(&time);
    
    string today = to_string(1900 + timeinfo->tm_year) + "-" + 
                   to_string(1 + timeinfo->tm_mon) + "-" + 
                   to_string(timeinfo->tm_mday);
    
    ifstream file(dateFile);
    string lastDate;
    
    if (file.is_open()) {
        getline(file, lastDate);
        file.close();
        
        if (lastDate == today) {
            return false;
        }
    }
    
    // Update the file with today's date
    ofstream outFile(dateFile);
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
        default: return COLOR_DARK;
    }
}

void displayAnimation() {
    system(CLEAR_SCREEN);
    
    TerminalSize size = getTerminalSize();
    int cols = size.width / 2;  // Account for full-width characters
    int rows = size.height - 2;
    
    // Ensure reasonable minimum
    if (cols < 10) cols = 10;
    if (rows < 5) rows = 5;
    
    // Create grid
    vector<vector<int>> grid(rows, vector<int>(cols, 0));
    
    // Animation: wave from top to bottom
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int centerCol = cols / 2;
            int distFromCenter = abs(col - centerCol);
            int maxDist = centerCol;
            int intensityLevel = max(1, 4 - (distFromCenter * 3) / maxDist);
            
            grid[row][col] = intensityLevel;
            
            // Print the square
            cout << getColorForLevel(intensityLevel) << SQUARE << COLOR_RESET;
            cout.flush();
            
            this_thread::sleep_for(chrono::milliseconds(1));
        }
        cout << "\n";
    }
    
    // Hold state
    this_thread::sleep_for(chrono::milliseconds(200));
    
    // Fade out and clear
    system(CLEAR_SCREEN);
}

int main() {
    if (isFirstLaunchOfDay()) {
        displayAnimation();
    }
    
    return 0;
}
