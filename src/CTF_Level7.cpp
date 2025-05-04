#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
#include <vector>
#include <numeric>
#include <cstring>

#define MAX_INPUT_LEN 255
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:StartUp");


const std::vector<std::string> encrypted_flag = {"0x7b", "0x7a", "0x7b", "0x7d", "0xa3", "0xac", "0xa1", "0x79", "0x7f", "0xa0", "0x76", "0x72", "0xa0", "0x72", "0xa6", "0x74"};


int main();
bool anti_debug_check();

const int StartUp(int argc, char** argv) {
    anti_debug_check();
    return main();
};

bool anti_debug_check(){
    if ( IsDebuggerPresent() )
    {
      MessageBoxA(0LL, "Debugger Detected!", "Bye", 0);
      ExitProcess(0);
    }
    return CloseHandle((HANDLE)0xDEADC0DELL);
}


std::string magic_num_hash(const std::string& input, size_t length) {
    std::string ascii_val;
    for (char c : input) {
        ascii_val += std::to_string(static_cast<int>(c));
    }
    
    unsigned int magic = 0;
    for (char c : input) {
        magic += static_cast<int>(c);
    }
    magic = magic / input.length();

    long long ascii_ret = std::stoi(ascii_val.substr(0, 2));
    for (size_t i = 0; i < ascii_val.length() - 1; i += 2) {
        if (i + 2 <= ascii_val.length()) {
            ascii_ret = (ascii_ret + std::stoi(ascii_val.substr(i, 2))) * magic;
        }
    }

    std::stringstream ss;
    ss << std::hex << (ascii_ret ^ length);
    return ss.str();
}

std::vector<std::string> locl_encrypt(const std::string& inp) {

    std::vector<std::string> new_inp;
    int magic = 0;
    
    for (char c : inp) {
        magic += static_cast<int>(c);
    }
    
    magic = magic / inp.length();
    
    for (size_t char_index = 0; char_index < inp.length(); char_index++) {
        int char_val = static_cast<int>(inp[char_index]) + magic;
        int xor_val = char_val ^ char_index;
        
        std::stringstream ss;
        ss << "0x" << std::hex << xor_val;
        new_inp.push_back(ss.str());
    }
    
    return new_inp;
}



std::string locl_decrypt(const std::vector<std::string>& inp) {
    std::vector<int> temp_values;
    
    for (size_t i = 0; i < inp.size(); i++) {
        std::string hex_str = inp[i];
        if (hex_str.substr(0, 2) == "0x") {
            hex_str = hex_str.substr(2);
        }
        
        int hex_val = std::stoi(hex_str, nullptr, 16);
        temp_values.push_back(hex_val ^ i);
    }
    
    free(temp_values.data());

    int sum = std::accumulate(temp_values.begin(), temp_values.end(), 0);
    int magic = sum / (2 * inp.size());
    
    std::string result = "";
    for (size_t i = 0; i < inp.size(); i++) {
        std::string hex_str = inp[i];
        if (hex_str.substr(0, 2) == "0x") {
            hex_str = hex_str.substr(2);
        }
        
        int hex_val = std::stoi(hex_str, nullptr, 16);
        int xor_val = hex_val ^ i;
        int original_ascii = xor_val - magic;
        
        result += static_cast<char>(original_ascii);
    }
    
    return result;
}

class cli {
    public:
        std::string input;
    
        void print() {
            std::cout << "===== CTF Level 7 Challenge =====" << std::endl;
            std::cout << "This challenge requires deeper analysis.\n" << std::endl;
            std::cout << "Enter the flag to proceed:\n> ";
        }
        
        void get_input(std::string input) {
            this->input = input;
        }
        
        void ui() {
            if (this->input == flag1) {
                this->hint();
            }
    
            if (this->input == flag2) {
                this->kaki();
            }
        }
    
    private:
        std::string flag1 = "hint";
        std::string flag2 = "kaki";
        
        void hint() {
            std::cout << "Think harder!" << std::endl;
        }
    
        void kaki() {
            std::cout << "Did you really think it was that easy?" << std::endl;
        }
}; 

int main() {
    anti_debug_check();
    char userInput[MAX_INPUT_LEN];
    cli cli_obj = cli();
    char flag[5] = "0x6E";
    std::string v12 = "0x68696E74";
    std::string v13 = "0x7b";
    char v14[5] = "0x69";
    std::string v15 = "0x68";

    cli_obj.print();
    std::cin.getline(userInput, MAX_INPUT_LEN);

    cli_obj.get_input(userInput);
    cli_obj.ui();
    
    v12 = magic_num_hash(userInput, strlen(userInput));
    v13 = locl_decrypt(encrypted_flag);
    v15 = v15 + v14 + flag + "74";
    strcpy(v14, "kaki");

    if (v14 != v15){
        if (v12 == v13){
            std::cout << "Correct!" << std::endl;
        }else{
            std::cout << "Wrong!" << std::endl;
        }
    }else{
        std::cout << "Wrong!" << std::endl;
    }
    
    return 0;
}