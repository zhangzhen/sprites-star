#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <set>

class Library
{
public:
    Library(const std::string &name, const std::string &readGroup);
    std::string GetName() const { return name; }
    void AddReadGroup(const std::string &readGroup) { readGroups.insert(readGroup); }
    int GetMeanInsertSize() const { return meanInsertSize; }
    void SetMeanInsertSize(int newMeanInsertSize) { meanInsertSize = newMeanInsertSize; }
    int GetInsertSizeSd() const { return insertSizeSd; }
    void SetInsertSizeSd(int newInsertSizeSd) { insertSizeSd = newInsertSizeSd; }
    int GetMaxInsertSize() const { return meanInsertSize + 3 * insertSizeSd; }
    bool InLibrary(std::string readGroup)
    {
        return readGroups.count(readGroup);
    }

    friend std::ostream& operator <<(std::ostream& stream, const Library& lib);

private:
    std::string name;
    int meanInsertSize;
    int insertSizeSd;
    std::set<std::string> readGroups;
};

#endif // LIBRARY_H
