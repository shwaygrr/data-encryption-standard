#include <iostream>
#include <vector>
#include <bitset>
#include <iomanip>
#include <string>
#include <bit>

std::bitset<56> keyPerm56(const std::bitset<64>& key64);
std::bitset<64> initPerm64(const std::bitset<64>& plain_64);
std::vector<std::bitset<48>> subKeys48(const std::bitset<56>& key_perm56);

std::vector<unsigned short int> revTable(const std::vector<unsigned short int>& table);
void leftRotate(std::bitset<28>& bin28, unsigned int num_rots);

int main() {
    //plain text
    std::bitset<64> plain_t64(0b0000000100100011010001010110011110001001101010111100110111101111);
    
    // initPerm64(plain_t64);


    //key
    std::bitset<64> key64(0b0001001100110100010101110111100110011011101111001101111111110001);
    std::bitset<56> key_perm56 = keyPerm56(key64);
    
    subKeys48(key_perm56);
    return 0;
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


/*
    Initial Permutation
        -   Input: 64-bit plain text
        -   Output: 64-bit Permutation 
*/
std::bitset<64> initPerm64(const std::bitset<64>& plain_t64) {
    
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

    IP_table = revTable(IP_table);
 
    std::bitset<64> new_plain_t64;

    // std::cout << "Transformation: " << std::endl;
    for(int bit = 0; bit < IP_table.size(); bit++) {
        unsigned short int IP_bit = IP_table.size()-IP_table[bit]; //IP_bit from left since bitset goes from right to left (bit 7-right = bit 57-left)
        std::cout << 64-bit << "/" << plain_t64[bit] << "-->" << 64-IP_bit << "/" << plain_t64[IP_bit] << std::endl;
        new_plain_t64[bit] = plain_t64[IP_bit];
    }

    // std::cout << plain_t64 << "---->" << new_plain_t64 << std::endl;
    return new_plain_t64;   
}


/*
    Key Permutation to 56 bits
        -   Input: 64-bit key
        -   Output: 56-bit key (temp)
*/
std::bitset<56> keyPerm56(const std::bitset<64>& key64) {
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
    
    // std::cout << "Transformation: " << std::endl;
    for(int bit = 0; bit < PC_1.size(); bit++) {
        unsigned short int PC_1_bit = 64-PC_1[bit]; //PC_1_bit from left since bitset goes from right to left (bit 7-right = bit 57-left)
        new_key56[bit] = key64[PC_1_bit];
    }

    // std::cout << key64 << "---->" << new_key56 << std::endl;

    return new_key56;
}


/*
    Generate 16 48-bit subkeys that have gone through trasformation
        -   Input: 56-bit Key permutation
        -   Output: 16 48-bit subkeys
*/
std::vector<std::bitset<48>> subKeys48(const std::bitset<56>& key_perm56) {
    
    //split key perm
    std::string str_key_perm56 = key_perm56.to_string();
    
    std::bitset<28> left28(str_key_perm56.substr(0, 28)); // first half
    std::bitset<28> right28(str_key_perm56.substr(28)); //second half

    // std::cout << left28 << std::endl << right28;
    std::vector<std::bitset<56>> subkeys;
    
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

        std::bitset<56> subkey(left28.to_string() + right28.to_string());
        subkeys.push_back(subkey);
    }

    // int count = 1;
    // for (std::bitset<56> subkey : subkeys) {
    //     std::cout << count << ". " << subkey << std::endl;
    //     count++;
    // }
    

    //permutate each sum key to 48-bits using PC_2
    std::vector<unsigned short int> PC_2 = {
        14, 17, 11, 24, 1,  5,
        3,  28, 15, 6,  21, 10,
        23, 19, 12, 4,  26, 8,
        16, 7,  27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
    };

    PC_2 = revTable(PC_2);

    std::vector<std::bitset<48>> subkeys48_perm;
    // std::cout << "Transformation: " << std::endl;
    for (std::bitset<56>& subkey56 : subkeys) {
        std::bitset<48> new_subkey48;
        for(int bit = 0; bit < PC_2.size(); bit++) {
            unsigned short int PC_2_bit = 56-PC_2[bit];
            new_subkey48[bit] = subkey56[PC_2_bit];
        }
        subkeys48_perm.push_back(new_subkey48);
    }

    // int count = 1;
    // for (std::bitset<48> subkey : subkeys48_perm) {
    //     std::cout << count << ". " << subkey << std::endl;
    //     count++;
    // }

    return subkeys48_perm;
}