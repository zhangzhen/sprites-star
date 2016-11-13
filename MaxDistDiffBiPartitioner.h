#ifndef MAXDISTDIFFBIPARTITIONER_H
#define MAXDISTDIFFBIPARTITIONER_H

#include "bipartitioner.h"

class MaxDistDiffBiPartitioner : public IBiPartitioner
{
public:
    MaxDistDiffBiPartitioner();
    std::vector<int> Partition(const std::vector<int> &insertSizes);
};

#endif // MAXDISTDIFFBIPARTITIONER_H
