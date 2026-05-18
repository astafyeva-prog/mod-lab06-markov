#ifndef TEXTGEN_H
#define TEXTGEN_H

#include <deque>
#include <map>
#include <vector>
#include <string>
#include <random>

class MarkovTextGenerator {
private:
    typedef std::deque<std::string> Prefix;
    typedef std::map<Prefix, std::vector<std::string>> StateTable;
    
    StateTable statetab;
    int npref;
    std::mt19937 rng;
    
public:
    MarkovTextGenerator(int prefixSize = 2);
    
    void buildFromFile(const std::string& filename);
    void buildFromText(const std::string& text);
    
    std::string generate(int maxWords);
    
    // Для тестирования
    StateTable& getStateTable() { return statetab; }
    void clear() { statetab.clear(); }
    void addSuffix(const Prefix& prefix, const std::string& suffix);
    std::string getRandomSuffix(const Prefix& prefix);
};

#endif
