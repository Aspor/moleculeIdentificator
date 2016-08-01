#ifndef IDENTIFICATOR_H
#define IDENTIFICATOR_H

#include <string>
#include <vector>
#include <array>
#include "atom.h"

class Identificator
{
public:
    Identificator();
    std::string ind(std::vector<Atom*> atoms, std::vector<std::vector< int> > bonds);
    std::string name(std::string);

private:
    void dfs(int atomID,std::vector<std::vector<int> > bonds ,bool newBranch, int parentID);
    std::vector<Atom*> atoms;
    std::vector<bool> visitedAtoms;
    std::vector<std::vector<Atom*> > branches;
    std::vector<Atom* > branchPoints;
    std::vector<std::array<int,2> > cyclePoints;
    int branchID;
    int cycleID;
    std::string generateSMILE(std::vector<Atom*> branch,int branchID,bool firstBranch, std::vector<std::vector<int > > bonds);

};

#endif // IDENTIFICATOR_H
