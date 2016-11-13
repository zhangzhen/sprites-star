#ifndef TARGETREGIONFINDER_H
#define TARGETREGIONFINDER_H

#include "TargetRegion.h"
#include "SpanningPair.h"
#include "spanningpairsreader.h"
#include "bipartitioner.h"
#include "bipartitionqualifier.h"
#include "positionpicker.h"

class ITargetRegionFinder
{

public:
    virtual ~ITargetRegionFinder() {}
    virtual TargetRegion* FindRegion(const GenomePosition &gPos) = 0;

};

#endif // TARGETREGIONFINDER_H
