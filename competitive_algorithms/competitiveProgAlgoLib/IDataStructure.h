#ifndef COMPETITIVE_ALGORITHMS_IDATASTRUCTURE_H
#define COMPETITIVE_ALGORITHMS_IDATASTRUCTURE_H

#include <cstdlib>

namespace cpa {

    template<class TType>
    class IDataStructure {
    protected:
        std::size_t m_size{};
    public:
        [[nodiscard]]
        std::size_t size() const;

        virtual const TType& getValueAt(std::size_t t_index) const = 0;
    };

    template<class TType>
    std::size_t IDataStructure<TType>::size() const {
        return this->m_size;
    }

}

#endif //COMPETITIVE_ALGORITHMS_IDATASTRUCTURE_H
