#ifndef DELETIONFINDER_H
#define DELETIONFINDER_H

#include "targetregionfinder.h"
#include "clip.h"
#include "CallParams.h"
#include "GenomePosition.h"

class DeletionFinder
{
public:
    DeletionFinder(AbstractClip *pRead,
                   ITargetRegionFinder *pRegionFinder);
	void AddRead(AbstractClip *pRead);
	GenomePosition GetClipPosition()
	{ 
		return GenomePosition(reads[0]->getReferenceId(),
			reads[0]->getReferenceName(),
			reads[0]->getClipPosition());
	}
	std::string GetReadType() { return reads[0]->getType(); }
    TargetRegion *FindTargetRegion();
    Deletion *FindVariant(FaidxWrapper *pFaidx, const CallParams& cParams);

private:
	std::vector<AbstractClip *> reads;
	ITargetRegionFinder *pRegionFinder;
};

#endif // DELETIONFINDER_H
