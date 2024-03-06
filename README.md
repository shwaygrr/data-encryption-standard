# Data Encryption Standard (DES) Implementation

## Overview
This project implements the Data Encryption Standard (DES) algorithm witht the Electronic Code Block(ECB) mode of operation in C++. DES is a widely used symmetric-key algorithm for data encryption and decryption. DES operates on 64-bit blocks of plaintext using a 64-bit key, applying various permutations and substitutions to achieve encryption and decryption.

## Implementation Details
- Language: C++
- Mode of Operation: Electronic Code Block (ECB)
- Key Length: 64 bits
- Block Size: 64 bits

## Files Included
- `des_tables.h` and `des_tables.cpp`: to declare and define the tables used in DES 
- `des.h`: The header file for the functions and templates used to in the DES
- `des.cpp`: The source code file containing the implementation of the DES and its helper functions.
- `main.cpp`: The main source code file containing the ECB implementationn and usage
- `README.md`: This file providing an overview of the project and instructions for use.

## How to run with VSCode
1. Open the `main.cpp` directory.
2. Build and Run
3. If there is error linking the files
    1. Open the `tasks.json` file in your Visual Studio Code workspace (`.vscode` directory)
    2. Ensure that the `args` property within the `"tasks"` array looks like the following:
        ```json
            "args": [
                "-fdiagnostics-color=always",
                "-g",
                "${workspaceFolder}/*.cpp",
                "-o",
                "fileDirname{fileDirname}\\{fileBasenameNoExtension}.exe"
            ],
        ```
    3. Save the `tasks.json` file.
    4. Build and Run in `main.cpp`