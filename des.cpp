#include <iostream>
#include <vector>
#include <bitset>

std::bitset<64> init_perm(std::bitset<64>& plain_64);

int main() {
    std::bitset<64> plain_t64(0b000000010010001101000101011001111000100110101011110011011110111);

    std::bitset<64> ans = init_perm(plain_t64);

    std::cout << plain_t64 << std::endl;
    std::cout << ans;

    return 0;
}

/*
    Step 1: Initial Permutation
        -   Input: 64-bit plain text
        -   Output: 64-bit Permutation 
*/
std::bitset<64> init_perm(std::bitset<64>& plain_t64) {
    
    std::vector<int8_t> IP_table = {
        58,	50,	42,	34,	26,	18,	10,	2,
        60,	52,	44,	36,	28,	20,	12,	4,
        62,	54,	46,	38,	30,	22,	14,	6,
        64,	56,	48,	40,	32,	24,	16,	8,
        57,	49,	41,	33,	25,	17,	9,	1,
        59,	51,	43,	35,	27,	19,	11,	3,
        61,	53,	45,	37,	29,	21,	13,	5,
        63,	55,	47,	39,	31,	23,	15,	7
    };

    std::bitset<64> new_plain_t64 = plain_t64;
    
    for (int8_t i = 0; i < 64; i++) {
        int8_t IP_bit = IP_table[i];
        new_plain_t64[i] = plain_t64[IP_bit-1];
    }

    return new_plain_t64;   
}