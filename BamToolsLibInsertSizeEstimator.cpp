#include "BamToolsLibInsertSizeEstimator.h"
#include "globals.h"
#include "statistics.h"

#include <algorithm>
#include <iterator>

using namespace std;
using namespace BamTools;

BamToolsLibInsertSizeEstimator::BamToolsLibInsertSizeEstimator(BamTools::BamReader *pBamReader)
    : pBamReader(pBamReader)
{
}


void BamToolsLibInsertSizeEstimator::estimate(Library *pLib)
{
    BamAlignment al;
    size_t cnt = 0;
    vector<int> insertSizes;

    while (pBamReader->GetNextAlignment(al) && cnt < 10000)
    {
        string rg;
        if (!al.GetTag("RG", rg))
        {
            rg = NORGTAGREADGROUPNAME;
        }

        if (al.IsProperPair() && al.MatePosition > al.Position
            && pLib->InLibrary(rg) && al.InsertSize < 10000)
        {
            insertSizes.push_back(al.InsertSize);
            cnt++;
        }
    }

//    cout << insertSizes.size() << endl;
//    copy(insertSizes.begin(), insertSizes.end(), ostream_iterator<int>(cout, ", "));

    double meanInsertSize = GetMean(begin(insertSizes), end(insertSizes));
    double insertSizeSd = GetStandardDeviation(begin(insertSizes), end(insertSizes));

    pLib->SetMeanInsertSize((int)round(meanInsertSize));
    pLib->SetInsertSizeSd((int)round(insertSizeSd));
}
