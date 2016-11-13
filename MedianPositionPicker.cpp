#include "MedianPositionPicker.h"
#include <algorithm>

using namespace std;

MedianPositionPicker::MedianPositionPicker()
{
}


int MedianPositionPicker::Pick(std::vector<int> positions)
{
    nth_element(begin(positions), begin(positions) + positions.size()/2, end(positions));
    return positions[positions.size()/2];
}
