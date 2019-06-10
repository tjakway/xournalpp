#include <utility>

int getPairFirst(std::pair<int, int> p)
{
    return std::get<0>(p);
}

int getPairSecond(std::pair<int, int> p)
{
    return std::get<1>(p);
}
