#ifndef READGROUPTOLIBRARYMAPREADER_H
#define READGROUPTOLIBRARYMAPREADER_H

#include <map>
#include <string>

class IReadGroupToLibraryMapReader
{
public:
    virtual bool GetMap(std::map<std::string, std::string> & readGroupToLibMap) = 0;
};

#endif // READGROUPTOLIBRARYMAPREADER_H
