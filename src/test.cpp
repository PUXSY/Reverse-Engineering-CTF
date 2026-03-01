#include <bitset>
#include <iostream>
#include <string>

// Base64 from your code
static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (int j = 0; j < 4; j++)
                ret += base64_chars[char_array_4[j]];
            i = 0;
        }
    }

    if (i) {
        for (int j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (int j = 0; j < i + 1; j++)
            ret += base64_chars[char_array_4[j]];

        while (i++ < 3)
            ret += '=';
    }

    return ret;
}

std::string TextToBinaryString(const std::string& words) {
    std::string binaryString;
    for (char c : words) {
        binaryString += std::bitset<8>(c).to_string();
    }
    return binaryString;
}

int main() {
    const std::string flag =
        "CTF{To_be_HOne$t,-I-doN't-understand_What-I-did-Here.}";

    std::string bin = TextToBinaryString(flag);

    std::bitset<2048> bits(bin);
    std::bitset<2048> shifted = bits >> 64;

    std::string shiftedString = shifted.to_string();

    std::string out = base64_encode(
        (const unsigned char*)shiftedString.c_str(),
        shiftedString.size()
    );

    std::cout << out << std::endl;
}