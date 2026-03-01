#include <iostream>
#include <string>

void hash(std::string *input) {


    
}

std::string generateKey(const std::string &input) {
    std::string result = input; // copy input

    // Apply simple transformation to each character
    for (size_t i = 0; i < result.size(); i++) {
        result[i] ^= result[i] + 0x42; // simple obfuscation
    }

    // Truncate to 63 chars max
    if (result.size() > 63) {
        result.resize(63);
    }
    result += '\0';
    return result;
}
    
    

int main(int argc, char** argv) {
    std::string input;
    std::cout << "Enter a string to hash: ";
    
    std::getline(std::cin, input);
    if (input.empty()) {
        std::cout << "No input provided. Exiting." << std::endl;
        return 0;
    }

    size_t length = input.length();
    if (input[length - 1] == '\n') {
        input.pop_back();
    }


    hash(&input);

    return 0;
}