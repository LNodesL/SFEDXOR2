#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <filesystem>

namespace fs = std::filesystem;

// Function to generate a random key of a specified length
std::string GenerateRandomKey(size_t length) {
    const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::string key;
    key.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        key += charset[rand() % (sizeof(charset) - 1)];
    }
    return key;
}

// Function to get the size of a file
size_t GetFileSize(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return 0;
    }
    return static_cast<size_t>(file.tellg());
}

void XorEncryptDecrypt(const std::string& inputFile, const std::string& outputFile, const std::string& key, bool encrypt) {
    std::ifstream file(inputFile, std::ios::binary);
    if (!file) {
        std::cerr << "Could not open input file for reading.\n";
        return;
    }

    size_t fileSize = GetFileSize(inputFile);
    std::vector<unsigned char> buffer(fileSize);
    file.read(reinterpret_cast<char*>(&buffer[0]), fileSize);
    file.close();

    if (encrypt) {
        for (size_t i = 0; i < fileSize; ++i) {
            buffer[i] ^= key[i];
        }
    } else { // Decrypt
        for (size_t i = 0; i < fileSize; ++i) {
            buffer[i] ^= key[i % key.size()];
        }
    }

    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "Could not open output file for writing.\n";
        return;
    }
    outFile.write(reinterpret_cast<const char*>(&buffer[0]), fileSize);
    outFile.close();

    std::cout << "Operation complete. Output file: " << outputFile << std::endl;
}

int main(int argc, char* argv[]) {
    std::string input, output, keyFile, keyOutput, keyString, mode;

    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " -i <inputfile> -o <outputfile> -m <mode> [-k <keyfile>] [-ko <keyoutputfile>] [-ks <keystring>]\n";
        return 1;
    }

    srand(time(nullptr)); // Seed for random number generation

    for (int i = 1; i < argc; i += 2) {
        std::string flag = argv[i];
        if (flag == "-i") {
            input = argv[i + 1];
        } else if (flag == "-o") {
            output = argv[i + 1];
        } else if (flag == "-m") {
            mode = argv[i + 1];
        } else if (flag == "-k") {
            keyFile = argv[i + 1];
        } else if (flag == "-ko") {
            keyOutput = argv[i + 1];
        } else if (flag == "-ks") {
            keyString = argv[i + 1];
        }
    }

    if (input.empty() || output.empty() || mode.empty()) {
        std::cerr << "Input, output files, and mode must be specified.\n";
        return 1;
    }

    if (mode != "encrypt" && mode != "decrypt") {
        std::cerr << "Invalid mode. Use 'encrypt' or 'decrypt'.\n";
        return 1;
    }

    std::string key;
    if (mode == "encrypt") {
        if (keyString.empty()) {
            if (keyFile.empty()) {
                key = GenerateRandomKey(GetFileSize(input));
                std::cout << "Generated Key: " << key << std::endl;
            } else {
                std::ifstream keyIn(keyFile, std::ios::binary);
                if (!keyIn) {
                    std::cerr << "Could not open key file for reading.\n";
                    return 1;
                }
                key = std::string(std::istreambuf_iterator<char>(keyIn), std::istreambuf_iterator<char>());
                keyIn.close();
            }
            if (!keyOutput.empty()) {
                std::ofstream keyOut(keyOutput, std::ios::binary);
                if (!keyOut) {
                    std::cerr << "Could not open key output file for writing.\n";
                    return 1;
                }
                keyOut.write(key.c_str(), key.size());
                keyOut.close();
                std::cout << "Key saved to file: " << keyOutput << std::endl;
            }
        } else {
            key = keyString;
        }
    } else { // Decrypt mode
        if (keyFile.empty() && keyString.empty()) {
            std::cerr << "Key file or key string must be provided for decryption.\n";
            return 1;
        } else if (!keyFile.empty()) {
            std::ifstream keyIn(keyFile, std::ios::binary);
            if (!keyIn) {
                std::cerr << "Could not open key file for reading.\n";
                return 1;
            }
            key = std::string(std::istreambuf_iterator<char>(keyIn), std::istreambuf_iterator<char>());
            keyIn.close();
        } else {
            key = keyString;
        }
    }

    XorEncryptDecrypt(input, output, key, mode == "encrypt");

    return 0;
}
