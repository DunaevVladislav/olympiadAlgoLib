#ifndef COMPETITIVE_ALGORITHMS_ISPARSETABLE_H
#define COMPETITIVE_ALGORITHMS_ISPARSETABLE_H

#include <vector>
#include "ISegmentSolver.h"
#include "IDataStructure.h"

namespace cpa {

    template<class TType, class TFunction = std::function<TType(const TType &, const TType &)>>
    class SparseTable :
            public IDataStructure<TType>,
            public ISegmentSolver<TType, TFunction>{
    private:

    };

}

#endif //COMPETITIVE_ALGORITHMS_ISPARSETABLE_H
