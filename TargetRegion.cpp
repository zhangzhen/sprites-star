#include "TargetRegion.h"

using namespace std;

TargetRegion::TargetRegion(int referenceId,
                           const string& referenceName,
                           int startPos,
                           int endPos,
                           int fromClipPosition,
                           int numOfPairs,
                           bool heterozygous)
    : cRegion(referenceId, referenceName, startPos, endPos),
      fromClipPosition(fromClipPosition),
      numOfPairs(numOfPairs),
      heterozygous(heterozygous)
{
}

ostream &operator <<(ostream &stream, const TargetRegion &tRegion)
{
    stream << tRegion.cRegion
           << "\t" << tRegion.fromClipPosition
           << "\t" << tRegion.numOfPairs
           << "\t" << tRegion.heterozygous;
    return stream;
}
