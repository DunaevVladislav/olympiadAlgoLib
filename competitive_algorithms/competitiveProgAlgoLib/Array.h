#ifndef COMPETITIVE_ALGORITHMS_ARRAY_H
#define COMPETITIVE_ALGORITHMS_ARRAY_H

#include <functional>
#include <vector>
#include "IMutableDataStructure.h"
#include "ISegmentSolver.h"

namespace cpa {

    template<class TType, class TFunction = std::function<TType(const TType &, const TType &)>>

    class Array :
            public IMutableDataStructure<TType>,
            public ISegmentSolver<TType, TFunction> {
    private:
        std::vector<TType> m_data{};
    public:
        explicit Array(const TFunction &t_function = nullptr);

        template<class TIter>
        Array(TIter t_begin, TIter t_end, const TFunction &t_function = nullptr);

        Array(const Array<TType, TFunction> &other);

        Array(Array<TType, TFunction> &&other) noexcept;

        const TType &getValueAt(std::size_t t_index) const override;

        void changeValueAt(std::size_t t_index, const TType &t_value) override;

        TType valueOnSegment(std::size_t t_left_index, std::size_t t_right_index) const override;
    };

    template<class TType, class TFunction>
    Array<TType, TFunction>::Array(const TFunction &t_function) {
        this->m_function = t_function;
    }

    template<class TType, class TFunction>
    template<class TIter>
    Array<TType, TFunction>::Array(TIter t_begin, TIter t_end, const TFunction &t_function) {
        this->m_function = t_function;
        m_data.assign(t_begin, t_end);
        this->m_size = m_data.size();
    }

    template<class TType, class TFunction>
    Array<TType, TFunction>::Array(const Array<TType, TFunction> &other) {
        this->m_function = other.m_function;
        this->m_data = other.m_data;
        this->m_size = other.m_size;
    }

    template<class TType, class TFunction>
    Array<TType, TFunction>::Array(Array<TType, TFunction> &&other) noexcept {
        this->m_function = std::move(other.m_function);
        this->m_data = std::move(other.m_data);
        this->m_size = std::move(other.m_size);
    }

    template<class TType, class TFunction>
    const TType &Array<TType, TFunction>::getValueAt(std::size_t t_index) const {
        return this->m_data[t_index];
    }

    template<class TType, class TFunction>
    void Array<TType, TFunction>::changeValueAt(std::size_t t_index, const TType &t_value) {
        this->m_data[t_index] = t_value;
    }

    template<class TType, class TFunction>
    TType Array<TType, TFunction>::valueOnSegment(std::size_t t_left_index, std::size_t t_right_index) const {
        TType result = this->m_data[t_left_index];
        for (std::size_t i = t_left_index + 1; i <= t_right_index; ++i) {
            result = this->m_function(result, this->m_data[i]);
        }
        return result;
    }

}

#endif //COMPETITIVE_ALGORITHMS_ARRAY_H
