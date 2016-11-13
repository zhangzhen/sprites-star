#ifndef TARGETREGION_H
#define TARGETREGION_H

#include "ChromosomeRegion.h"

class TargetRegion
{
public:
    TargetRegion(int referenceId,
                 const std::string &referenceName,
                 int startPos,
                 int endPos,
                 int fromClipPosition,
                 int numOfPairs,
                 bool heterozygous=false);
    int GetReferenceId() const { return cRegion.GetReferenceId(); }
    int GetStartPosition() const { return cRegion.GetStartPosition(); }
    int GetEndPosition() const { return cRegion.GetEndPosition(); }
    int GetFromClipPosition() const { return fromClipPosition; }
    int GetNumOfPairs() const { return numOfPairs; }
    int IsHeterozygous() const { return heterozygous; }
    ChromosomeRegion GetChromosomeRegion() const { return cRegion; }

    friend std::ostream &operator <<(std::ostream& stream, const TargetRegion& tRegion);

private:
    ChromosomeRegion cRegion;
    int fromClipPosition;
    int numOfPairs;
    bool heterozygous;
};

#endif // TARGETREGION_H
