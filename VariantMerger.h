#ifndef VARIANTMERGER
#define VARIANTMERGER

#include "Deletion.h"

#include <vector>


class VariantMerger
{
public:
    VariantMerger(Deletion *pVariant)
        : variants({pVariant})
    {}

    void AddVariant(Deletion *pVariant);

	Deletion *merge();

private:
    std::vector<Deletion *> variants;
};

#endif // VARIANTMERGER

