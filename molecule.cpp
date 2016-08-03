#include "molecule.h"


Molecule::Molecule()
{

}
Molecule::Molecule(int CSID, std::string MolecularFormula, std::string SMILES, std::string InChI, std::string InChIKey, double AverageMass, double MolecularWeight, double MonoisotopicMass, double NominalMass, double ALogP, double XLogP, std::string CommonName){

}
Molecule::Molecule(std::vector<std::array<std::string,2> > nInfo){
    info=nInfo;
}
std::vector<std::array<std::string,2> > Molecule::getInfo(){
    return info;
}
