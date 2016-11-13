#ifndef TARGETREGIONTORIGHTFINDER_H
#define TARGETREGIONTORIGHTFINDER_H

#include "LeafTargetRegionFinder.h"

class TargetRegionToRightFinder : public LeafTargetRegionFinder
{
public:
    TargetRegionToRightFinder(ISpanningPairsReader* pPairsReader,
                              IBiPartitioner* pPartitioner,
                              IBiPartitionQualifier* pQualifier,
                              IPositionPicker* pPosPicker);

protected:
    TargetRegion *GetFinalRegion(const GenomePosition &gPos);
};

#endif // TARGETREGIONTORIGHTFINDER_H
