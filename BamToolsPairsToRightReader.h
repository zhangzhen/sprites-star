#ifndef BAMTOOLSPAIRSTORIGHTREADER_H
#define BAMTOOLSPAIRSTORIGHTREADER_H

#include "spanningpairsreader.h"
#include "api/BamReader.h"

class BamToolsPairsToRightReader : public ISpanningPairsReader
{
public:
    BamToolsPairsToRightReader(Library *pLib, BamTools::BamReader *pBamReader);
    void Init(const GenomePosition &newGPos);
    SpanningPair *NextPair();

private:
    BamTools::BamReader *pBamReader;
};

#endif // BAMTOOLSPAIRSTORIGHTREADER_H
