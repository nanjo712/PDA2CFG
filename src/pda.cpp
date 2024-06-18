#include "pda/pda.hpp"

#include <algorithm>
#include <format>
#include <iostream>
#include <iterator>
#include <string>

#include "cfg/cfg.hpp"

void dfs(std::set<std::string>& states,
         std::set<std::string>::iterator currentState, int total_times,
         int current_times, std::vector<std::vector<std::string>>& sequences,
         std::vector<std::string>& current_sequence)
{
    if (current_times == total_times)
    {
        sequences.push_back(current_sequence);
        return;
    }
    for (auto i = currentState; i != states.end(); i++)
    {
        for (int j = 1; j <= total_times - current_times; j++)
        {
            // choose for j times
            for (int k = 0; k < j; k++)
            {
                current_sequence.push_back(*i);
            }
            dfs(states, std::next(i), total_times, current_times + j, sequences,
                current_sequence);
            for (int k = 0; k < j; k++)
            {
                current_sequence.pop_back();
            }
        }
    }
}

std::vector<std::vector<std::string>> generateSequence(PDA& pda,
                                                       int total_times)
{
    std::vector<std::vector<std::string>> sequences;
    std::vector<std::string> current_sequence;
    dfs(pda.states, pda.states.begin(), total_times, 0, sequences,
        current_sequence);
    return sequences;
}

PDA::PDA() {}

PDA::~PDA() {}

void PDA::addState(std::string state) { states.insert(state); }

void PDA::addInputSymbol(std::string inputSymbol)
{
    inputAlphabet.insert(inputSymbol);
}

void PDA::addStackSymbol(std::string stackSymbol)
{
    stackAlphabet.insert(stackSymbol);
}

void PDA::setStartState(std::string startState)
{
    this->startState = startState;
}

void PDA::setStartStackSymbol(std::string startStackSymbol)
{
    this->startStackSymbol = startStackSymbol;
}

void PDA::addFinalState(std::string finalState)
{
    finalStates.insert(finalState);
}

void PDA::addTransistion(Transistion transistion)
{
    if (transistion.inputSymbol == "e")
    {
        transistion.inputSymbol = "";
    }
    if (transistion.stackPush == "e")
    {
        transistion.stackPush = "";
    }
    transistions.push_back(transistion);
}

void PDA::printPDA()
{
    std::cout << "States: ";
    for (auto state : states)
    {
        std::cout << state << " ";
    }
    std::cout << std::endl;

    std::cout << "Input Alphabet: ";
    for (auto inputSymbol : inputAlphabet)
    {
        std::cout << inputSymbol << " ";
    }
    std::cout << std::endl;

    std::cout << "Stack Alphabet: ";
    for (auto stackSymbol : stackAlphabet)
    {
        std::cout << stackSymbol << " ";
    }
    std::cout << std::endl;

    std::cout << "Start State: " << startState << std::endl;
    std::cout << "Start Stack Symbol: " << startStackSymbol << std::endl;

    std::cout << "Final States: ";
    for (auto finalState : finalStates)
    {
        std::cout << finalState << " ";
    }
    std::cout << std::endl;

    std::cout << "Transistions: " << std::endl;
    for (auto transistion : transistions)
    {
        std::cout << transistion.currentState << " "
                  << (transistion.inputSymbol == "" ? "e"
                                                    : transistion.inputSymbol)
                  << " " << transistion.stackTop << " " << transistion.nextState
                  << " "
                  << (transistion.stackPush == "" ? "e" : transistion.stackPush)
                  << std::endl;
    }
}

ContextFreeGrammar PDA::convertToCFG()
{
    ContextFreeGrammar cfg;
    std::vector<std::string> symbolList;
    char currentSymbol = 'A';
    cfg.addNonTerminal("S");
    cfg.setStartSymbol("S");
    for (auto source : states)
    {
        for (auto target : states)
        {
            for (auto stackTop : stackAlphabet)
            {
                auto symbol =
                    std::format("[{},{},{}]", source, stackTop, target);

                cfg.addNonTerminal(symbol);
            }
        }
    }
    std::cout << "Step 1\n";

    for (auto inputSymbol : inputAlphabet)
    {
        cfg.addTerminal(inputSymbol);
    }
    std::cout << "Step 2\n";

    for (auto state : states)
    {
        auto rhs =
            std::format("[{},{},{}]", startState, startStackSymbol, state);
        cfg.addProduction({"S", {rhs}});
    }
    std::cout << "Step 3\n";

    for (auto transistion : transistions)
    {
        if (transistion.stackPush == "")
        {
            auto lhs = std::format("[{},{},{}]", transistion.currentState,
                                   transistion.stackTop, transistion.nextState);
            auto rhs = transistion.inputSymbol;
            if (rhs == "") rhs = "e";
            cfg.addProduction({lhs, {rhs}});
        }
        else
        {
            auto stackPush = transistion.stackPush;
            auto pushSize = stackPush.size();
            for (auto sta = states.begin(); sta != states.end(); sta++)
            {
                auto chosenStates = generateSequence(*this, pushSize);
                for (auto chosenState : chosenStates)
                {
                    do
                    {
                        auto lhs = std::format(
                            "[{},{},{}]", transistion.currentState,
                            transistion.stackTop, chosenState[pushSize - 1]);
                        std::vector<std::string> rhs = {
                            transistion.inputSymbol};
                        if (rhs[0] == "") rhs = {};
                        auto lastState = transistion.nextState;
                        for (auto i = 0; i < pushSize; i++)
                        {
                            rhs.push_back(std::format("[{},{},{}]", lastState,
                                                      stackPush[i],
                                                      chosenState[i]));
                            lastState = chosenState[i];
                        }
                        cfg.addProduction({lhs, rhs});
                    } while (std::next_permutation(chosenState.begin(),
                                                   chosenState.end()));
                }
            }
        }
    }

    return cfg;
}
