#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#include "cfg/cfg.hpp"
#include "pda/pda.hpp"

int main()
{
    freopen("pda.txt", "r", stdin);
    PDA pda;
    int stateSize, inputSize, stackSize, finalSize, transSize;
    std::cout << "Enter the number of states: ";
    std::cin >> stateSize;
    for (int i = 0; i < stateSize; i++)
    {
        std::string state;
        std::cout << "Enter state " << i + 1 << ": ";
        std::cin >> state;
        pda.addState(state);
    }
    std::cout << "Enter the number of input symbols: ";
    std::cin >> inputSize;
    for (int i = 0; i < inputSize; i++)
    {
        std::string input;
        std::cout << "Enter input symbol " << i + 1 << ": ";
        std::cin >> input;
        pda.addInputSymbol(input);
    }
    std::cout << "Enter the number of stack symbols: ";
    std::cin >> stackSize;
    for (int i = 0; i < stackSize; i++)
    {
        std::string stack;
        std::cout << "Enter stack symbol " << i + 1 << ": ";
        std::cin >> stack;
        pda.addStackSymbol(stack);
    }
    std::cout
        << "Enter the number of final states (0 for empty stack acceptance): ";
    std::cin >> finalSize;
    for (int i = 0; i < finalSize; i++)
    {
        std::string final;
        std::cout << "Enter final state " << i + 1 << ": ";
        std::cin >> final;
        pda.addFinalState(final);
    }
    std::cout << "Enter the number of transistions: ";
    std::cin >> transSize;
    for (int i = 0; i < transSize; i++)
    {
        Transistion trans;
        std::cout << "Enter transistion " << i + 1 << ":\n";
        std::cout << "Current state: ";
        std::cin >> trans.currentState;
        std::cout << "Input symbol (e for no input): ";
        std::cin >> trans.inputSymbol;
        std::cout << "Stack top: ";
        std::cin >> trans.stackTop;
        std::cout << "Next state: ";
        std::cin >> trans.nextState;
        std::cout << "Stack push (e for no push):";
        std::cin >> trans.stackPush;
        pda.addTransistion(trans);
    }
    std::cout << "Enter the start state: ";
    std::string startState;
    std::cin >> startState;
    pda.setStartState(startState);

    std::cout << "Enter the start stack symbol: ";
    std::string startStackSymbol;
    std::cin >> startStackSymbol;
    pda.setStartStackSymbol(startStackSymbol);

    std::cout << "The PDA is:\n";
    pda.printPDA();

    ContextFreeGrammar cfg = pda.convertToCFG();
    std::cout << "\nThe equivalent CFG is:\n";
    cfg.printGrammar();

    cfg.removeEpsilonProductions();
    cfg.removeUnitProductions();
    cfg.findUsefulNonTerminals();
    cfg.findReachableSymbols();

    std::cout << "\nThe simplified CFG is:\n";
    cfg.printGrammar();
    return 0;
}