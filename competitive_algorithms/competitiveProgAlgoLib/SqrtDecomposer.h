#ifndef COMPETITIVE_ALGORITHMS_SQRTDECOMPOSER_H
#define COMPETITIVE_ALGORITHMS_SQRTDECOMPOSER_H

#include <cmath>
#include "ISegmentSolver.h"
#include "IMutableDataStructure.h"

namespace cpa {

    template<class TType, class TFunction = std::function<TType(const TType &, const TType &)>>
    class SqrtDecomposer :
            public IMutableDataStructure<TType>,
            public ISegmentSolver<TType, TFunction> {
    private:
        std::vector<TType> m_data;
        std::vector<TType> m_block_data;
        std::size_t m_block_size;

        template<class TIter>
        void build(TIter t_begin, TIter t_end);

    public:
        explicit SqrtDecomposer(const TFunction &t_function = nullptr);

        template<class TIter>
        SqrtDecomposer(TIter t_begin, TIter t_end, const TFunction &t_function = nullptr);

        SqrtDecomposer(const SqrtDecomposer<TType, TFunction> &other);

        SqrtDecomposer(SqrtDecomposer<TType, TFunction> &&other) noexcept;

        const TType &getValueAt(std::size_t t_index) const override;

        void changeValueAt(std::size_t t_index, const TType &t_value) override;

        TType valueOnSegment(std::size_t t_left_index, std::size_t t_right_index) const override;
    };

    template<class TType, class TFunction>
    SqrtDecomposer<TType, TFunction>::SqrtDecomposer(const TFunction &t_function) {
        this->m_function = t_function;
        this->m_block_size = 0;
    }

    template<class TType, class TFunction>
    template<class TIter>
    SqrtDecomposer<TType, TFunction>::SqrtDecomposer(TIter t_begin, TIter t_end, const TFunction &t_function) {
        this->m_function = t_function;
        this->build(t_begin, t_end);
    }

    template<class TType, class TFunction>
    SqrtDecomposer<TType, TFunction>::SqrtDecomposer(const SqrtDecomposer<TType, TFunction> &other) {
        this->m_function = other.m_function;
        this->m_size = other.m_size;
        this->m_block_size = other.m_block_size;
        this->m_data = other.m_data;
        this->m_block_data = other.m_block_data;
    }

    template<class TType, class TFunction>
    SqrtDecomposer<TType, TFunction>::SqrtDecomposer(SqrtDecomposer<TType, TFunction> &&other) noexcept {
        this->m_function = std::move(other.m_function);
        this->m_size = std::move(other.m_size);
        this->m_block_size = std::move(other.m_block_size);
        this->m_data = std::move(other.m_data);
        this->m_block_data = std::move(other.m_block_data);
    }

    template<class TType, class TFunction>
    template<class TIter>
    void SqrtDecomposer<TType, TFunction>::build(TIter t_begin, TIter t_end) {
        this->m_data.assign(t_begin, t_end);
        this->m_size = this->m_data.size();
        this->m_block_size = static_cast<int>(std::sqrt(this->m_size));
        if (this->m_size == 0) {
            return;
        }
        for (std::size_t i = 0; i < this->m_size; ++i) {
            if (i % this->m_block_size == 0) {
                this->m_block_data.push_back(this->m_data[i]);
            } else {
                this->m_block_data.back() = this->m_function(this->m_block_data.back(), this->m_data[i]);
            }
        }
    }

    template<class TType, class TFunction>
    const TType &SqrtDecomposer<TType, TFunction>::getValueAt(std::size_t t_index) const {
        return this->m_data[t_index];
    }

    template<class TType, class TFunction>
    void SqrtDecomposer<TType, TFunction>::changeValueAt(std::size_t t_index, const TType &t_value) {
        this->m_data[t_index] = t_value;
        auto block = t_index / this->m_block_size;
        this->m_block_data[block] = this->m_data[block * this->m_block_size];
        for (std::size_t i = 1; i < this->m_block_size; ++i) {
            this->m_block_data[block] = this->m_function(this->m_block_data[block],
                                                         this->m_data[block * this->m_block_size + i]);
        }
    }

    template<class TType, class TFunction>
    TType SqrtDecomposer<TType, TFunction>::valueOnSegment(std::size_t t_left_index, std::size_t t_right_index) const {
        auto l_block = t_left_index / this->m_block_size;
        auto r_block = t_right_index / this->m_block_size;
        auto result = this->m_data[t_left_index];
        if (l_block == r_block) {
            for (auto i = t_left_index + 1; i <= t_right_index; ++i) {
                result = this->m_function(result, this->m_data[i]);
            }
        } else {
            for (auto i = t_left_index + 1; i < (l_block + 1) * this->m_block_size; ++i) {
                result = this->m_function(result, this->m_data[i]);
            }
            for (auto i = l_block + 1; i < r_block; ++i) {
                result = this->m_function(result, this->m_block_data[i]);
            }
            for (auto i = r_block * this->m_block_size; i <= t_right_index; ++i) {
                result = this->m_function(result, this->m_data[i]);
            }
        }
        return result;
    }

}

#endif //COMPETITIVE_ALGORITHMS_SQRTDECOMPOSER_H
