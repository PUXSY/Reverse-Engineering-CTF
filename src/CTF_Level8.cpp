#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <iostream> 

#define MAX_INPUT_LEN 255
const char flag[38] = "681e91fd69229a93";

namespace obf {
    using u64 = uint64_t;

    // Obfuscated constants
    static const u64 Ω = 0x9e3779b97f4a7c15ULL;
    static const u64 ψ = 0xbf58476d1ce4e5b9ULL;
    static const u64 ξ = 0x94d049bb133111ebULL;

    // Mix function: split, xor, rotate, multiply
    inline u64 mix(u64 x) {
        x ^= x >> 30;
        x *= ψ;
        x ^= x >> 27;
        x *= ξ;
        x ^= x >> 31;
        return x;
    }

    // Core obscure hasher
    std::string hash(const std::string& s) {
        u64 h = Ω;
        // process each group of up to 8 bytes
        for (size_t i = 0; i < s.size(); i += 8) {
            u64 chunk = 0;
            size_t end = std::min(s.size() - i, size_t(8));
            for (size_t j = 0; j < end; ++j) {
                // reverse bit order of each byte
                auto b = static_cast<u64>(s[i + j]);
                b = ((b & 0xF0) >> 4) | ((b & 0x0F) << 4);
                b = ((b & 0xCC) >> 2) | ((b & 0x33) << 2);
                b = ((b & 0xAA) >> 1) | ((b & 0x55) << 1);
                chunk |= (b << (j * 8));
            }
            h ^= mix(chunk ^ (h + Ω));
        }
        // final avalanche
        h = mix(h ^ (s.size() * Ω));

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


int secret() {
    printf("Enter the secret string: ");
    char userInput[MAX_INPUT_LEN];
    if (obf::hash(userInput) == flag)
        printf("Correct!\n");

    return 0;
}

int main() {
    char buffer[10];

    printf("Enter a string: ");
    fgets(buffer, 100, stdin);  
    printf("You entered: %s\n", buffer);
    return 0;
}