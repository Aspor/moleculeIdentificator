#include "identificator.h"
#include<algorithm>
#include <iterator>

#include <QDebug>
//#include<iostream>


using namespace  std;

Identificator::Identificator()
{
    atoms=vector<Atom*>();
    branchMap = multimap<Atom*,vector<Atom*> > ();
    visitedAtoms=vector<bool>();
    cyclePoints=vector<std::array< int,2> >();
    cycleID=0;
}

std::string Identificator::ind(std::vector<Atom *> nAtoms, std::vector<std::vector<int> > bonds){
    atoms=nAtoms;
    cyclePoints.clear();
    branchMap.clear();
    visitedAtoms.resize(atoms.size(),false);
    cycleID=0;

    dfs(0,bonds,true,-1);
    canonize(branchMap.find(nullptr)->second,branchMap,branchMap.find(nullptr));

    string SMILE=generateSMILE  ( branchMap.find(nullptr)->second ,bonds);
    return SMILE;

}

//todo canonize
//TODO cycle bondOrder
std::string Identificator::generateSMILE(vector<Atom*> branch, vector<vector<int> >bonds){

    const string bondSymbols[]={"","","=","#","$"};
    qDebug()<<"branch"<<branch.size();
    string smile="";

    for (int i=0;i<branch.size();i++){
        smile+=branch[i]->SMILEout();
        while(cycleMap.count(branch[i]) !=0){
            multimap<Atom*,int>::iterator cyc=cycleMap.find(branch[i]);

            smile+=bondSymbols[cycleBonds.find(cyc->second)->second];

            //cycle number as character
            smile+='0'+ cyc->second;            
            cycleMap.erase(cyc);
        }

        int thisAtomInd = distance (atoms.begin(), find(atoms.begin(),atoms.end(), branch[i]));
        multimap<Atom*,vector<Atom*> >::iterator branchPoint = branchMap.find(branch[i]);
        while(branchPoint!=branchMap.end()){
            Atom* nextAtom= branchPoint->second[0];

            int nextAtomInd= distance(atoms.begin(), find(atoms.begin(),atoms.end(),nextAtom));

            smile+=bondSymbols[bonds[thisAtomInd][nextAtomInd]];
            smile+="(";
            smile+=generateSMILE(branchPoint->second, bonds);
            smile+=")";
            branchMap.erase(branchPoint);
            branchPoint = branchMap.find(branch[i]);
        }
        int nextAtomInd= distance(atoms.begin(), find(atoms.begin(),atoms.end(),branch[i+1]));
        if(nextAtomInd!=atoms.size())
            smile+=bondSymbols[bonds[thisAtomInd][nextAtomInd]];
    }

    return smile;
}


//TODO canonize
void Identificator::dfs(int atomID,std::vector<std::vector<int> > bonds ,bool newBranch, int parent ){
    std::multimap<Atom*,std::vector<Atom*> >::iterator tmpBranchInd =branchInd;
    if (newBranch){
        if(branchMap.size()==0){
            branchMap.insert(pair<Atom*,vector<Atom* > > (nullptr,vector<Atom*>() ));
            branchInd=branchMap.begin();
        }
        else{
            branchMap.insert(pair<Atom*,vector<Atom* > >(atoms[parent],vector<Atom*>() ));
            branchInd=branchMap.upper_bound(atoms[parent]);
            branchInd--;
        }
    }

    bool branch=false;
    visitedAtoms[atomID]=true;
    (*branchInd).second.push_back(atoms[atomID]);

    bool tmp=false;
    for (int i=0; i<bonds.size(); i++){

        //If there exixts a bond
        if(bonds[atomID][i] !=0){

            //If atom hasn't been visited before
            if(!visitedAtoms[i]){

//                //If there are more than one bond from first atom
//                if(parent==-1 && tmp){
//                    branch=true;
//                }

                dfs(i,bonds,branch,atomID);
                //If there are more than one bond from atom there a branch
                branch=true;
            }

            //If we have visited atom before there a cycle
            else if(parent!=i){
                //int cycleP[2]={atomID,i};


                int countA= cycleMap.count(atoms[atomID]);
                int countI =cycleMap.count(atoms[i]);
                std::pair <std::multimap<Atom*,int>::iterator, std::multimap<Atom*,int>::iterator>
                        rangeA=cycleMap.equal_range(atoms[atomID]);

                std::pair <std::multimap<Atom*,int>::iterator, std::multimap<Atom*,int>::iterator>
                        rangeI=cycleMap.equal_range(atoms[i]);
                bool cycleExits=false;

                for(std::multimap<Atom*,int>::iterator j=rangeA.first;j!=rangeA.second;++j){
                    for(std::multimap<Atom*,int>::iterator k=rangeI.first;k!=rangeI.second;++k){
                        if(j->second==k->second){
                            cycleExits=true;
                            break;
                        }
                    }
                    if(cycleExits)
                        break;
                }
                if(!cycleExits){
                    qDebug()<<"newCyc";
                    cycleMap.emplace(atoms[atomID],cycleID);
                    cycleMap.emplace(atoms[i],cycleID);
                    cycleBonds.emplace(cycleID,bonds[atomID][i]);
                    cycleID++;
                }

//                //if there isn't a cycle with same atoms
//                //Tamahan ei muuten toimi
//                if(!(countA==countI==1 &&  iterI->second==iterP->second)){
//                    qDebug()<<"newCyc";
//                    cycleMap.emplace(atoms[atomID],cycleID);
//                    cycleMap.emplace(atoms[i],cycleID);
//                    cycleBonds.emplace(cycleID,bonds[atomID][i]);
//                    cycleID++;
//                }
            }
            tmp=true;
        }
    }
    //branchInd--;
    //Change to previous branch?
    branchInd=tmpBranchInd;
    return;
}

void Identificator::canonize(vector<Atom*> skeleton, multimap<Atom*,vector< Atom*> >branchMap,multimap<Atom*,vector<Atom* > >::iterator branchIter){
    // TODO  cycles
    bool changed=false;
    for(int i=0;i<skeleton.size();i++){
        std::multimap<Atom*,vector<Atom*> >::iterator branchPoint=branchMap.find(skeleton[i]);
        if(branchPoint!=branchMap.end()){
            if(branchPoint->second.size() > skeleton.size()-i-1){
                vector<Atom*> tmp = branchPoint->second;
                //branch point is skeleton[i] and it will stay in skeleton regatless
                vector<Atom*> tmp2(skeleton.begin()+i+1,skeleton.end());
                branchMap.erase(branchPoint);
                branchMap.insert(pair<Atom*, vector<Atom* > >(skeleton[i],tmp2 ) );
                //branchPoint->second =tmp2;
                //branchMap.insert(branchPoint,tmp2)
                skeleton.erase(skeleton.begin()+i+1,skeleton.end());
                skeleton.insert(skeleton.end(), tmp.begin(),tmp.end());
                //"if it works don't change it"
                branchMap.find(branchIter->first)->second=skeleton;
                //branchInd++;
                changed=true;
            }
        }
    }
    if(changed){
        canonize(skeleton,branchMap,branchIter);
    }
    branchIter++;
    if(branchIter!=this->branchMap.end()){
        canonize(branchIter->second  ,branchMap, branchIter);
    }
    this->branchMap=branchMap;
}
