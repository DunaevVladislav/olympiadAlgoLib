#ifndef COMPETITIVE_ALGORITHMS_ISEGMENTSOLVER_H
#define COMPETITIVE_ALGORITHMS_ISEGMENTSOLVER_H

#include <functional>
#include <vector>

namespace cpa {

    template<class TType, class TFunction = std::function<TType(const TType&, const TType&)>>
    class ISegmentSolver {
    protected:
        TFunction m_function;
    public:
        virtual TType valueOnSegment(std::size_t t_left_index, std::size_t t_right_index) const = 0;
    };

}

#endif //COMPETITIVE_ALGORITHMS_ISEGMENTSOLVER_H
