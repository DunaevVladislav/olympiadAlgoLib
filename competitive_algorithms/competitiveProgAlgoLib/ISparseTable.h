#ifndef COMPETITIVE_ALGORITHMS_ISPARSETABLE_H
#define COMPETITIVE_ALGORITHMS_ISPARSETABLE_H

#include <vector>
#include "ISegmentSolver.h"
#include "IDataStructure.h"

namespace cpa {

    template<class TType, class TFunction = std::function<TType(const TType &, const TType &)>>
    class SparseTable :
            public IDataStructure<TType>,
            public ISegmentSolver<TType, TFunction> {
    private:
        std::vector<std::size_t> m_log;
        std::vector<std::vector<TType>> m_st;

        void calculateLog();

        template<class TIter>
        void calculateSparseTable(TIter t_begin, TIter t_end);

    public:
        explicit SparseTable(const TFunction &t_function = nullptr);

        template<class TIter>
        SparseTable(TIter t_begin, TIter t_end, const TFunction &t_function = nullptr);

        SparseTable(const SparseTable<TType, TFunction> &other);

        SparseTable(SparseTable<TType, TFunction> &&other) noexcept;

        const TType &getValueAt(std::size_t t_index) const override;

        TType valueOnSegment(std::size_t t_left_index, std::size_t t_right_index) const override;
    };


    template<class TType, class TFunction>
    SparseTable<TType, TFunction>::SparseTable(const TFunction &t_function) {
        this->m_function = t_function;
    }

    template<class TType, class TFunction>
    template<class TIter>
    SparseTable<TType, TFunction>::SparseTable(TIter t_begin, TIter t_end, const TFunction &t_function) {
        this->m_function = t_function;
        this->m_size = std::distance(t_begin, t_end);
        this->calculateLog();
        this->calculateSparseTable(t_begin, t_end);
    }

    template<class TType, class TFunction>
    void SparseTable<TType, TFunction>::calculateLog() {
        this->m_log.resize(this->m_size);
        this->m_log[0] = 0;
        this->m_log[1] = 0;
        for (std::size_t i = 2; i < this->m_log.size(); ++i) {
            this->m_log[i] = this->m_log[i / 2] + 1;
        }
    }

    template<class TType, class TFunction>
    template<class TIter>
    void SparseTable<TType, TFunction>::calculateSparseTable(TIter t_begin, TIter t_end) {
        this->m_st.resize(this->m_log.back() + 1);
        this->m_st[0].assign(t_begin, t_end);
        for (std::size_t i = 1; i < this->m_st.size(); ++i) {
            this->m_st[i].resize(this->m_size - (1u << i) + 1);
            for (std::size_t j = 0; j < this->m_st[i].size(); ++j) {
                this->m_st[i][j] = this->m_function(this->m_st[i - 1][j], this->m_st[i - 1][j + (1u << (i - 1))]);
            }
        }
    }

    template<class TType, class TFunction>
    SparseTable<TType, TFunction>::SparseTable(const SparseTable<TType, TFunction> &other) {
        this->m_function = other.m_function;
        this->m_size = other.m_size;
        this->m_log = other.m_log;
        this->m_st = other.m_st;
    }

    template<class TType, class TFunction>
    SparseTable<TType, TFunction>::SparseTable(SparseTable<TType, TFunction> &&other) noexcept {
        this->m_function = std::move(other.m_function);
        this->m_size = std::move(other.m_size);
        this->m_log = std::move(other.m_log);
        this->m_st = std::move(other.m_st);
    }

    template<class TType, class TFunction>
    const TType &SparseTable<TType, TFunction>::getValueAt(std::size_t t_index) const {
        return this->m_st[0][t_index];
    }

    template<class TType, class TFunction>
    TType SparseTable<TType, TFunction>::valueOnSegment(std::size_t t_left_index, std::size_t t_right_index) const {
        auto i = this->m_log[t_right_index - t_left_index];
        return this->m_function(this->m_st[i][t_left_index], this->m_st[i][t_right_index - (1u << i) + 1]);
    }

}

#endif //COMPETITIVE_ALGORITHMS_ISPARSETABLE_H
