#ifndef LEAFTARGETREGIONFINDER_H
#define LEAFTARGETREGIONFINDER_H

#include "targetregionfinder.h"

class LeafTargetRegionFinder : public ITargetRegionFinder
{
public:
    LeafTargetRegionFinder(ISpanningPairsReader* pPairsReader,
                           IBiPartitioner* pPartitioner,
                           IBiPartitionQualifier* pQualifier,
                           IPositionPicker* pPosPicker);
    TargetRegion* FindRegion(const GenomePosition &gPos);

protected:
    void Clear()
    {
        pairs.clear();
        labels.clear();
        heterozygous = false;
    }

    virtual TargetRegion* GetFinalRegion(const GenomePosition &gPos) = 0;

    std::vector<SpanningPair*> pairs;
    std::vector<int> labels;
    ISpanningPairsReader* pPairsReader;
    IBiPartitioner* pPartitioner;
    IBiPartitionQualifier* pQualifier;
    IPositionPicker* pPosPicker;
    bool heterozygous;

};

#endif // LEAFTARGETREGIONFINDER_H
