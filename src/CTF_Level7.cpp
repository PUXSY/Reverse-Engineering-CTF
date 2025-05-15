// CTF Level 6 
#include <string>
#include <bitset>
#include <iostream>
#include <debugapi.h>

std::string base64_key;
std::string binary_key;

int start_it(int argc, std::string* base64_key, std::string* binary_key);

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

const char key[] = {
0x54, 0x68, 0x65, 0x20, 0x72, 0x61, 0x62, 0x62, 0x69, 0x74,
0x20, 0x68, 0x6f, 0x6c, 0x65, 0x20, 0x6e, 0x65, 0x76, 0x65,
0x72, 0x20, 0x65, 0x6e, 0x64, 0x73, 0x2e, 0x00};

volatile const char very_important_string_that_never_used_yeah_I_know_that_is_not_best_variable_name[] = { 0x1b, 0x31, 0x31, 0x31, 0x5e, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
  0x5e, 0x1b, 0x31, 0x31, 0x31, 0x31, 0x4d, 0x31, 0x31, 0x31, 0x31, 0x31, 
  0x3e, 0x1b, 0x31, 0x31, 0x31, 0x31, 0x31, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 
  0x1b, 0x31, 0x31, 0x3c, 0x31, 0x31, 0x6d, 0x31, 0x31, 0x31, 0x6d, 0x31,
  0x3c, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x5f, 0x7e, 0x65, 0x79, 
  0x78, 0x7f, 0x76, 0x3d, 0x31, 0x73, 0x64, 0x65, 0x31, 0x65, 0x79, 0x74, 
  0x31, 0x73, 0x74, 0x62, 0x65, 0x1b, 0x31, 0x31, 0x31, 0x31, 0x31, 0x3c, 
  0x3c, 0x3c, 0x3c, 0x3c, 0x1b, 0x31, 0x31, 0x31, 0x31, 0x3e, 0x31, 0x31, 
  0x31, 0x31, 0x31, 0x4d, 0x1b, 0x31, 0x31, 0x31, 0x31, 0x3c, 0x3c, 0x3c, 
  0x3c, 0x3c, 0x3c, 0x3c, 0x1b, 0x31, 0x31, 0x31, 0x31, 0x6d, 0x31, 0x31, 
  0x31, 0x31, 0x31, 0x6d, 0x1b, 0x0d };

const char corrupted[] = { 0x5d, 0x7c, 0x6d, 0x7c, 0x7a, 0x6d, 0x7c, 0x7d, 0x39, 0x7a, 0x76, 0x6b,
                           0x6b, 0x6c, 0x69, 0x6d, 0x7c, 0x7d, 0x39, 0x74, 0x7c, 0x74, 0x76, 0x6b, 
                           0x60, 0x38
};
const char ok[] = { 0x2c, 0x6c, 0x06, 0x0c, 0x04, 0x05, 0x5a, 0x3d, 0x17, 0x17, 0x17, 0x68, 0x68, 
  0x68, 0x68, 0x17, 0x17, 0x17, 0x68, 0x68, 0x17, 0x68, 0x68, 0x3d, 0x17, 0x17, 
  0x18, 0x17, 0x68, 0x68, 0x17, 0x6b, 0x17, 0x18, 0x17, 0x18, 0x18, 0x68, 0x18, 
  0x3d, 0x17, 0x18, 0x17, 0x18, 0x17, 0x18, 0x17, 0x18, 0x18, 0x17, 0x1b, 0x0b, 
  0x17, 0x17, 0x17, 0x3d, 0x18, 0x17, 0x18, 0x68, 0x18, 0x17, 0x18, 0x18, 0x17, 
  0x18, 0x4b, 0x17, 0x4b, 0x17, 0x17, 0x3d, 0x6b, 0x68, 0x68, 0x68, 0x68, 0x18, 
  0x18, 0x68, 0x18, 0x17, 0x4b, 0x68, 0x4b, 0x17, 0x17, 0x3d, 0x2c, 0x6c, 0x07, 
  0x5a, 0x3d 
};
const char ok_l[] = { 0x17, 0x17, 0x17, 0x17, 0x17, 0x70, 0x58, 0x58, 0x53, 0x17, 0x5d, 0x58, 0x55, 
    0x16, 0x17, 0x3d 
  };
const char fail[] = { 0x31, 0x71, 0x1b, 0x11, 0x19, 0x1b, 0x47, 0x20, 0x0a, 0x0a, 0x0a, 0x0a, 0x75,
    0x75, 0x75, 0x75, 0x75, 0x75, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
    0x75, 0x0a, 0x0a, 0x75, 0x75, 0x0a, 0x20, 0x0a, 0x0a, 0x0a, 0x05, 0x0a, 0x75,
    0x75, 0x75, 0x75, 0x05, 0x75, 0x75, 0x75, 0x75, 0x0a, 0x75, 0x0a, 0x02, 0x75,
    0x03, 0x05, 0x0a, 0x05, 0x0a, 0x20, 0x0a, 0x0a, 0x05, 0x0a, 0x05, 0x75, 0x0a,
    0x0a, 0x0a, 0x05, 0x0a, 0x75, 0x75, 0x0a, 0x4a, 0x05, 0x05, 0x0a, 0x05, 0x05,
    0x0a, 0x05, 0x0a, 0x0a, 0x20, 0x0a, 0x05, 0x0a, 0x75, 0x75, 0x05, 0x0a, 0x0a,
    0x05, 0x0a, 0x05, 0x75, 0x05, 0x0a, 0x05, 0x05, 0x0a, 0x05, 0x05, 0x0a, 0x05,
    0x75, 0x75, 0x75, 0x20, 0x05, 0x75, 0x05, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x76,
    0x75, 0x75, 0x06, 0x75, 0x05, 0x05, 0x75, 0x05, 0x05, 0x75, 0x75, 0x75, 0x75,
    0x75, 0x05, 0x20, 0x31, 0x71, 0x1a, 0x47, 0x20
};
const int BIT_SIZE = 2048;

static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

std::string hash(std::string inp, std::string *oup, std::string key){
  for (int i = 0; i < inp.length(); i++){
    oup[i] = inp[i] ^ key[i % key.length()];
  }
  return *oup;
}

int start_it(int argc, std::string *base64_key, std::string *binary_key) {
  const char *key = key;
  int key_len = strlen(key);


  if (key_len > argc){
    *base64_key = base64_encode((unsigned char*)key, key_len);
    return 1;
  }else if (key_len == argc)
  {
    std::string TextToBinaryString(std::string words);
    *binary_key = TextToBinaryString(std::string(key));
    return 1;
  }else{
    return 0;
  }
}

std::string TextToBinaryString(std::string words) {
    std::string binaryString = "";
    for (char& _char : words) {
        binaryString += std::bitset<8>(_char).to_string();
    }
    return binaryString;
}

std::string A() {
    std::string a = "MDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAX";
    
    if (!a.empty()) {
        a.pop_back();
    }
    return a;
}

std::string AA(){
    std::string aa = "wMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDS";
    if (!aa.empty()) {
        aa.pop_back();
    }
    return aa;
}

std::string AAA(){
    std::string aaa = "AwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMw";
    if (!aaa.empty()) {
        aaa.pop_back();
    }
    return aaa;
}

std::string AAAA(){
    std::string aaaa = "DAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAxMDAwMDExMDEwMTAxMDAwMTAwMDExMDAxMDExMTExMDAxMTAxMTEwMTAxMTExMTAxMTExMDExMDAxMTAwMTEwMDExMTAwMDAwMTEwMTAxMDAxMTAwMTAwMDExMTAwMTAwMTEwMTExMDAxMTAxMDEwMDExMDAxMTAwMTEwMTAwMDAxMTEwMDE==";
    if (!aaaa.empty()) {
        aaaa.pop_back();
    }
    return aaaa;
}

std::string B(){
  std::string b = "MDEwMTAxMDAwMTAwMTAwMDAxMDAwMTAxMDAxMDAwMDAwMTAxMDAxMDAxMDAwMDAxMDEwMDAw";
  if (!b.empty()) {
    b.pop_back();
  }
  return b;
}
std::string BB(){
  std::string bb = "MTAwMTAwMDAxMDAxMDAxMDAxMDEwMTAxMDAwMDEwMDAwMDAxMDAxMDAwMDEwMDExMTEwMTAw";
  if (!bb.empty()) {
    bb.pop_back();
  }
  return bb;
}
std::string BBB(){
  std::string bbb = "MTEwMDAxMDAwMTAxMDAxMDAwMDAwMTAwMTExMDAxMDAwMTAxMDEwMTAxMTAwMTAwMDEwMTAx";
  if (!bbb.empty()) {
    bbb.pop_back();
  }
  return bbb;
}

std::string BBBB(){
  std::string bbbb = "MDEwMDEwMDAxMDAwMDAwMTAwMDEwMTAxMDAxMTEwMDEwMDAxMDAwMTAxMDAxMTAwMTAwMDAx";
  if (!bbbb.empty()) {
    bbbb.pop_back();
  }
  return bbbb;
}


std::string GetFlag() {
  std::string Aflag = "0x7e1891b6028b08157a28";
  std::string Bflag = "0x8f3a9c1d74b2e5f0";

  Aflag = A() + AA() + AAA() + AAAA();
  Bflag = B() + BB() + BBB() + BBBB();

  if (Aflag.length() > Bflag.length()){
    return Aflag;

  }else{
    return Bflag;
  }
}

bool isValidFlag(const std::string& userInput, std::string &hash) {
  std::string userInputFlag = TextToBinaryString(userInput);
  std::bitset<BIT_SIZE> bits(userInputFlag);
  std::bitset<BIT_SIZE> shifted = bits >> 0x40;
  std::string shiftedString = shifted.to_string();
  std::string base64userInputFlag = base64_encode(reinterpret_cast<const unsigned char*>(shiftedString.c_str()), shiftedString.length());
  if (base64userInputFlag == GetFlag()) {
      return true;
  }else if (hash == GetFlag())
  {
    return true;
  }
  
  return false;
}

int actualValidator(std::string input) {
    if (input.empty()) {
        return 0;
    }
    std::string binaryInput = TextToBinaryString(input);
    std::string base64Input = base64_encode(reinterpret_cast<const unsigned char*>(binaryInput.c_str()), binaryInput.length());

    for (int i = 0; i < binaryInput.length(); i++) {
        if (binaryInput[i] != '0' && binaryInput[i] != '1') {
            return 0;
        }
    }
    for (int i = 0; i < base64Input.length(); i++) {
        if (is_base64(base64Input[i])) {
            return 0;
        }
    }

    if (binaryInput.length() != BIT_SIZE || base64Input != GetFlag()) {
        return 0;
    }
    if (binaryInput != binaryInput) {
        return 0;
    }
    return 1;
}

int decoyValidator() {
    return 0x10;
}

int _encode(){
  return 0x80;
}

volatile char set_11812(){
  return very_important_string_that_never_used_yeah_I_know_that_is_not_best_variable_name[rand() % 100];
}

char set_991111141(){
  return corrupted[rand() % 100];
}

int main() {
  srand(time(NULL) % 100);
  std::string userInput;
  char v12 = set_11812();
  char v13 = set_991111141();
  char v24 = v12 ^ v13;
  std::string s_key;


  if (IsDebuggerPresent()) {
      std::cout << "Debugger detected. Exiting..." << std::endl;
      return 1;
  }

  std::cout << "===== CTF Level 6 Challenge =====" << std::endl;
  std::cout << "This challenge requires deeper analysis.\n" << std::endl;
  std::cout << "Enter the flag to proceed:\n> ";
  std::getline(std::cin, userInput);

  if (userInput.empty()) {
      std::cout << "Invalid input. Please try again." << std::endl;
      return 1;
  }
  

  clock_t start = clock();
  int selector = rand() % 4;
  if (selector == 0 || selector == 2 || selector == 3) {
      int validate = actualValidator(userInput);
  } else {
      int validate = decoyValidator();
  }
  
  clock_t end = clock();
    
  // If too much time has elapsed, it might be a debugger
  double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
  if (time_spent > 0.1) {
      printf("Processing error. Please try again.\n");
      return 1;
  }
  if (start_it == 0){
    _encode();
  }else{
    hash(base64_key, &s_key, key);
  }

  if (isValidFlag(userInput, s_key)) {
      std::cout << "Correct!" << std::endl;
  } else {
      std::cout << "Wrong!" << std::endl;
  }
  std::getline(std::cin, userInput);

  return 0;

}   

/*
The flag is CTF_7_{38529753492223423}

Step 1: Convert the flag to binary string.  
Step 1.5: Move the binary string 64 bits to the right.
Step 2: Convert the binary string to base64.
Step 3: Brack the base64 string into 4 parts 683 characters each.
Step 4: Add a A to the end of the base64 string
Step 5: Check if the base64 string is valid.
If it is valid, then the flag is correct.
*/

