#ifndef SPANNINGPAIRSREADER_H
#define SPANNINGPAIRSREADER_H

#include "GenomePosition.h"
#include "SpanningPair.h"
#include "Library.h"

class ISpanningPairsReader
{
public:
    ISpanningPairsReader(Library *pLib) : pLib(pLib) {}
    int GetMeanInsertSize() const { return pLib->GetMeanInsertSize(); }
    int GetMaxInsertSize() const { return pLib->GetMaxInsertSize(); }

    virtual ~ISpanningPairsReader() {}
    virtual void Init(const GenomePosition &newGPos) { gPos = newGPos; }
    virtual SpanningPair *NextPair() = 0;

protected:
    Library *pLib;
    GenomePosition gPos;
};

#endif // SPANNINGPAIRSREADER_H
