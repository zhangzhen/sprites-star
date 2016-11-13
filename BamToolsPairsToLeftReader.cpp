#include "BamToolsPairsToLeftReader.h"
#include "api/BamAlignment.h"
#include "error.h"
#include "globals.h"

using namespace std;
using namespace BamTools;

BamToolsPairsToLeftReader::BamToolsPairsToLeftReader(Library *pLib, BamTools::BamReader *pBamReader)
    : ISpanningPairsReader(pLib),
      pBamReader(pBamReader)
{
}

void BamToolsPairsToLeftReader::Init(const GenomePosition &newGPos)
{
    int end = newGPos.GetPosition();
    int start = end - pLib->GetMaxInsertSize();

    assert(start > 0);

    if (!pBamReader->SetRegion(newGPos.GetReferenceId(), start - 1, newGPos.GetReferenceId(), end))
    {
        error("Could not set the region.");
    }
    ISpanningPairsReader::Init(newGPos);
}

SpanningPair *BamToolsPairsToLeftReader::NextPair()
{
    BamAlignment al;
    while (pBamReader->GetNextAlignment(al))
    {
        string rg;
        if (!al.GetTag("RG", rg))
        {
            rg = NORGTAGREADGROUPNAME;
        }

        string xt;
        if (al.GetTag("XT", xt))
        {
            xt = xt.substr(0,1);
            if (xt == "R") {
                continue;
            }
        }

        if (!al.IsReverseStrand() && al.IsMateReverseStrand() && al.RefID == al.MateRefID
                && al.GetEndPosition() <= gPos.GetPosition() && al.MatePosition + 1 > gPos.GetPosition()
                && pLib->InLibrary(rg))
        {
            return new SpanningPair(al.RefID,
                                    pBamReader->GetReferenceData()[al.RefID].RefName,
                                    abs(al.InsertSize),
                                    al.Position + 1,
                                    al.MatePosition + 1,
                                    al.Length);
        }
    }

    return NULL;

}
