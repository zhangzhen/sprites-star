#ifndef MEDIANPOSITIONPICKER_H
#define MEDIANPOSITIONPICKER_H

#include "positionpicker.h"

class MedianPositionPicker : public IPositionPicker
{
public:
    MedianPositionPicker();

    int Pick(std::vector<int> positions);
};

#endif // MEDIANPOSITIONPICKER_H
