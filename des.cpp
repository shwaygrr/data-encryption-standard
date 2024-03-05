#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <iomanip>
#include <sstream>
#include <math.h>

#include "des_tables.h"

std::vector<std::bitset<48>> genSubkeys48(const std::bitset<56>& key_perm56);
std::bitset<64> ciperGen64 (std::bitset<64> key64, std::bitset<64> plain_t64);
std::bitset<32> f32(std::bitset<32> right32, std::bitset<48> subkey48);

//helper functions
std::vector<unsigned short int> revTable(const std::vector<unsigned short int>& table);
void leftRotate(std::bitset<28>& bin28, unsigned int num_rots);
unsigned int stringToHexPadded(const std::string& message);

int main() {

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


    
    std::bitset<64> plain_t64(0b0000000100100011010001010110011110001001101010111100110111101111);
    // //key
    std::bitset<64> key64(0b0001001100110100010101110111100110011011101111001101111111110001);
    
    // std::bitset<64> plain_t64(0X1122334455667788);
    // std::bitset<64> key64(0x752878397493CB70);

    ciperGen64(key64, plain_t64);
    return 0;
}
/********************************************************************HELPER FUNCTIONS**********************************************************************/
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

/*
    Reverse Vector (helper function)
        -   Input vector
        -   output: Input vector reversed
        -   Not part of DES, solely to deal with nature of bitset DS   
*/
std::vector<unsigned short int> revTable(const std::vector<unsigned short int>& table) {
    std::vector<unsigned short int> reverse_table;

    for (int i = table.size()-1; i >= 0; --i) {
        reverse_table.push_back(table[i]);
    }

    return reverse_table;
}

/* 
    Bit rotation (helper function)
        -   Input: Binary(28-bits) and numbr of rotations (nums_shifts)
        -   Output: Binary(28-bits) rotated left num_shift times 
*/
void leftRotate(std::bitset<28>& bin28, unsigned int num_rots) {
    /*
        -   In n<<d, last num_rots bits are 0. 
        -   To put first 3 bits of num_rots at last, do bitwise or of n<<d with n >>(INT_BITS - d)
        -   https://www.geeksforgeeks.org/rotate-bits-of-an-integer/
    */
    bin28 = ( bin28 << num_rots)|(bin28 >> (28 - num_rots));
}
/*********************************************************************************************************************************************************/



/*
    Permute
        Input: Permutation table of size return_T and binary of size input_T
        Output: Permutated binary of size return_T
*/
template <size_t input_T, size_t return_T>
std::bitset<return_T> permute(const std::vector<unsigned short int>& table, const std::bitset<input_T>& input) {
    //reverse table
    std::vector<unsigned short int> rev_table = revTable(table);
    
    std::bitset<return_T> permutation;
    
    // std::cout << "Transformation: " << std::endl;
    for(int bit = 0; bit < table.size(); bit++) {
        unsigned short int table_bit = input.size()-rev_table[bit]; //PC_1_bit from left since bitset goes from right to left (bit 7-right = bit 57-left)
        permutation[bit] = input[table_bit];
    }

    // std::cout << key64 << "---->" << new_key56 << std::endl;
    return permutation;
}


/*
    Generate 16 48-bit subkeys that have gone through trasformation
        -   Input: 56-bit Key permutation
        -   Output: 16 48-bit subkeys
*/
std::vector<std::bitset<48>> genSubkeys48(const std::bitset<56>& key_perm56) {
    
    //split key perm
    std::string str_key_perm56 = key_perm56.to_string();
    
    std::bitset<28> left28(str_key_perm56.substr(0, 28)); // first half
    std::bitset<28> right28(str_key_perm56.substr(28)); //second half

    std::vector<std::bitset<56>> subkeys56;
    
    //rotate and create subkeys
    for (int i = 1; i <= 16; i++) {
        if (i == 1 || i == 2 || i == 9 || i == 16) {
            //rotate left 1
            leftRotate(left28, 1);
            leftRotate(right28, 1);
        } else {
            //rotate left 2
            leftRotate(left28, 2);
            leftRotate(right28, 2);
        }

        //concatonate left and right
        std::bitset<56> subkey56(left28.to_string() + right28.to_string());
        subkeys56.push_back(subkey56);
    }

    std::vector<std::bitset<48>> subkeys_perm48;

    //Permutation using PC_2 for each 56-bit subkey
    for (std::bitset<56>& subkey56 : subkeys56) {
        std::bitset<48> new_subkey48 = permute<56, 48>(PC_2, subkey56);
        subkeys_perm48.push_back(new_subkey48);
    }

    return subkeys_perm48;
}


/*
    f function
        -   Input: 32-bit right side of previous plain text permutation and 48-bit subkey for correspondig round of encryption
        -   Output: 32-bits to be XOR with left side of previous plain text permutation
*/
std::bitset<32> f32(std::bitset<32> right32, std::bitset<48> subkey48) {
    //Get permutation of right with E_bit table
    std::bitset<48> new_right48 = permute<32, 48>(E_bit, right32);

    //XOR key and right
    new_right48 = new_right48 ^= subkey48;

    //Transform to 32-bits using S_BOXES
    std::string str_new_right48 = new_right48.to_string();
    std::string str_new_right32 = "";

    for (int i = 1; i <= 8; i++) {
        //get 6-bit block
        std::string block6(str_new_right48.substr(i*6-6, i*6));

        //row from first and last bit 00-11
        std::string row = ""; row += block6[0]; row += block6[5];
        std::bitset<2> row2(row);

        //column from middle 4 bits 0000-1111
        std::bitset<4> column4(block6.substr(1, 5));
        // std::cout << column4 << std::endl;
        
        //get 4-bit number
        std::bitset<4> block4(S_BOXES[i-1][(unsigned short int)row2.to_ulong()][(unsigned short int)column4.to_ulong()]);

        //concatonate to new 32-bit right
        str_new_right32 += block4.to_string();
    }
    std::bitset<32> new_right32(str_new_right32);
    
    //Permutation using P_table
    new_right32 = permute<32, 32>(P_table, new_right32);

    return new_right32;
}

/*
    16 rounds of Encryption
        -   Input: 64-bit key and 64-bit plain text
        -   Output: 64-bit cipher text
*/
std::bitset<64> ciperGen64 (std::bitset<64> key64, std::bitset<64> plain_t64) {
    //generate 16 keys
    std::bitset<56> key_perm56 = permute<64, 56>(PC_1, key64);
    std::vector<std::bitset<48>> subkeys48 = genSubkeys48(key_perm56);

    //plain text permutation
    std::bitset<64> plain_t_perm64 = permute<64, 64>(IP_table, plain_t64);


    //split plain text
    std::string str_plain_t_perm64 = plain_t_perm64.to_string();
    
    std::bitset<32> left32(str_plain_t_perm64.substr(0, 32)); //L0
    std::bitset<32> right32(str_plain_t_perm64.substr(32)); //R0

    // 16 rounds of encryption on plaintext
    for (int round = 0; round < 16; round++) {
        std::bitset<32> temp_left32 = left32;
        left32 = right32; //L_(i+1)
        right32 = temp_left32 ^= f32(left32, subkeys48[round]); //R_(i+1)
    }

    std::bitset<64> post_rounds64(right32.to_string()+left32.to_string());
    // std::cout <<  post_rounds64;

    std::bitset<64> cipher_t64 = permute<64, 64>(IP_1_inv, post_rounds64);
    std::cout << cipher_t64;

    return cipher_t64;
}