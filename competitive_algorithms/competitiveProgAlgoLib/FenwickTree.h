#ifndef COMPETITIVE_ALGORITHMS_FENWICKTREE_H
#define COMPETITIVE_ALGORITHMS_FENWICKTREE_H

#include "IMutableDataStructure.h"
#include "ISegmentSolver.h"

namespace cpa {

    template<class TType, class TFunction = std::function<TType(const TType &, const TType &)>>
    class FenwickTree :
            public IMutableDataStructure<TType>,
            public ISegmentSolver<TType, TFunction> {
    private:
        std::vector<TType> m_tree;
        std::vector<TType> m_counter_tree;

        template<class TIter>
        void build(TIter t_begin, TIter t_end);

        [[nodiscard]] int f(unsigned int x) const noexcept;

        [[nodiscard]] int g(unsigned int x) const noexcept;

    public:
        const TType &getValueAt(std::size_t t_index) const override;

        void changeValueAt(std::size_t t_index, const TType &t_value) override;

        TType valueOnSegment(std::size_t t_left_index, std::size_t t_right_index) const override;

        explicit FenwickTree(const TFunction &t_function = nullptr);

        template<class TIter>
        FenwickTree(TIter t_begin, TIter t_end, const TFunction &t_function = nullptr);

        FenwickTree(const FenwickTree<TType, TFunction> &other);

        FenwickTree(FenwickTree<TType, TFunction> &&other) noexcept;

    };

    template<class TType, class TFunction>
    FenwickTree<TType, TFunction>::FenwickTree(const TFunction &t_function) {
        this->m_function = t_function;
    }

    template<class TType, class TFunction>
    template<class TIter>
    FenwickTree<TType, TFunction>::FenwickTree(TIter t_begin, TIter t_end, const TFunction &t_function) {
        this->m_function = t_function;
        build(t_begin, t_end);
    }

    template<class TType, class TFunction>
    FenwickTree<TType, TFunction>::FenwickTree(const FenwickTree<TType, TFunction> &other) {
        this->m_function = other.m_function;
        this->m_size = other.m_size;
        this->m_tree = other.m_tree;
        this->m_counter_tree = other.m_counter_tree;
    }

    template<class TType, class TFunction>
    FenwickTree<TType, TFunction>::FenwickTree(FenwickTree<TType, TFunction> &&other) noexcept {
        this->m_function = std::move(other.m_function);
        this->m_size = std::move(other.m_size);
        this->m_tree = std::move(other.m_tree);
        this->m_counter_tree = std::move(other.m_counter_tree);
    }

    template<class TType, class TFunction>
    template<class TIter>
    void FenwickTree<TType, TFunction>::build(TIter t_begin, TIter t_end) {
        this->m_tree.assign(t_begin, t_end);
        this->m_counter_tree.assign(std::next(this->m_tree.begin()), this->m_tree.end());
        this->m_size = this->m_tree.size();
        for (std::size_t i = 1; (1u << i) <= this->m_size; ++i) {
            for (std::size_t j = (1u << i) - 1; j < this->m_size; j += 1u << (i + 1)) {
                for (std::size_t k = 0; k < i; ++k) {
                    this->m_tree[j] = this->m_function(this->m_tree[j], this->m_tree[j ^ (1u << k)]);
                }
            }
        }
        for (std::size_t i = 1; (1u << i) < this->m_size; ++i) {
            for (std::size_t j = (1u << i) - 1; j < this->m_size; j += 1u << (i + 1)) {
                for (std::size_t k = 0; k < i && j + (1u << k) < this->m_counter_tree.size(); ++k) {
                    this->m_counter_tree[j] = this->m_function(this->m_counter_tree[j],
                                                               this->m_counter_tree[j + (1u << k)]);
                }
            }
        }
    }

    template<class TType, class TFunction>
    const TType &FenwickTree<TType, TFunction>::getValueAt(std::size_t t_index) const {
        return (t_index & 1u) == 1 ? this->m_counter_tree[t_index ^ 1u] : this->m_tree[t_index];
    }

    template<class TType, class TFunction>
    void FenwickTree<TType, TFunction>::changeValueAt(std::size_t t_index, const TType &t_value) {
        for (auto i = t_index; i < this->m_size; i = g(i)) {
            this->m_tree[i] = this->m_function(this->m_tree[i], t_value);
        }
        for (int i = static_cast<int>(t_index) - 1; i >= 0; i = f(i)) {
            this->m_counter_tree[i] = this->m_function(this->m_counter_tree[i], t_value);
        }
    }

    template<class TType, class TFunction>
    TType FenwickTree<TType, TFunction>::valueOnSegment(std::size_t t_left_index, std::size_t t_right_index) const {
        int l = static_cast<int>(t_left_index) - 1;
        int r = t_right_index;
        bool r_first = f(r) >= l;
        auto res = r_first ? this->m_tree[r] : this->m_counter_tree[l];
        if (r_first) {
            r = f(r);
        } else {
            l = g(l);
        }
        while (l < r) {
            if (f(r) >= l) {
                res = this->m_function(res, this->m_tree[r]);
                r = f(r);
            } else {
                res = this->m_function(res, this->m_counter_tree[l]);
                l = g(l);
            }
        }
        return res;
    }

    template<class TType, class TFunction>
    int FenwickTree<TType, TFunction>::f(unsigned int x) const noexcept {
        return static_cast<int>(x & (x + 1)) - 1;
    }

    template<class TType, class TFunction>
    int FenwickTree<TType, TFunction>::g(unsigned int x) const noexcept {
        return static_cast<int>(x | (x + 1));
    }
}

#endif //COMPETITIVE_ALGORITHMS_FENWICKTREE_H
