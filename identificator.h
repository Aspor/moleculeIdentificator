#ifndef IDENTIFICATOR_H
#define IDENTIFICATOR_H

#include <string>
#include <vector>
#include <map>

#include <array>
#include "atom.h"

class Identificator
{
public:
    Identificator();
    std::string ind(std::vector<Atom*> atoms, std::vector<std::vector< int> > bonds);
    std::string name(std::string);

private:
    void dfs(int atomID, std::vector<std::vector<int> > bonds , bool newBranch, int parentID);
    std::vector<Atom*> atoms;
    std::vector<bool> visitedAtoms;
    std::multimap <Atom* , std::vector<Atom* > > branchMap;
    std::vector<std::array<int,2> > cyclePoints;
    int cycleID;
    std::multimap<Atom*,std::vector<Atom*> >::iterator branchInd;

    std::multimap<Atom*,int> cycleMap;
    std::multimap<int,int> cycleBonds;

    //std::string generateSMILE(std::vector<Atom*> branch,int branchID, std::vector<std::vector<int > > bonds);
    std::string generateSMILE(std::vector<Atom*> branch, std::vector<std::vector<int> >bonds);


    std::string canonizeSMILE(std::string);



//    void canonize(std::vector<Atom*> skeleton, std::vector<std::vector< Atom*> >branches, std::vector<Atom*> branchPoints, int branchind);
    void canonize(std::vector<Atom *> skeleton, std::multimap<Atom*, std::vector<Atom *> > branches, std::multimap<Atom*,std::vector<Atom*> >::iterator branchIter );
};

#endif // IDENTIFICATOR_H
