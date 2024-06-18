#include "cfg/cfg.hpp"

#include <algorithm>
#include <iostream>
#include <string>

ContextFreeGrammar::ContextFreeGrammar()
{
    this->productions = std::vector<Production>();
    this->nonTerminals = std::set<std::string>();
    this->terminals = std::set<std::string>();
    this->startSymbol = '\0';
}

ContextFreeGrammar::ContextFreeGrammar(std::vector<Production> productions,
                                       std::set<std::string> nonTerminals,
                                       std::set<std::string> terminals,
                                       std::string startSymbol)
{
    this->productions = productions;
    this->nonTerminals = nonTerminals;
    this->terminals = terminals;
    this->startSymbol = startSymbol;
}

void ContextFreeGrammar::addProduction(Production production)
{
    if (production.rhs[0] == "e")
    {
        production.rhs.clear();
        std::cout << "Getting epsilon production\n";
    }
    this->productions.push_back(production);
}

void ContextFreeGrammar::addNonTerminal(std::string nonTerminal)
{
    this->nonTerminals.insert(nonTerminal);
}

void ContextFreeGrammar::addTerminal(std::string terminal)
{
    this->terminals.insert(terminal);
}

void ContextFreeGrammar::setStartSymbol(std::string startSymbol)
{
    this->startSymbol = startSymbol;
}

std::vector<Production> ContextFreeGrammar::getProductions()
{
    return this->productions;
}

std::set<std::string> ContextFreeGrammar::getNonTerminals()
{
    return this->nonTerminals;
}

std::set<std::string> ContextFreeGrammar::getTerminals()
{
    return this->terminals;
}

std::string ContextFreeGrammar::getStartSymbol() { return this->startSymbol; }

void ContextFreeGrammar::printProductions()
{
    std::cout << "Productions:\n";
    for (Production production : this->productions)
    {
        std::cout << production.lhs << " -> ";
        if (production.rhs.size() == 0)
        {
            std::cout << "e\n";
            continue;
        }
        for (auto symbol : production.rhs)
        {
            std::cout << symbol;
        }
        std::cout << "\n";
    }
}

void ContextFreeGrammar::printNonTerminals()
{
    std::cout << "Non-terminals:\n";
    for (auto &nonTerminal : this->nonTerminals)
    {
        std::cout << nonTerminal << " ";
    }
    std::cout << "\n";
}

void ContextFreeGrammar::printTerminals()
{
    std::cout << "Terminals:\n";
    for (auto terminal : this->terminals)
    {
        std::cout << terminal << " ";
    }
    std::cout << "\n";
}

void ContextFreeGrammar::printStartSymbol()
{
    std::cout << "Start symbol: " << this->startSymbol << "\n";
}

void ContextFreeGrammar::printGrammar()
{
    this->printProductions();
    this->printNonTerminals();
    this->printTerminals();
    this->printStartSymbol();
}

bool ContextFreeGrammar::isNonTerminal(std::string symbol)
{
    return this->nonTerminals.find(symbol) != this->nonTerminals.end();
}

bool ContextFreeGrammar::isTerminal(std::string symbol)
{
    return this->terminals.find(symbol) != this->terminals.end();
}

bool ContextFreeGrammar::isStartSymbol(std::string symbol)
{
    return this->startSymbol == symbol;
}

void ContextFreeGrammar::findUsefulNonTerminals()
{
    std::set<std::string> N0;
    std::set<std::string> Ndot;
    for (Production production : this->productions)
    {
        auto lhs = production.lhs;
        auto rhs = production.rhs;
        if (std::all_of(rhs.begin(), rhs.end(), [this](std::string symbol)
                        { return isTerminal(symbol); }))
        {
            N0.insert(lhs);
        }
    }

    while (Ndot != N0)
    {
        N0 = Ndot;
        for (Production production : this->productions)
        {
            auto lhs = production.lhs;
            auto rhs = production.rhs;
            if (std::all_of(rhs.begin(), rhs.end(),
                            [&N0, this](std::string symbol) {
                                return N0.find(symbol) != N0.end() ||
                                       isTerminal(symbol);
                            }))
            {
                Ndot.insert(lhs);
            }
        }
    }

    this->nonTerminals = Ndot;

    std::vector<Production> newProductions;
    for (Production production : this->productions)
    {
        auto lhs = production.lhs;
        auto rhs = production.rhs;
        if (Ndot.find(lhs) != Ndot.end() &&
            std::all_of(
                rhs.begin(), rhs.end(), [&N0, this](std::string symbol)
                { return N0.find(symbol) != N0.end() || isTerminal(symbol); }))
        {
            newProductions.push_back(production);
        }
    }

    this->productions = newProductions;
}

void ContextFreeGrammar::findReachableSymbols()
{
    std::set<std::string> N0 = {this->startSymbol};
    std::set<std::string> Ndot = {this->startSymbol};

    do
    {
        N0 = Ndot;
        for (Production production : this->productions)
        {
            auto lhs = production.lhs;
            auto rhs = production.rhs;
            if (N0.find(lhs) != N0.end())
            {
                for (auto symbol : rhs)
                {
                    Ndot.insert(symbol);
                }
            }
        }
    } while (Ndot != N0);

    auto nonTerminalsCopy = this->nonTerminals;
    nonTerminalsCopy.clear();
    std::set_intersection(
        nonTerminalsCopy.begin(), nonTerminalsCopy.end(), Ndot.begin(),
        Ndot.end(),
        std::inserter(this->nonTerminals, this->nonTerminals.begin()));

    auto terminalsCopy = this->terminals;
    terminalsCopy.clear();
    std::set_intersection(
        terminalsCopy.begin(), terminalsCopy.end(), Ndot.begin(), Ndot.end(),
        std::inserter(this->terminals, this->terminals.begin()));

    std::vector<Production> newProductions;
    for (Production production : this->productions)
    {
        auto lhs = production.lhs;
        auto rhs = production.rhs;
        if (Ndot.find(lhs) != Ndot.end() ||
            std::all_of(
                rhs.begin(), rhs.end(), [&N0, this](std::string symbol)
                { return N0.find(symbol) != N0.end() || isTerminal(symbol); }))
        {
            newProductions.push_back(production);
        }
    }

    std::sort(newProductions.begin(), newProductions.end());
    newProductions.erase(
        std::unique(newProductions.begin(), newProductions.end()),
        newProductions.end());
    this->productions = newProductions;
}

void ContextFreeGrammar::removeEpsilonProductions()
{
    std::set<std::string> nullable;
    for (Production production : this->productions)
    {
        if (production.rhs.empty())
        {
            nullable.insert(production.lhs);
        }
    }

    std::set<std::string> N0;
    do
    {
        N0 = nullable;
        for (Production production : this->productions)
        {
            auto lhs = production.lhs;
            auto rhs = production.rhs;
            if (std::all_of(
                    rhs.begin(), rhs.end(), [&nullable](std::string symbol)
                    { return nullable.find(symbol) != nullable.end(); }))
            {
                nullable.insert(lhs);
            }
        }
    } while (nullable != N0);

    std::vector<Production> newProductions;
    for (Production production : this->productions)
    {
        auto lhs = production.lhs;
        auto rhs = production.rhs;
        if (rhs.empty())
        {
            continue;
        }

        std::vector<std::vector<std::string>> subsets;
        subsets.push_back({});
        for (auto symbol : rhs)
        {
            if (nullable.find(symbol) != nullable.end())
            {
                for (auto &subset : subsets)
                {
                    subset.push_back(symbol);
                }

                std::vector<std::vector<std::string>> newSubsets;
                for (auto &subset : subsets)
                {
                    newSubsets.push_back(subset);
                    subset.pop_back();
                    newSubsets.push_back(subset);
                }
                subsets = newSubsets;
            }
            else
            {
                for (auto &subset : subsets)
                {
                    subset.push_back(symbol);
                }
            }
        }

        for (auto &subset : subsets)
        {
            if (subset.empty()) continue;
            newProductions.push_back({lhs, subset});
        }
    }

    if (nullable.find(this->startSymbol) != nullable.end())
    {
        std::string newStartSymbol = this->startSymbol + "'";
        this->addNonTerminal(newStartSymbol);
        newProductions.push_back({newStartSymbol, {}});
        newProductions.push_back({newStartSymbol, {this->startSymbol}});
        this->setStartSymbol(newStartSymbol);
    }

    std::sort(newProductions.begin(), newProductions.end());
    newProductions.erase(
        std::unique(newProductions.begin(), newProductions.end()),
        newProductions.end());
    this->productions = newProductions;
}

void ContextFreeGrammar::removeUnitProductions()
{
    std::vector<Production> newProductions;
    for (auto nonTerminal : nonTerminals)
    {
        std::set<std::string> N0 = {nonTerminal};
        std::set<std::string> Ndot = {nonTerminal};

        do
        {
            N0 = Ndot;
            for (Production production : this->productions)
            {
                auto lhs = production.lhs;
                auto rhs = production.rhs;
                if (N0.find(lhs) != N0.end())
                {
                    if (rhs.size() == 1 && isNonTerminal(rhs[0]))
                    {
                        Ndot.insert(rhs[0]);
                    }
                }
            }
        } while (Ndot != N0);

        for (Production production : this->productions)
        {
            auto lhs = production.lhs;
            auto rhs = production.rhs;
            if (rhs.size() == 1 && isNonTerminal(rhs[0])) continue;
            if (std::find(Ndot.begin(), Ndot.end(), lhs) != Ndot.end())
            {
                newProductions.push_back({nonTerminal, rhs});
            }
        }
    }

    std::sort(newProductions.begin(), newProductions.end());
    newProductions.erase(
        std::unique(newProductions.begin(), newProductions.end()),
        newProductions.end());
    this->productions = newProductions;
}
