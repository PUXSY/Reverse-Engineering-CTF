/* CTF Level 13

flag: CTF{ACtu^LLy,_h0W-d!d-YOu-d0_IT?!}
*/
#include <string>
#include <iostream>
#include <cstddef>
#include <windows.h>
#include <stdlib.h>

/*
1;┳┓    ┏━┓┏        
2;┣┫┓┏  ┃┗┛╋┓┏┓┏┓┏  
3;┻┛┗┫  ┗━┛┛┗┻┛┗┗┫  
4;   ┛           ┛  
*/
#define mesegboxA MessageBoxW(NULL, L"\x43\x6f\x72\x72\x65\x63\x74\x20\x46\x6c\x61\x67\x21", L"\x43\x54\x46", MB_OK);
const size_t KEY = 0xC;
std::string secret = "";

// https://www.binarymath.net/decimal-to-binary.php
std::string decimalToBinary(int decimalNumber){
    std::string binaryNumber = "";
    while (decimalNumber > 0)
    {
        binaryNumber += (decimalNumber % 2) + '0';
        decimalNumber /= 2;
    }
    return binaryNumber;
}

std::string textToBinary(char *text) {
    std::string binaryText = "";
    int index = 0;

    while (text[index] != '\0')
    {
        char character = text[index] - '0'; // get the ascii value of the character
        binaryText += decimalToBinary(character);
            
        index += 1;
    }
    
    return binaryText;
}

bool Debuger() {
    if (IsDebuggerPresent()) {
        printf("Debugger detected. Exiting...");
        ExitProcess(0);
    }
    if (CheckRemoteDebuggerPresent(GetCurrentProcess(), nullptr)){  
        abort();
    }
    
    if (IsDebuggerPresent() || CheckRemoteDebuggerPresent(GetCurrentProcess(), nullptr)){
        std::exit(EXIT_FAILURE);
        abort();
    }
    return CloseHandle((HANDLE)0xDEADC0DELL);
}

std::string PART(const char String[], const int Ofszet){
    std::string binaryNumber = "";
    const int key = KEY + Ofszet;

    for (int i = 0; String[i] != '\0'; i++){
        int c = static_cast<unsigned char>(String[i]) ^ key;
        binaryNumber += decimalToBinary(c);
        Sleep(rand() % 10); // optional noise
    }
    return binaryNumber;
}

// 110010010010110111010011000111000111001100111001110111 : 0x60 <= 0xC + 0x54 | aadddbabaabdahaahaadadaadaabaa

std::string PARTA(const int Ofszet){
    const char String[31] = "\x61\x61\x64\x64\x64\x62\x61\x62\x61\x61\x62\x64\x61\x68\x61\x61\x68\x61\x61\x64\x61\x64\x61\x61\x64\x61\x61\x62\x61\x61";
    Debuger();
    return PART(String, Ofszet);
}

// 11111101000011010110011111001111101111010111 : 0x65 <= 0xC + 0x59 | ddddddgudggdaddddaddddgdddggdd
std::string PARTB(int Ofszet){
    const char String[31] = "\x64\x64\x64\x64\x64\x64\x67\x75\x64\x67\x67\x64\x61\x64\x64\x64\x64\x61\x64\x64\x64\x64\x67\x64\x64\x64\x67\x67\x64\x64";
    Debuger();
    return PART(String, Ofszet);
}

// 11101010011001101111100011100011100110011100111011001 : 0x67 <= 0xC + 0x5B | fffeecfcfeffffoffoffcfcffcffefc
std::string PARTC(int Ofszet){
    const char String[32] = "\x66\x66\x66\x65\x65\x63\x66\x63\x66\x65\x66\x66\x66\x66\x6f\x66\x66\x6f\x66\x66\x63\x66\x63\x66\x66\x63\x66\x66\x65\x66\x63";
    Debuger();
    return PART(String, Ofszet);
}
// 0x746869732069732064756d6d7920696e666f => 0x746869732, 0x069732064756d6 , 0xd7920696e666f (some dummy info.)
std::string reternCorectFlag(){
    secret += "┣┫┓┏  ┃┗┛╋┓┏┓┏┓┏\n"; // 2;
    std::string binaryNumber = "";
    unsigned long long tpx = 0x746869732;
    binaryNumber += PARTA(0x34 ^ 0x60);
    if (IsDebuggerPresent()) {
        abort();
    }
    unsigned long long tpy = 0x069732064756d6;
    binaryNumber += PARTB(0x3C ^ 0x65);
    if (IsDebuggerPresent()) {
        abort();
    }
    unsigned long long tpz = 0xd7920696e666f;
    binaryNumber += PARTC(0x3c ^ 0x67);
    if (IsDebuggerPresent()) {
        abort();
    }
    unsigned long t = (tpx + tpy + tpz);
    if (t < 0){
        printf("how?\n%ld", t);
        exit(1);
    }
    secret += "┻┛┗┫  ┗━┛┛┗┻┛┗┗┫\n"; // 3;
    return binaryNumber;
}


int main() {
    Debuger();
    char userInput[64];
    secret += "┳┓    ┏━┓┏\n"; // 1;
    printf("=---==+: CTF Challenge :+==---=\n");    
    printf("Find and enter the correct flag:\n> ");
    
    fgets(userInput, sizeof(userInput), stdin);

    Debuger();
    if (textToBinary(userInput) == reternCorectFlag()) {
        mesegboxA
    } else {
        printf("\x57\x72\x6f\x6e\x67\x21\n");
    }
    
    SetConsoleOutputCP(CP_UTF8);
    secret += "   ┛           ┛\n\0"; // 4;
    std::cout << secret;
    return 0;
}
