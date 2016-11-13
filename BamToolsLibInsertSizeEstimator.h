#ifndef BAMTOOLSLIBINSERTSIZEESTIMATOR_H
#define BAMTOOLSLIBINSERTSIZEESTIMATOR_H

#include "libraryinsertsizeestimator.h"
#include "api/BamReader.h"

class BamToolsLibInsertSizeEstimator : public ILibraryInsertSizeEstimator
{
public:
    BamToolsLibInsertSizeEstimator(BamTools::BamReader *pBamReader);
    void estimate(Library *pLib);

private:
    BamTools::BamReader *pBamReader;
};

#endif // BAMTOOLSLIBINSERTSIZEESTIMATOR_H
