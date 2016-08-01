#ifndef ATOM_H
#define ATOM_H

#include <vector>
#include <string>

class Atom
{
public:
    Atom();
    Atom(std::string element, int charge, int id);
    void setElement(std::string);
    void setCharge(int);
    void addCycle(int cycleId);
    void clearCycles();
    std::string SMILEout();
    std::string getElement();


private:
    std::vector<int> cycles;
    std::string atomicSymbol;
    int id;
    int charge;

};

#endif // ATOM_H
