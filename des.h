#ifndef DES_H
#define DES_H

#include <iostream>
#include <bitset>
#include <string>

#include "des_tables.h"


//helper functions
std::vector<unsigned short int> revTable(const std::vector<unsigned short int>& table); //Return a table reversed -- Convenient for bitset
void leftRotate(std::bitset<28>& bin28, unsigned int num_rots); //Rotate 28-bit binary left num_rots times

std::vector<std::bitset<48>> genSubkeys48(const std::bitset<56>& key_perm56); //generate subkeys from 56-bit permutated key
std::bitset<64> cipherGen64 (std::bitset<64> key64, std::bitset<64> plain_t64); //
std::bitset<32> f32(std::bitset<32> right32, std::bitset<48> subkey48); //fiestel function operating on right and subkey

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
    
    //permutation
    for(int bit = 0; bit < table.size(); bit++) {
        unsigned short int table_bit = input.size()-rev_table[bit];
        permutation[bit] = input[table_bit];
    }

    return permutation;
}
#endif