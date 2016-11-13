#ifndef ANOVABIPARTITIONQUALIFIER_H
#define ANOVABIPARTITIONQUALIFIER_H

#include "bipartitionqualifier.h"

class AnovaBiPartitionQualifier : public IBiPartitionQualifier
{
public:
    AnovaBiPartitionQualifier(double threshold);

    bool IsQualified(const std::vector<int> &insertSizes, const std::vector<int> &labels);

private:
    double threshold;
};

#endif // ANOVABIPARTITIONQUALIFIER_H
