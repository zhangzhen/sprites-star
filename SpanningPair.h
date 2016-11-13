#ifndef SPANNINGPAIR_H
#define SPANNINGPAIR_H

#include <string>
#include <iostream>

class SpanningPair
{
public:
    SpanningPair(int referenceId,
                 const std::string &referenceName,
                 int insertSize,
                 int leftReadPosition,
                 int rightReadPosition,
                 int readLength);
    int GetReferenceId() const { return referenceId; }
    std::string GetReferenceName() const { return referenceName; }
    int GetInsertSize() const { return insertSize; }
    int GetLeftReadPosition() const { return leftReadPosition; }
    int GetRightReadPosition() const { return rightReadPosition; }
    int GetReadLength() const { return readLength; }

    friend std::ostream &operator <<(std::ostream& stream, const SpanningPair& sPair);

private:
    int referenceId;
    std::string referenceName;
    int insertSize;
    int leftReadPosition;
    int rightReadPosition;
    int readLength;
};

#endif // SPANNINGPAIR_H
