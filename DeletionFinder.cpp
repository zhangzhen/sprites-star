#include "DeletionFinder.h"
#include "Deletion.h"

#include <algorithm>


DeletionFinder::DeletionFinder(AbstractClip *pRead,
                               ITargetRegionFinder *pRegionFinder)
    : reads({ pRead }),
      pRegionFinder(pRegionFinder)
{
}

void DeletionFinder::AddRead(AbstractClip *pRead)
{
	if (pRead->getType() == reads[0]->getType()
		&& pRead->getClipPosition() == reads[0]->getClipPosition())
	{
		reads.push_back(pRead);
	}
}

TargetRegion *DeletionFinder::FindTargetRegion()
{
    return pRegionFinder->FindRegion(GetClipPosition());
}


Deletion *DeletionFinder::FindVariant(FaidxWrapper *pFaidx, const CallParams &cParams)
{
    TargetRegion *pTargetReg;
    if((pTargetReg = FindTargetRegion()))
    {
		return reads[0]->call(pTargetReg, *pFaidx, cParams.GetMinClip(), 1-cParams.GetMaxErrorRate());
    }
    return NULL;
}
