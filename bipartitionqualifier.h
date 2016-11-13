#ifndef BIPARTITIONQUALIFIER_H
#define BIPARTITIONQUALIFIER_H

#include "SpanningPair.h"
#include <vector>

class IBiPartitionQualifier
{
public:
    virtual bool IsQualified(const std::vector<int> &insertSizes, const std::vector<int> &labels) = 0;
};

#endif // BIPARTITIONQUALIFIER_H
