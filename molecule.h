#ifndef MOLECULE_H
#define MOLECULE_H

#include "atom.h"
#include <vector>
#include <algorithm>

class molecule
{
public:
    molecule();
private:
    std::vector<Atom*> atoms;
};

#endif // MOLECULE_H
