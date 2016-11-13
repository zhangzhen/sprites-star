#ifndef COMPOSITETARGETREGIONFINDER_H
#define COMPOSITETARGETREGIONFINDER_H

#include "targetregionfinder.h"

class CompositeTargetRegionFinder : public ITargetRegionFinder
{
public:
    CompositeTargetRegionFinder() {}
    void Add(ITargetRegionFinder *pRegionFinder)
    {
        children.push_back(pRegionFinder);
    }
    TargetRegion *FindRegion(const GenomePosition &gPos);

private:
    std::vector<ITargetRegionFinder*> children;
};

#endif // COMPOSITETARGETREGIONFINDER_H
