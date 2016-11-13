#include "BamToolsPairsToRightReader.h"
#include "api/BamAlignment.h"
#include "error.h"
#include "globals.h"

using namespace BamTools;
using namespace std;

BamToolsPairsToRightReader::BamToolsPairsToRightReader(Library *pLib, BamTools::BamReader *pBamReader)
    : ISpanningPairsReader(pLib),
      pBamReader(pBamReader)
{
}

void BamToolsPairsToRightReader::Init(const GenomePosition &newGPos)
{
    int start = newGPos.GetPosition();
    int end = start + pLib->GetMaxInsertSize();

    assert(start < end);

    if (!pBamReader->SetRegion(newGPos.GetReferenceId(), start - 1, newGPos.GetReferenceId(), end))
    {
        error("Could not set the region.");
    }

    ISpanningPairsReader::Init(newGPos);
}

SpanningPair* BamToolsPairsToRightReader::NextPair()
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

        if (al.IsReverseStrand() && !al.IsMateReverseStrand() && al.RefID == al.MateRefID
                && al.Position + 1 >= gPos.GetPosition() && al.MatePosition + al.Length < gPos.GetPosition()
                && pLib->InLibrary(rg))
        {
            return new SpanningPair(al.RefID,
                                    pBamReader->GetReferenceData()[al.RefID].RefName,
                                    abs(al.InsertSize),
                                    al.MatePosition + 1,
                                    al.Position + 1,
                                    al.Length);
        }
    }

    return NULL;
}

