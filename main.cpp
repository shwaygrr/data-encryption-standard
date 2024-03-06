#include <sstream>
#include <iomanip>
#include <cstdint>
#include "des.h"

std::string stringToHexPadded(const std::string& message);
uint64_t hexToDec(const std::string& hex64);
std::string binToHex(const std::bitset<64>& cipher_t64);
std::string ECB(const std::string& key, const std::string& plain_t);

int main() {
    // ask for key
    std::string key; //0E329232EA6D0D73
    
    do {
        std::cout << "Enter the key **16-hexadigit hex**: "; 
        std::cin >> key;
    } while (key.length() != 16);
    
    std::cin.ignore(); //so getline does not read whiitespace after cin
    
    //ask for message
    std::string message;  //Your lips are smoother than vaseline
    std::cout << "Enter your message: ";
    std::getline(std::cin, message);
    
    // mode of operation
    std::string cipher_text_hex = ECB(key, message);
    std::cout << "Cipher Text (in Hex): " << cipher_text_hex << std::endl;
    return 0;
}


/*
    String to Hex
        -   Input: String
        -   Output: Integer Representation of String Hex
        https://www.techieclues.com/blogs/convert-string-to-hexadecimal-in-cpp
*/
std::string stringToHexPadded(const std::string& message) {
    std::string hex_result = "";
    for (char c : message) {
        int ascii = static_cast<int>(c);
        std::stringstream ss;
        ss << std::hex << std::uppercase << ascii;
        hex_result += ss.str();
    }
    // Add "0D0A" for Carriage Return and Line Feed
    hex_result += "0D0A";

    //pad
    while(hex_result.length() % 16 != 0) {
        hex_result +="0";
    }

    return hex_result;
}

/*
    Hexadecimal to decimal
        - Input: 16-hexadigit Hexadecimal string
        - Output: Decimal (64-bits)
        https://stackoverflow.com/questions/11031159/c-converting-hexadecimal-to-decimal
*/
uint64_t hexToDec(const std::string& hex64) {
    //create an input string stream with the hexadecimal string
    std::istringstream iss(hex64);

    uint64_t decimal;

    //read the hexadecimal string as a hexadecimal integer
    iss >> std::hex >> decimal;

    return decimal;
}

/*
    Binary to Hexadecimal
        - Input: 64-bit binary
        - Output: 16-hexadigit bit
*/
std::string binToHex(const std::bitset<64>& cipher_t64) {
    //convert the std::bitset to an integer
    unsigned long long int_value = cipher_t64.to_ullong();

    //create a string stream to hold the hexadecimal representation
    std::stringstream res;

    //write the integer value as hexadecimal to the stringstream
    res << std::hex << std::uppercase << int_value;
    
    //pad front
    std::string hex_string16 = res.str();
    while (hex_string16.length() != 16) {
        hex_string16 = "0" + hex_string16; 
    }
    
    return hex_string16;
}


/*
    Electronic Code Block Mode of Operation
        - Input: Plain text
        - Output: Cipher text
*/
std::string ECB(const std::string& key, const std::string& plain_t) {
    //turn key to binary
    std::bitset<64> key64(hexToDec(key));
    
    // turn to hex and pad
    std::string message_hex = stringToHexPadded(plain_t);
    
    // start encryption 64 bits at a time
    std::string cipher_text = "";

    for(int i = 0; i < message_hex.length()/16; i++) {
        //get 16-hexadecimal block
        std::string plaint_t_hex16 = message_hex.substr(i*16, 16);

        //turn into 64-bit block
        std::bitset<64> plain_t_bin64(hexToDec(plaint_t_hex16));

        //encrypt
        std::string cipher_t_block64 = binToHex(cipherGen64(key64, plain_t_bin64));
        cipher_text += (cipher_t_block64 + " ");
    }

    return cipher_text;
}
