#ifndef COMPETITIVE_ALGORITHMS_IMUTABLEDATASTRUCTURE_H
#define COMPETITIVE_ALGORITHMS_IMUTABLEDATASTRUCTURE_H

#include "IDataStructure.h"

namespace cpa {

    template<class TType>
    class IMutableDataStructure : public IDataStructure<TType> {
    public:
        virtual void changeValueAt(std::size_t t_index, const TType& t_value) = 0;
    };

}

#endif //COMPETITIVE_ALGORITHMS_IMUTABLEDATASTRUCTURE_H
