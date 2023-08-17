#include <iostream>
#include <fstream>
#include <map>
#include <cmath>

#include "compact_trie.hpp"

// Compression algorithm.
void compress(std::fstream &inputFile, FILE *outputFile)
{
    // Declares the tree to be used as auxiliary data structure.
    CompactTrie *tree = new CompactTrie;

    // Verifies the amount of codes that will be necessary.
    char inputChar;
    std::string holdStr = "";
    while (inputFile >> std::noskipws >> inputChar)
    {
        // If the current string is already on the tree, continues for the next character.
        if (tree->search(holdStr + inputChar))
        {
            holdStr = holdStr + inputChar;
        }
        // If the current string is not on the tree,
        // ...inserts the (prefix + read character) it into the data structure.
        else
        {
            tree->insert(holdStr + inputChar);
            holdStr = "";
        }
    }

    // Based on the maximum amount of codes, set the length in bits of the integer.
    float codeLimit = tree->getUniversalCode();
    long long int bitAmount = ceil(log2(codeLimit) / 8);
    fwrite(&bitAmount, 4, 1, outputFile);

    // Deallocates all the allocated elements.
    delete tree;

    // Reads each character of the input text.
    inputFile.clear();
    inputFile.seekg(0, std::ios::beg);
    tree = new CompactTrie;
    holdStr = "";
    char lastInput;
    while (true)
    {
        lastInput = inputChar;
        inputFile >> std::noskipws >> inputChar;

        // If the file has ended, outputs the (code + character) nonetheless.
        if (inputFile.eof())
        {
            if (holdStr != "")
            {
                holdStr.pop_back();
                int holdStrCode = tree->getCode(holdStr);
                fwrite(&holdStrCode, bitAmount, 1, outputFile);
                fwrite(&inputChar, sizeof(char), 1, outputFile);
            }
            break;
        }

        // If the current string is already on the tree, continues for the next character.
        if (tree->search(holdStr + inputChar))
        {
            holdStr = holdStr + inputChar;
        }
        // If the current string is not on the tree,
        // ...outputs the code for its (prefix + read character) and inserts it into the data structure.
        else
        {
            int holdStrCode = tree->getCode(holdStr);
            fwrite(&holdStrCode, bitAmount, 1, outputFile);
            fwrite(&inputChar, sizeof(char), 1, outputFile);
            tree->insert(holdStr + inputChar);
            holdStr = "";
        }
    }

    // Deallocates all the allocated elements.
    delete tree;
}

// Decompression algorithm.
void decompress(FILE *inputFile, std::fstream &outputFile)
{
    // Declares the tree to be used as auxiliary data structure.
    CompactTrie *tree = new CompactTrie;

    // Collects the amount of bits used to compress the file.
    long long int bitAmount = 0;
    fread(&bitAmount, 4, 1, inputFile);

    // Reads the (code + character) and reconstructs the text.
    char inputChar;
    int code = 0;
    int globalCode = 1;
    std::string holdStr = "";
    tree->insertDec("0", "");
    while (true)
    {
        fread(&code, bitAmount, 1, inputFile);
        fread(&inputChar, sizeof(char), 1, inputFile);

        // Checks if the end of the file has been set.
        if (feof(inputFile))
        {
            break;
        }

        holdStr = tree->getCodeDec(std::to_string(code));
        outputFile << std::noskipws << holdStr;
        outputFile << std::noskipws << inputChar;
        tree->insertDec(std::to_string(globalCode), holdStr + inputChar);
        globalCode++;
    }

    // Deallocates all the allocated elements.
    delete tree;
}

int main(int argc, char *argv[])
{
    // Verifies if a compression or a decompression is to be performed and act accordingly.
    if (argv[1][1] == 'c')
    {
        // Opens the input/output files.
        std::fstream inputFile;
        inputFile.open(argv[2], std::ios::in);
        FILE *outputFile;
        if (argc > 4)
        {
            outputFile = fopen(argv[4], "wb");
        }
        else
        {
            std::string outputFileName = argv[2];
            outputFileName.replace(outputFileName.length() - 4, 4, ".z78");
            outputFile = fopen(outputFileName.c_str(), "wb");
        }

        // Compresses the input file onto the output file.
        compress(inputFile, outputFile);

        // Closes used files.
        inputFile.close();
        fclose(outputFile);
    }
    else if (argv[1][1] == 'x')
    {
        // Opens the input/output files.
        FILE *inputFile;
        inputFile = fopen(argv[2], "rb");
        std::fstream outputFile;
        if (argc > 4)
        {
            outputFile.open(argv[4], std::ios::out);
        }
        else
        {
            std::string outputFileName = argv[2];
            outputFileName.replace(outputFileName.length() - 4, 4, ".txt");
            outputFile.open(outputFileName, std::ios::out);
        }

        // Decompresses the input file onto the output file.
        decompress(inputFile, outputFile);

        // Closes used files.
        fclose(inputFile);
        outputFile.close();
    }

    return 0;
}