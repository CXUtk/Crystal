#include "Medium.h"

namespace crystal
{
    static MediumInterface nullInterface = MediumInterface(nullptr);

    const MediumInterface* MediumInterface::GetNullMedium()
    {
        return &nullInterface;
    }
}
