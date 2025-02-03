//
// Created by David Yang on 2025-02-02.
//

#pragma once

namespace fs = std::filesystem;

class Preprocessor {
public:
    std::string& filePath; // Member variable as a modifiable reference

    explicit Preprocessor(std::string& filePath) : filePath(filePath) {}

    void createFolderAndFile(const std::string& folderName, const std::string& fileName) {
        // Create the folder
        if (!fs::exists(folderName)) { // Check if the folder already exists
            if (fs::create_directory(folderName)) {
                std::cout << "Folder created: " << folderName << std::endl;
            } else {
                std::cerr << "Error: Failed to create folder " << folderName << std::endl;
                return;
            }
        } else {
            std::cout << "Folder already exists: " << folderName << std::endl;
            // existing location
            std::cout << "Folder path: " << fs::absolute(folderName) << std::endl;
        }

        // Path of the file inside the folder
        filePath = folderName + "/" + fileName;

        // Create the file inside the folder
        std::ofstream file(filePath); // Open file for writing
        if (file.is_open()) {
            file.close();
            std::cout << "File created: " << filePath << std::endl;
        } else {
            std::cerr << "Error: Failed to create file " << filePath << std::endl;
        }
    }



    static std::string import(std::string& module) {
        std::erase_if(module, [](const char c) { return c == '\"' || c == ' '; });
        std::cout << "Preprocessor: Importing module '" << module << "'" << std::endl;

        std::ifstream file(std::getenv("CVASTSTDLIB") + module);

        if (!file.is_open()) {
            std::cerr << "Preprocessor Error: Failed to open file '" << module << "'" << std::endl;
        }

        std::string fileContent((std::istreambuf_iterator(file)),
                            std::istreambuf_iterator<char>());

        // Remove newline characters
        std::erase(fileContent, '\n');

        file.close();

        return fileContent;

    }


    std::string preprocess() {
        std::ifstream file(filePath);
        std::string appendTo;

        if (!file.is_open()) {
            std::cerr << "Preprocessor Error: Failed to open file '" << filePath << "'" << std::endl;
        }

        std::string line;
        std::string wholeFile;
        while (std::getline(file, line)) { // Read the file line-by-line
            if (line.empty()) {
                continue; // Skip empty lines
            }

            if (line[0] == '@') { // Check if the current line starts with '@'
                std::cout << "Preprocessor: Found directive '" << line << "'" << std::endl;
                if (line.starts_with("@import")) {
                    std::string module = line.substr(7);
                    appendTo += import(module);
                }
            } else {
                wholeFile += line;
            }


        }
        createFolderAndFile("CVastCache", "cache.cv");

        std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
        wholeFile = appendTo + wholeFile;
        outFile << wholeFile;

        outFile.close();

        file.close();

        return appendTo;
    }
};