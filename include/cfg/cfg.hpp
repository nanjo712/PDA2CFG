#ifndef CFG_HPP_
#define CFG_HPP_

#include <set>
#include <string>
#include <vector>

struct Production
{
    std::string lhs;
    std::vector<std::string> rhs;

    bool operator==(const Production &other) const
    {
        return lhs == other.lhs && rhs == other.rhs;
    }

    bool operator<(const Production &other) const
    {
        return lhs < other.lhs || (lhs == other.lhs && rhs < other.rhs);
    }
};

class ContextFreeGrammar
{
   private:
    std::vector<Production> productions;
    std::set<std::string> nonTerminals;
    std::set<std::string> terminals;
    std::string startSymbol;

   public:
    ContextFreeGrammar();
    ContextFreeGrammar(std::vector<Production> productions,
                       std::set<std::string> nonTerminals,
                       std::set<std::string> terminals,
                       std::string startSymbol);
    void addProduction(Production production);
    void addNonTerminal(std::string nonTerminal);
    void addTerminal(std::string terminal);
    void setStartSymbol(std::string startSymbol);

    std::vector<Production> getProductions();
    std::set<std::string> getNonTerminals();
    std::set<std::string> getTerminals();
    std::string getStartSymbol();

    void printProductions();
    void printNonTerminals();
    void printTerminals();
    void printStartSymbol();
    void printGrammar();

    bool isNonTerminal(std::string symbol);
    bool isTerminal(std::string symbol);
    bool isStartSymbol(std::string symbol);

    void findUsefulNonTerminals();
    void findReachableSymbols();
    void removeEpsilonProductions();
    void removeUnitProductions();
    void removeUselessProductions();
};

#endif