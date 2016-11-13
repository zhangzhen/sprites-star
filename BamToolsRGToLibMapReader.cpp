#include "BamToolsRGToLibMapReader.h"
#include "Library.h"
#include "libraryinsertsizeestimator.h"
#include "globals.h"

BamToolsRGToLibMapReader::BamToolsRGToLibMapReader(BamTools::BamReader *pBamReader)
    : pBamReader(pBamReader)
{
}

bool BamToolsRGToLibMapReader::GetMap(std::map<std::string, std::string> &readGroupToLibMap)
{
    BamTools::SamHeader header = pBamReader->GetHeader();
    if (!header.HasReadGroups())
    {
        return false;
    }

    for (auto it = header.ReadGroups.Begin(); it != header.ReadGroups.End(); ++it)
    {
        readGroupToLibMap[it->ID] = it->Library;
    }
    return true;
}
