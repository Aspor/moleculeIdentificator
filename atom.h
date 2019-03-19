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

    int getId();

protected:
    std::string atomicSymbol;


private:
    //vector of cycleId atom is part of
    std::vector<int> cycles;
    int id;
    int charge;

};

#endif // ATOM_H
