#include <vector>
#include <memory>

#include "shapes.h"

int main()
{
    std::vector<std::shared_ptr<shape>> shapes = {
        std::make_shared<point>(0, 0),
        std::make_shared<rectangle>(0, 0, 0, 0),
        std::make_shared<circle>(0, 0, 0)
    };

    for(auto shpptr1 : shapes)
        for(auto shpptr2 : shapes)
            intersect(*shpptr1, *shpptr2);

    return 0;
}
