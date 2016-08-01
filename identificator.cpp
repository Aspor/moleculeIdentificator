#include "identificator.h"
#include<algorithm>
#include <iterator>
#include <QDebug>
//#include<iostream>


using namespace  std;

Identificator::Identificator()
{
    atoms=vector<Atom*>();
    branches=vector<vector<Atom*> >();
    branchPoints=vector<Atom* >();
    visitedAtoms=vector<bool>();
    cyclePoints=vector<std::array< int,2> >();
    cycleID=0;
}

std::string Identificator::ind(std::vector<Atom *> nAtoms, std::vector<std::vector<int> > bonds){
    atoms=nAtoms;
    branches.clear();
    branchPoints.clear();
    cyclePoints.clear();
    visitedAtoms.resize(atoms.size(),false);
    branchID=0;
    cycleID=0;
    dfs(0,bonds,true,0);
    string SMILE=generateSMILE(branches[0],0,true,bonds);
    return SMILE;

}
std::string Identificator::generateSMILE(vector<Atom*> branch,int branchInd,
                                         bool firstBranch, vector<vector <int> > bonds){
    const string bondSymbols[]={"","","=","#","$"};
    string smile="";
    for (int i=0;i<branch.size();i++){
        smile+=branch[i]->SMILEout();
        int thisAtomInd = distance (atoms.begin(), find(atoms.begin(),atoms.end(), branch[i]));


        std::vector<Atom*>::iterator branchPoint=find(branchPoints.begin(),branchPoints.end(),  branch[i]);
        while(!firstBranch && branchPoint!=branchPoints.end()){
            branchPoints.erase(branchPoint);
            branchInd++;

            int nextAtomInd= distance(atoms.begin(), find(atoms.begin(),atoms.end(),branches[branchInd][0]));
            qDebug()<<thisAtomInd << nextAtomInd << bonds[thisAtomInd][nextAtomInd] <<"BRANCHBOND";
            smile+=bondSymbols[bonds[thisAtomInd][nextAtomInd]];



            smile+="(";
            smile+=generateSMILE(branches[branchInd],branchInd,false,bonds);
            smile+=")";
        }
        int nextAtomInd= distance(atoms.begin(), find(atoms.begin(),atoms.end(),branch[i+1]));
        qDebug()<<thisAtomInd << nextAtomInd << bonds[thisAtomInd][nextAtomInd] <<"BOND";
        if(nextAtomInd!=atoms.size())
            smile+=bondSymbols[bonds[thisAtomInd][nextAtomInd]];
        if( branchPoint!=branchPoints.end())
            firstBranch=false;
    }
    return smile;
}

void Identificator::dfs(int atomID,std::vector<std::vector<int> > bonds ,bool newBranch, int parent){
    if (newBranch){
        qDebug()<<"newBranch";
        std::vector<Atom*> tmpV;
        branches.push_back(tmpV);
        branchPoints.push_back(atoms[branchID]);
    }
    bool b=false;
    visitedAtoms[atomID]=true;
    branches.back().push_back(atoms[atomID]);
    bool tmp=false;
    for (int i=0; i<bonds.size(); i++){
        if(bonds[atomID][i] !=0){
            if(!visitedAtoms[i]){
                dfs(i,bonds,b,atomID);
                if(tmp)
                    b=true;
                branchID=atomID;
            }
            else if(parent!=i){
                //int cycleP[2]={atomID,i};
                bool newCycle=true;
                for(int j=0;j<cyclePoints.size();j++){
                    if(cyclePoints[j][0]==i and cyclePoints[j][1]==atomID)
                        newCycle=false;
                }

                if(newCycle){
                    atoms[atomID]->addCycle(cycleID);
                    atoms[i]->addCycle(cycleID);
                    cyclePoints.push_back({atomID,i});
                    cycleID+=1;
                }

                //cylces.push_back(tmpV);
            }
            tmp =true;
        }
    }
    return;
}
