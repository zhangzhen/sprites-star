#include "CompositeTargetRegionFinder.h"


TargetRegion *CompositeTargetRegionFinder::FindRegion(const GenomePosition &gPos)
{
    std::vector<TargetRegion*> regions;
    for (auto &finder : children)
    {
        TargetRegion *pRegion = finder->FindRegion(gPos);
        if (pRegion)
        {
            regions.push_back(pRegion);
        }
    }

    if (regions.empty()) return NULL;
    return regions.front();
}
