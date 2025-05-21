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

int main(int argc, const char** argv) {
    char input[20];
    char transformed_input[20];
    char expected_key[20];
    
    std::cout << "Enter license key: ";
    std::cin >> input;

    strcpy_s(transformed_input, sizeof(transformed_input), input);
    strcpy_s(expected_key, sizeof(expected_key), "CTF_2_{779841834}");

    bool is_valid = strcmp(transformed_input, expected_key) == 0;

    if (is_valid) {
        MessageBoxA(nullptr, "License Accepted!", "Success", 0);
    } else {
        MessageBoxA(nullptr, "Invalid License", "Error", 0);
    }
    
    return 0;
}

