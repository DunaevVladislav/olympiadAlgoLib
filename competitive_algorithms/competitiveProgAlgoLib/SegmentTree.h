#ifndef COMPETITIVE_ALGORITHMS_SEGMENTTREE_H
#define COMPETITIVE_ALGORITHMS_SEGMENTTREE_H

#include "IMutableDataStructure.h"
#include "ISegmentSolver.h"

namespace cpa {

    template<class TType, class TFunction = std::function<TType(const TType &, const TType &)>>
    class SegmentTree :
            public IMutableDataStructure<TType>,
            public ISegmentSolver<TType, TFunction> {
    private:
        std::vector<TType> m_nodes;

        template<class TIter>
        void build(TIter t_begin, TIter t_end);

    public:
        explicit SegmentTree(const TFunction &t_function = nullptr);

        template<class TIter>
        SegmentTree(TIter t_begin, TIter t_end, const TFunction &t_function = nullptr);

        SegmentTree(const SegmentTree<TType, TFunction> &other);

        SegmentTree(SegmentTree<TType, TFunction> &&other) noexcept;

        const TType &getValueAt(std::size_t t_index) const override;

        void changeValueAt(std::size_t t_index, const TType &t_value) override;

        TType valueOnSegment(std::size_t t_left_index, std::size_t t_right_index) const override;
    };

    template<class TType, class TFunction>
    SegmentTree<TType, TFunction>::SegmentTree(const TFunction &t_function) {
        this->m_function = t_function;
    }

    template<class TType, class TFunction>
    template<class TIter>
    SegmentTree<TType, TFunction>::SegmentTree(TIter t_begin, TIter t_end, const TFunction &t_function) {
        this->m_function = t_function;
        build(t_begin, t_end);
    }

    template<class TType, class TFunction>
    SegmentTree<TType, TFunction>::SegmentTree(const SegmentTree<TType, TFunction> &other) {
        this->m_function = other.m_function;
        this->m_size = other.m_size;
        this->m_nodes = other.m_nodes;
    }

    template<class TType, class TFunction>
    SegmentTree<TType, TFunction>::SegmentTree(SegmentTree<TType, TFunction> &&other) noexcept {
        this->m_function = std::move(other.m_function);
        this->m_size = std::move(other.m_size);
        this->m_nodes = std::move(other.m_nodes);
    }

    template<class TType, class TFunction>
    template<class TIter>
    void SegmentTree<TType, TFunction>::build(TIter t_begin, TIter t_end) {
        this->m_size = std::distance(t_begin, t_end);
        if (this->m_size == 0) {
            return;
        }
        this->m_nodes.resize(this->m_size * 2);
        std::copy(t_begin, t_end, this->m_nodes.begin() + this->m_size);
        for (auto i = this->m_size - 1; i > 0; --i) {
            this->m_nodes[i] = this->m_function(this->m_nodes[i << 1u], this->m_nodes[(i << 1u) | 1u]);
        }
    }

    template<class TType, class TFunction>
    const TType &SegmentTree<TType, TFunction>::getValueAt(std::size_t t_index) const {
        return this->m_nodes[this->m_size + t_index];
    }

    template<class TType, class TFunction>
    void SegmentTree<TType, TFunction>::changeValueAt(std::size_t t_index, const TType &t_value) {
        t_index += this->m_size;
        for (this->m_nodes[t_index] = t_value; t_index > 1; t_index >>= 1u) {
            this->m_nodes[t_index >> 1u] = this->m_function(this->m_nodes[t_index], this->m_nodes[t_index ^ 1u]);
        }
    }

    template<class TType, class TFunction>
    TType SegmentTree<TType, TFunction>::valueOnSegment(std::size_t t_left_index, std::size_t t_right_index) const {
        t_left_index += this->m_size;
        t_right_index += this->m_size + 1;
        while ((t_left_index & 1u) == 0 && (t_right_index & 1u) == 0) {
            t_left_index >>= 1u;
            t_right_index >>= 1u;
        }
        auto odd_left = (t_left_index & 1u) == 1;
        auto odd_right = (t_right_index & 1u) == 1;
        auto result = odd_left ? this->m_nodes[t_left_index++] : this->m_nodes[--t_right_index];
        if (odd_left && odd_right) {
            result = this->m_function(result, this->m_nodes[--t_right_index]);
        }
        t_left_index >>= 1u;
        t_right_index >>= 1u;
        while (t_left_index < t_right_index) {
            if ((t_left_index & 1u) == 1) {
                result = this->m_function(result, this->m_nodes[t_left_index++]);
            }
            if ((t_right_index & 1u) == 1) {
                result = this->m_function(result, this->m_nodes[--t_right_index]);
            }
            t_left_index >>= 1u;
            t_right_index >>= 1u;
        }
        return result;
    }

}

#endif //COMPETITIVE_ALGORITHMS_SEGMENTTREE_H
