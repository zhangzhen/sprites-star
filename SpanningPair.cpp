#include "SpanningPair.h"

using namespace std;

SpanningPair::SpanningPair(int referenceId,
                           const string& referenceName,
                           int insertSize,
                           int leftReadPosition,
                           int rightReadPosition,
                           int readLength)
    : referenceId(referenceId),
      referenceName(referenceName),
      insertSize(insertSize),
      leftReadPosition(leftReadPosition),
      rightReadPosition(rightReadPosition),
      readLength(readLength)
{
}

ostream &operator <<(ostream &stream, const SpanningPair &sPair)
{
    stream << sPair.GetReferenceId()
           << "\t" << sPair.GetLeftReadPosition()
           << "\t" << sPair.GetRightReadPosition()
           << "\t" << sPair.GetInsertSize()
           << "\t" << sPair.GetReadLength();
    return stream;
}
