#include "IAccStructure.h"
#include "Brute.h"
#include "BVH.h"
//#include "KDTree.h"

namespace tracer
{
    std::shared_ptr<IAccStructure> IAccStructure::CreateAccelerator(const std::string& name)
    {
        if (name == "Brute")
        {
            return std::make_shared<Brute>();
        }
        else if (name == "BVH")
        {
            return std::make_shared<BVH>();
        }
        //else if (name == "KDTree")
        //{
        //    return std::make_shared<KDTree>();
        //}
        else
        {
            throw std::exception("No such accelerator!");
        }
    }
}