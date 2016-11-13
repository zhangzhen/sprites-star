#ifndef BIPARTITIONER_H
#define BIPARTITIONER_H

#include <vector>

class IBiPartitioner
{
public:
    virtual ~IBiPartitioner() {}
    virtual std::vector<int> Partition(const std::vector<int> &insertSizes) = 0;
};

#endif // BIPARTITIONER_H
