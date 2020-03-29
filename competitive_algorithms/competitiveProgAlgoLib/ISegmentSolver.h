#ifndef COMPETITIVE_ALGORITHMS_ISEGMENTSOLVER_H
#define COMPETITIVE_ALGORITHMS_ISEGMENTSOLVER_H

#include <functional>
#include <vector>

template<class TType, class TFunction = std::function<TType(TType, TType)>>
class ISegmentSolver {
private:
    const TFunction m_function;
public:
    virtual TType valueOnSegment(std::size_t left, std::size_t right) = 0;

    virtual std::vector<TType *> nodesOnSegment(std::size_t left, std::size_t right) = 0;
};

#endif //COMPETITIVE_ALGORITHMS_ISEGMENTSOLVER_H
