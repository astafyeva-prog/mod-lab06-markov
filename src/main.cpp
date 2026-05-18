#include "textgen.h"
#include <iostream>
#include <fstream>
#include <filesystem>

int main(int argc, char* argv[]) {
    try {
        std::string inputFile = "input.txt";
        std::string outputFile = "result/gen.txt";
        int prefixSize = 2;
        int maxWords = 1000;
        
        if (argc > 1) inputFile = argv[1];
        if (argc > 2) maxWords = std::stoi(argv[2]);
        
        // Создаём директорию result, если её нет
        std::filesystem::create_directory("result");
        
        MarkovTextGenerator generator(prefixSize);
        
        std::cout << "Building model from " << inputFile << "..." << std::endl;
        generator.buildFromFile(inputFile);
        
        std::cout << "Generating " << maxWords << " words..." << std::endl;
        std::string generated = generator.generate(maxWords);
        
        std::ofstream out(outputFile);
        out << generated;
        out.close();
        
        std::cout << "Generated text saved to " << outputFile << std::endl;
        std::cout << "\n--- Preview (first 500 chars) ---\n";
        std::cout << generated.substr(0, std::min<size_t>(500, generated.size())) << "...\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
