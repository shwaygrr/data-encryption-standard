#include <iostream>
#include <vector>
#include <bitset>
#include <iomanip>

std::bitset<64> init_perm(std::bitset<64>& plain_64);

int main() {
    //plain text
    std::bitset<64> plain_t64(0b0000000100100011010001010110011110001001101010111100110111101111);
    
    

    return 0;
}

/*
    Step 1: Initial Permutation
        -   Input: 64-bit plain text
        -   Output: 64-bit Permutation 
*/
std::bitset<64> init_perm(std::bitset<64>& plain_t64) {
    
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

    std::bitset<64> new_plain_t64;

    std::cout << "Transformation: " << std::endl;
    for(unsigned short int bit = 0; bit < IP_table.size(); bit++) {
        unsigned short int IP_bit = IP_table[bit]-1;
        std::cout << bit+1 << "/" << plain_t64[bit] << "-->" << IP_bit+1 << "/" << new_plain_t64[IP_bit] << std::endl;
        new_plain_t64[bit] = plain_t64[IP_bit];
    }

    std::cout << plain_t64 << "---->" << new_plain_t64 << std::endl;
    return new_plain_t64;   
}