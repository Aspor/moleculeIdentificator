#include "atom.h"
#include <sstream>

Atom::Atom(std::string atomicSymbol, int charge, int id)
{
    this->charge=charge;
    this->atomicSymbol=atomicSymbol;
    this->id=id;
    cycles=std::vector<int>();
}
Atom::Atom(){
    charge=0;
    atomicSymbol="NA";
    id=-1;
    cycles=std::vector<int>();
}
void Atom::addCycle(int cycleId){
    cycles.push_back(cycleId);
}

std::string Atom::Atom::SMILEout(){
    std::string result=atomicSymbol;
    std::ostringstream ostr;
    if(charge!=0){
        ostr<<charge;
        std::string chargeStr=ostr.str();
        result="["+result + chargeStr+"]";
    }
    for(int i=0;i<cycles.size();i++){
        ostr.flush();
        ostr<<cycles[i];
        std::string  cycStr = ostr.str();
        if(cycles[i]>9)
            cycStr="%"+cycStr;
        result+= cycStr;
    }
    cycles.clear();
    return result;
}
std::string Atom::getElement(){
    return atomicSymbol;
}
void Atom::setElement(std::string symblol){
    atomicSymbol=symblol;
    return;
}
