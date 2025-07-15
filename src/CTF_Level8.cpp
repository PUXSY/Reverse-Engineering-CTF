#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cstdint>  // Added missing header for uint64_t
#include <cstring>  // Added for fgets
#include <string>   // Added for std::string
#include <algorithm> // Added for std::min

#define NOMINMAX  // Prevent Windows from defining min/max macros
#include <windows.h> // Added for Windows API functions

#define MAX_INPUT_LEN 255
const char flag[38] = "681e91fd69229a93";

namespace obf {
    using u64 = uint64_t;

    // Obfuscated constants
    static const u64 xCEA9 = 0x9e3779b97f4a7c15ULL; 
    static const u64 xCF88 = 0xbf58476d1ce4e5b9ULL; 
    static const u64 xCEBE = 0x94d049bb133111ebULL; 

    // Mix function: split, xor, rotate, multiply
    inline u64 mix(u64 x) {
        x ^= x >> 30;
        x *= xCF88;
        x ^= x >> 27;
        x *= xCEBE;
        x ^= x >> 31;
        return x;
    }

    // Core obscure hasher
    std::string hash(const std::string& s) {
        u64 h = xCEA9;
        // process each group of up to 8 bytes
        for (size_t i = 0; i < s.size(); i += 8) {
            u64 chunk = 0;
            size_t end = (std::min)(s.size() - i, size_t(8));
            for (size_t j = 0; j < end; ++j) {
                // reverse bit order of each byte
                auto b = static_cast<u64>(s[i + j]);
                b = ((b & 0xF0) >> 4) | ((b & 0x0F) << 4);
                b = ((b & 0xCC) >> 2) | ((b & 0x33) << 2);
                b = ((b & 0xAA) >> 1) | ((b & 0x55) << 1);
                chunk |= (b << (j * 8));
            }
            h ^= mix(chunk ^ (h + xCEA9));
        }
        // final avalanche
        h = mix(h ^ (s.size() * xCEA9));

        // convert to hex
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        for (int i = 0; i < 8; ++i) {
            u64 byte = (h >> (i * 8)) & 0xFF;
            oss << std::setw(2) << byte;
        }
        return oss.str();
    }
}

bool anti_debug_check() {
    if (IsDebuggerPresent()) {
        MessageBoxA(NULL, "Debugger Detected!", "Bye", 0);
        ExitProcess(0);
    }
    return CloseHandle(reinterpret_cast<HANDLE>(0xDEADC0DE));
}

int secret() {
    printf("Enter the secret string: ");
    char userInput[MAX_INPUT_LEN];
    fgets(userInput, MAX_INPUT_LEN, stdin);  // Fixed to use fgets properly
    
    // Remove newline from fgets if present
    userInput[strcspn(userInput, "\n")] = '\0';
    
    if (obf::hash(std::string(userInput)) == flag)
        printf("Correct!\n");
    else
        printf("Incorrect!\n");

    return 0;
}

int main() {
    char buffer[10];
    anti_debug_check();
    
    printf("Enter a string: ");
    fgets(buffer, sizeof(buffer), stdin);  // Fixed buffer size
    printf("You entered: %s", buffer);  // Removed extra \n since fgets includes it
    return 0;
}