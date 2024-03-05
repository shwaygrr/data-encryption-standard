#include <iostream>
#include <vector>
#include <bitset>
#include <iomanip>
#include <string>
#include <bit>

std::bitset<56> keyPerm56(const std::bitset<64>& key64);
std::bitset<64> initPerm64(const std::bitset<64>& plain_t64);
std::vector<std::bitset<48>> subKeys48(const std::bitset<56>& key_perm56);
std::bitset<64> ciperGen64 (std::bitset<64> key64, std::bitset<64> plain_t64);
std::bitset<32> f32(std::bitset<32> right32, std::bitset<48> subkey48);

std::vector<unsigned short int> revTable(const std::vector<unsigned short int>& table);
void leftRotate(std::bitset<28>& bin28, unsigned int num_rots);

int main() {
    //plain text
    std::bitset<64> plain_t64(0b0000000100100011010001010110011110001001101010111100110111101111);
    //key
    std::bitset<64> key64(0b0001001100110100010101110111100110011011101111001101111111110001);
    
    // std::bitset<64> plain_t64(0X1122334455667788);
    // std::bitset<64> key64(0x752878397493CB70);

    ciperGen64(key64, plain_t64);
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
        // std::cout << 64-bit << "/" << plain_t64[bit] << "-->" << 64-IP_bit << "/" << plain_t64[IP_bit] << std::endl;
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

        std::bitset<56> subkey56(left28.to_string() + right28.to_string());
        subkeys56.push_back(subkey56);
    }

    // int count = 1;
    // for (std::bitset<56> subkey56 : subkeys56) {
    //     std::cout << count << ". " << subkey56 << std::endl;
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

    std::vector<std::bitset<48>> subkeys_perm48;
    // std::cout << "Transformation: " << std::endl;
    for (std::bitset<56>& subkey56 : subkeys56) {
        std::bitset<48> new_subkey48;
        for(int bit = 0; bit < PC_2.size(); bit++) {
            unsigned short int PC_2_bit = 56-PC_2[bit];
            new_subkey48[bit] = subkey56[PC_2_bit];
        }
        subkeys_perm48.push_back(new_subkey48);
    }

    // int count = 1;
    // for (std::bitset<48> subkey48 : subkeys_perm48) {
    //     std::cout << count << ". " << subkey << std::endl;
    //     count++;
    // }

    return subkeys_perm48;
}


/*
    f function
        -   Input: 32-bit right side of previous plain text permutation and 48-bit subkey for correspondig round of encryption
        -   Output: 32-bits to be XOR with left side of previous plain text permutation
*/
std::bitset<32> f32(std::bitset<32> right32, std::bitset<48> subkey48) {
    //right side expansion to 48 bits
     std::vector<unsigned short int> E_bit = {
        32, 1,  2,  3,  4,  5,
        4,  5,  6,  7,  8,  9,
        8,  9,  10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32, 1
    };

    E_bit = revTable(E_bit);

    std::bitset<48> new_right48;
    for(int bit = 0; bit < E_bit.size(); bit++) {
        unsigned short int E_bit_bit = 32-E_bit[bit];
        new_right48[bit] = right32[E_bit_bit];
    }
    // std::cout << new_right48 << std::endl;
    //XOR key and right
    new_right48 = new_right48 ^= subkey48;
    
    // std::cout << new_right48;
    //s_box function
    const std::vector<std::vector<std::vector<int>>> S_BOXES = {
               // S1
        {
            {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
            {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
            {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
            {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
        },
        // S2
        {
            {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
            {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
            {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
            {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
        },
        // S3
        {
            {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
            {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
            {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
            {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
        },
        // S4
        {
            {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
            {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
            {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
            {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
        },
        // S5
        {
            {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
            {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
            {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
            {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
        },
        // S6
        {
            {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
            {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
            {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
            {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
        },
        // S7
        {
            {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
            {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
            {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
            {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
        },
        // S8
        {
            {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
            {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
            {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
            {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
        }
    };

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
        
        std::bitset<4> block4(S_BOXES[i-1][(unsigned short int)row2.to_ulong()][(unsigned short int)column4.to_ulong()]);
        // std::cout << "table " << i << ", row " << (int)row2.to_ulong() << ", column " << (int)column4.to_ulong() << "--->" << block4 << std::endl;
        str_new_right32 += block4.to_string();
    }

    std::bitset<32> new_right32(str_new_right32);
    
    //final permutation
    std::vector<unsigned short int> P_table = {
        16, 7, 20, 21,
        29, 12, 28, 17,
        1, 15, 23, 26,
        5, 18, 31, 10,
        2, 8, 24, 14,
        32, 27, 3, 9,
        19, 13, 30, 6,
        22, 11, 4, 25
    };

    P_table = revTable(P_table);

    std::bitset<32> final32;
    for(int bit = 0; bit < P_table.size(); bit++) {
        unsigned short int P_table_bit = 32-P_table[bit];
        final32[bit] = new_right32[P_table_bit];
    }

    return final32;
}

/*
    16 rounds of Encryption
        -   Input: 64-bit key and 64-bit plain text
        -   Output: 64-bit cipher text
*/
std::bitset<64> ciperGen64 (std::bitset<64> key64, std::bitset<64> plain_t64) {
    //generate 16 keys
    std::bitset<56> key_perm56 = keyPerm56(key64);
    std::vector<std::bitset<48>> subkeys48 = subKeys48(key_perm56);

    //plain text permutation
    std::bitset<64> plain_t_perm64 = initPerm64(plain_t64);
    
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

    //final permutation
    std::vector<unsigned short int> IP_1_inv {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
    };

    IP_1_inv = revTable(IP_1_inv);

    std::bitset<64> cipher_t64;
    for(int bit = 0; bit < IP_1_inv.size(); bit++) {
        unsigned short int IP_1_inv_bit = 64-IP_1_inv[bit];
        cipher_t64[bit] = post_rounds64[IP_1_inv_bit];
    }
    std::cout << cipher_t64;
    return cipher_t64;
}