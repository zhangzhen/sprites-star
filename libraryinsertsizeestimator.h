#ifndef LIBRARYINSERTSIZEESTIMATOR_H
#define LIBRARYINSERTSIZEESTIMATOR_H

#include "Library.h"

class ILibraryInsertSizeEstimator
{
public:
    virtual void estimate(Library* pLib) = 0;
};

#endif // LIBRARYINSERTSIZEESTIMATOR_H
