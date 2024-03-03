#include <iostream>
#include <vector>
#include <bitset>
#include <iomanip>

std::bitset<56> keyGen(std::bitset<64> key64);
std::bitset<64> initPerm(std::bitset<64>& plain_64);

std::vector<unsigned short int> revTable(std::vector<unsigned short int> table);

int main() {
    //plain text
    std::bitset<64> plain_t64(0b0000000100100011010001010110011110001001101010111100110111101111);
    
    // initPerm(plain_t64);


    //key
    std::bitset<64> key64(0b0001001100110100010101110111100110011011101111001101111111110001);
    keyGen(key64);
    return 0;
}

/*
    Step 1: Initial Permutation
        -   Input: 64-bit plain text
        -   Output: 64-bit Permutation 
*/
std::bitset<64> initPerm(std::bitset<64>& plain_t64) {
    
    std::vector<unsigned short int> IP_table = {
        58,	50,	42,	34,	26,	18,	10,	2,
        60,	52,	44,	36,	28,	20,	12,	4,
        62,	54,	46,	38,	30,	22,	14,	6,
        64,	56,	48,	40,	32,	24,	16,	8,
        57,	49,	41,	33,	25,	17,	9,	1,
        59,	51,	43,	35,	27,	19,	11,	3,
        61,	53,	45,	37,	29,	21,	13,	5,
        63,	55,	47,	39,	31,	23,	15,	7
    };

    IP_table = revTable(IP_table); //can loop through table in reverse to use less memory
 
    std::bitset<64> new_plain_t64;

    std::cout << "Transformation: " << std::endl;
    for(unsigned short int bit = 0; bit < IP_table.size(); bit++) {
        unsigned short int IP_bit = IP_table.size()-IP_table[bit]; //IP_bit from left since bitset goes from right to left (bit 7-right = bit 57-left)
        std::cout << 64-bit << "/" << plain_t64[bit] << "-->" << 64-IP_bit << "/" << plain_t64[IP_bit] << std::endl;
        new_plain_t64[bit] = plain_t64[IP_bit];
    }

    std::cout << plain_t64 << "---->" << new_plain_t64 << std::endl;
    return new_plain_t64;   
}


/*
    Key Generatation
    -   Input: 64-bit key
    -   Output: 56-bit key (temp)
*/
std::bitset<56> keyGen(std::bitset<64> key64) {
    std::vector<unsigned short int> PC_1 = {
        57, 49, 41, 33, 25, 17, 9,
        1,  58, 50, 42, 34, 26, 18,
        10, 2,  59, 51, 43, 35, 27,
        19, 11, 3,  60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7,  62, 54, 46, 38, 30, 22,
        14, 6,  61, 53, 45, 37, 29,
        21, 13, 5,  28, 20, 12, 4
};
    
    PC_1 = revTable(PC_1);
    
    std::bitset<56> new_key56;
    
    std::cout << "Transformation: " << std::endl;
    for(unsigned short int bit = 0; bit < PC_1.size(); bit++) {
        unsigned short int PC_1_bit = 64-PC_1[bit]; //PC_1_bit from left since bitset goes from right to left (bit 7-right = bit 57-left)
        new_key56[bit] = key64[PC_1_bit];
    }

    std::cout << key64 << "---->" << new_key56 << std::endl;

    return new_key56;
}


/*
    Reverse Vector
        -   Input vector
        -   output: Input vector reversed
        -   Not part of DES, solely to deal with nature of bitset DS   
*/
std::vector<unsigned short int> revTable(std::vector<unsigned short int> table) {
    std::vector<unsigned short int> reverse_table;

    for (int i = table.size()-1; i >= 0; --i) {
        reverse_table.push_back(table[i]);
    }

    return reverse_table;
}  