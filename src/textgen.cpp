#include "textgen.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

MarkovTextGenerator::MarkovTextGenerator(int prefixSize) 
    : npref(prefixSize), rng(std::random_device{}()) {}

void MarkovTextGenerator::buildFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    buildFromText(buffer.str());
}

void MarkovTextGenerator::buildFromText(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> words;
    std::string word;
    
    while (iss >> word) {
        // Очистка от знаков препинания (упрощённая версия)
        word.erase(std::remove_if(word.begin(), word.end(), 
            [](char c) { return std::ispunct(c) && c != '-' && c != '\''; }), 
            word.end());
        if (!word.empty()) {
            words.push_back(word);
        }
    }
    
    if (words.size() < static_cast<size_t>(npref + 1)) {
        throw std::runtime_error("Text too short for building model");
    }
    
    // Заполнение таблицы
    Prefix prefix;
    for (int i = 0; i < npref; ++i) {
        prefix.push_back(words[i]);
    }
    
    for (size_t i = npref; i < words.size(); ++i) {
        statetab[prefix].push_back(words[i]);
        prefix.pop_front();
        prefix.push_back(words[i]);
    }
}

void MarkovTextGenerator::addSuffix(const Prefix& prefix, const std::string& suffix) {
    statetab[prefix].push_back(suffix);
}

std::string MarkovTextGenerator::getRandomSuffix(const Prefix& prefix) {
    auto it = statetab.find(prefix);
    if (it == statetab.end() || it->second.empty()) {
        return "";
    }
    
    std::uniform_int_distribution<size_t> dist(0, it->second.size() - 1);
    return it->second[dist(rng)];
}

std::string MarkovTextGenerator::generate(int maxWords) {
    if (statetab.empty()) {
        throw std::runtime_error("No data in state table");
    }
    
    // Выбор случайного начального префикса
    auto it = statetab.begin();
    std::uniform_int_distribution<size_t> dist(0, statetab.size() - 1);
    std::advance(it, dist(rng));
    
    Prefix prefix = it->first;
    
    std::vector<std::string> result(prefix.begin(), prefix.end());
    
    for (int i = npref; i < maxWords; ++i) {
        std::string next = getRandomSuffix(prefix);
        if (next.empty()) {
            break;
        }
        result.push_back(next);
        prefix.pop_front();
        prefix.push_back(next);
    }
    
    // Сборка результата
    std::stringstream ss;
    for (size_t i = 0; i < result.size(); ++i) {
        if (i > 0) ss << " ";
        ss << result[i];
    }
    
    return ss.str();
}
