#ifndef PERCHROMDELETIONFINDER_H
#define PERCHROMDELETIONFINDER_H

#include "DeletionFinder.h"
#include "api/BamReader.h"

class PerChromDeletionFinder
{
public:
    PerChromDeletionFinder(ITargetRegionFinder *pRegionToLeftFinder,
                           ITargetRegionFinder *pRegionToRightFinder,
						   FaidxWrapper *pFaidx);
    void AddRead(AbstractClip *pRead);
    void Clear();
    void FindTargetRegions(std::vector<TargetRegion *> &regions);
    void FindCalls(const CallParams& cParams, std::vector<Deletion *> &variants);
    std::vector<Deletion *> MergeCalls(const std::vector<Deletion *>& variants);

private:
	std::map<int, DeletionFinder*> finderMap;
	std::set<int> conflictSet;
    ITargetRegionFinder *pRegionToLeftFinder;
    ITargetRegionFinder *pRegionToRightFinder;
	FaidxWrapper *pFaidx;
};

typedef std::vector<std::size_t> IndexCluster;

std::vector<IndexCluster> ClusterIntervalsByIndex(const std::vector<Interval> &intervals);

#endif // PERCHROMDELETIONFINDER_H
