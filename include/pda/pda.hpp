#ifndef PDA_HPP_
#define PDA_HPP_

#include <iterator>
#include <set>
#include <string>
#include <vector>

class ContextFreeGrammar;

struct Transistion
{
    std::string currentState;
    std::string inputSymbol;
    std::string stackTop;
    std::string nextState;
    std::string stackPush;
};

class PDA
{
   private:
    std::set<std::string> states;
    std::set<std::string> inputAlphabet;
    std::set<std::string> stackAlphabet;
    std::string startState;
    std::string startStackSymbol;
    std::set<std::string> finalStates;  // empty for empty stack acceptance
    std::vector<Transistion> transistions;
    friend std::vector<std::vector<std::string>> generateSequence(
        PDA& pda, int total_times);

   public:
    PDA();
    ~PDA();

    void addState(std::string state);
    void addInputSymbol(std::string inputSymbol);
    void addStackSymbol(std::string stackSymbol);
    void setStartState(std::string startState);
    void setStartStackSymbol(std::string startStackSymbol);
    void addFinalState(std::string finalState);
    void addTransistion(Transistion transistion);

    void printPDA();

    ContextFreeGrammar convertToCFG();
};

#endif