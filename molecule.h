#ifndef MOLECULE_H
#define MOLECULE_H

#include "atom.h"
#include <vector>
#include <algorithm>
#include <map>

class Molecule
{
public:
    Molecule();
    Molecule(int CSID,std::string MolecularFormula,std::string SMILES,std::string InChI,std::string InChIKey,double AverageMass,double MolecularWeight,double MonoisotopicMass,double NominalMass,double ALogP,double XLogP,std::string CommonName);
    Molecule(std::vector<std::pair<std::string,std::string> >);
    std::vector<std::pair<std::string,std::string> > getInfo();

private:
    std::vector<Atom*> atoms;
    std::vector<std::pair<std::string,std::string> > info;
};

#endif // MOLECULE_H
