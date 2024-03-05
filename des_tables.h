#ifndef DES_TABLES_H
#define DES_TABLES_H

#include <vector>

// DES tables
extern const std::vector<unsigned short int> IP_table;

extern const std::vector<unsigned short int> PC_1;

extern const std::vector<unsigned short int> IP_1_inv;

extern const std::vector<unsigned short int> P_table;

extern const std::vector<std::vector<std::vector<int>>> S_BOXES;

extern const std::vector<unsigned short int> E_bit;

extern const std::vector<unsigned short int> PC_2;
#endif // DES_TABLES_H
