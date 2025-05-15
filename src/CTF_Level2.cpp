#include <iostream>
#include <string>
#include <cstring>
#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#else
// For non-Windows platforms, let's define a simple MessageBox alternative
void MessageBoxA(void* hWnd, const char* text, const char* caption, int type) {
    std::cout << "------------------------------" << std::endl;
    std::cout << caption << ": " << text << std::endl;
    std::cout << "------------------------------" << std::endl;
}
#endif

void transform_and_cleanup_buffer(void* dest, int64_t src);
int64_t copy_buffer_with_sso(int64_t dest, int64_t src);


int main(int argc, const char** argv) {
    std::string input;
    std::string transformed_input;
    std::string expected_key = "CTF_2_{779841834}";
    
    std::cout << "Enter license key: ";
    std::cin >> input;

    transformed_input = input;

    bool is_valid = (transformed_input == expected_key);

    if (is_valid) {
        MessageBoxA(nullptr, "License Accepted!", "Success", 0);
    } else {
        MessageBoxA(nullptr, "Invalid License", "Error", 0);
    }
    
    return 0;
}

void transform_and_cleanup_buffer(void* dest, int64_t src) {
    std::string* dest_str = static_cast<std::string*>(dest);
    std::string* src_str = reinterpret_cast<std::string*>(src);
    *dest_str = *src_str;
}

int64_t copy_buffer_with_sso(int64_t dest, int64_t src) {
    std::string* dest_str = reinterpret_cast<std::string*>(dest);
    std::string* src_str = reinterpret_cast<std::string*>(src);
    *dest_str = *src_str;
    return reinterpret_cast<int64_t>(dest_str);
}