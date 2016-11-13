#ifndef BAMTOOLSPAIRSTOLEFTREADER_H
#define BAMTOOLSPAIRSTOLEFTREADER_H

#include "spanningpairsreader.h"
#include "api/BamReader.h"

class BamToolsPairsToLeftReader : public ISpanningPairsReader
{
public:
    BamToolsPairsToLeftReader(Library *pLib, BamTools::BamReader *pBamReader);
    void Init(const GenomePosition &newGPos);
    SpanningPair *NextPair();

private:
    BamTools::BamReader *pBamReader;
};

#endif // BAMTOOLSPAIRSTOLEFTREADER_H
