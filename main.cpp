#include <sstream>
#include <iomanip>

#include "des.h"

unsigned int stringToHexPadded(const std::string& message);

int main() {
{
    //ask for input, plain_t and 
    // std::string message;

    // std::cout << "Please Enter your message: ";
    // std::getline (std::cin, message);
    
    // turn to hex, pad, turn to binary
    // unsigned int bin = stringToHexPadded(message);
    // const unsigned int num_bits = static_cast<int>(ceil(log(bin)))+1;
    // std::bitset<num_bits> plaint_t64(bin);
    
    // std::cout << message;

    // // start encryption 64 bits at a time
    // std::cout << plain64;

}
    
    std::bitset<64> plain_t64(0b0000000100100011010001010110011110001001101010111100110111101111);
    std::bitset<64> key64(0b0001001100110100010101110111100110011011101111001101111111110001);
    
    // std::bitset<64> plain_t64(0X1122334455667788);
    // std::bitset<64> key64(0x752878397493CB70);

    cipherGen64(key64, plain_t64);
    return 0;
}


/*
    String to Hex
        -   Input: String
        -   Output: Integer Representation of String Hex
        https://www.techieclues.com/blogs/convert-string-to-hexadecimal-in-cpp
*/
unsigned int stringToHexPadded(const std::string& message) {
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

    std::cout << hex_result << std::endl;

    std::stringstream ss;
    ss << std::hex << hex_result;
    unsigned int dec_result;
    ss >> dec_result;

    return dec_result;
}