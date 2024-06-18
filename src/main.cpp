#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <iostream>

#include "cfg/cfg.hpp"

int main()
{
    freopen("cfg.txt", "r", stdin);
    int Ns, Ts, lines;
    ContextFreeGrammar cfg;
    std::cout << "Enter the number of non-terminals: ";
    std::cin >> Ns;
    std::cout << "Enter the non-terminals: ";
    for (int i = 0; i < Ns; i++)
    {
        std::string nonTerminal;
        std::cin >> nonTerminal;
        cfg.addNonTerminal(nonTerminal);
    }
    std::cout << "Enter the number of terminals: ";
    std::cin >> Ts;
    std::cout << "Enter the terminals: ";
    for (int i = 0; i < Ts; i++)
    {
        std::string terminal;
        std::cin >> terminal;
        cfg.addTerminal(terminal);
    }
    std::cout << "Enter the start symbol: ";
    std::string startSymbol;
    std::cin >> startSymbol;
    cfg.setStartSymbol(startSymbol);
    std::cout << "Enter the number of productions: ";
    std::cin >> lines;
    for (int i = 0; i < lines; i++)
    {
        Production production;
        std::cout << "Enter the production: ";
        std::cin >> production.lhs;
        std::string rhs;
        std::cin >> rhs;
        for (char c : rhs)
        {
            production.rhs.push_back(std::string(1, c));
        }
        cfg.addProduction(production);
    }
    cfg.printGrammar();
    std::cout << "Removing epsilon productions\n";
    cfg.removeEpsilonProductions();
    cfg.printGrammar();

    std::cout << "Removing unit productions\n";
    cfg.removeUnitProductions();
    cfg.printGrammar();

    std::cout << "Remove useless symbol\n";
    cfg.findUsefulNonTerminals();
    cfg.findReachableSymbols();

    std::cout << "\nThe grammar is:\n";
    cfg.printGrammar();
    return 0;
}