#include "MaxDistDiffBiPartitioner.h"

#include <numeric>
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;

MaxDistDiffBiPartitioner::MaxDistDiffBiPartitioner()
{
}

std::vector<int> MaxDistDiffBiPartitioner::Partition(const std::vector<int> &insertSizes)
{
    if (insertSizes.size() == 1)
    {
        return {0};
    }

    vector<int> indices(insertSizes.size());
    iota(begin(indices), end(indices), 0);
    sort(begin(indices), end(indices),
              [&](int x, int y) { return insertSizes[x] < insertSizes[y]; });

    vector<int> result(insertSizes.size() - 1);
    for (size_t i = 1; i < insertSizes.size(); i++)
    {
        result[i-1] = insertSizes[indices[i]] - insertSizes[indices[i-1]];
    }

    int indexOfIndices = distance(begin(result), max_element(begin(result), end(result))) + 1;

    vector<int> labels(insertSizes.size());
    for (size_t i = 0; i < insertSizes.size(); i++)
    {
        labels[indices[i]] = i < indexOfIndices ? 0 : 1;
    }

    return labels;
}
