#ifndef TARGETREGIONTOLEFTFINDER_H
#define TARGETREGIONTOLEFTFINDER_H

#include "LeafTargetRegionFinder.h"

class TargetRegionToLeftFinder : public LeafTargetRegionFinder
{
public:
    TargetRegionToLeftFinder(ISpanningPairsReader* pPairsReader,
                             IBiPartitioner* pPartitioner,
                             IBiPartitionQualifier* pQualifier,
                             IPositionPicker* pPosPicker);

protected:
    TargetRegion *GetFinalRegion(const GenomePosition &gPos);
};

#endif // TARGETREGIONTOLEFTFINDER_H
