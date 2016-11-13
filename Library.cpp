#include "Library.h"
#include "globals.h"


Library::Library(const std::string &name, const std::string &readGroup)
    : name(name),
      meanInsertSize(-1),
      insertSizeSd(-1),
      readGroups({readGroup})
{
}

std::ostream &operator <<(std::ostream &stream, const Library &lib)
{
    stream << lib.name << "\t"
              << lib.meanInsertSize << "\t"
                 << lib.insertSizeSd << "\t"
                    << join(std::begin(lib.readGroups), std::end(lib.readGroups), ";");
    return stream;
}
