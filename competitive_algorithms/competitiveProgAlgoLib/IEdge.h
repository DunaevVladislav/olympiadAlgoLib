#ifndef COMPETITIVE_ALGORITHMS_IEDGE_H
#define COMPETITIVE_ALGORITHMS_IEDGE_H

#include <cstdlib>
#include <algorithm>

namespace cpa {

    template<class TWeight>
    class IEdge {
    protected:
        std::size_t m_from{};
        std::size_t m_to{};
        TWeight m_weight;

        virtual void setWeight(const TWeight &t_weight);

    public:
        IEdge() = default;
        IEdge(const IEdge &other);

        [[nodiscard]] std::size_t from() const noexcept;

        [[nodiscard]] std::size_t to() const noexcept;

        [[nodiscard]] virtual TWeight getWeight() const noexcept;

        void reverse() noexcept;
    };

    template<class TWeight>
    std::size_t IEdge<TWeight>::from() const noexcept {
        return m_from;
    }

    template<class TWeight>
    std::size_t IEdge<TWeight>::to() const noexcept {
        return m_to;
    }

    template<class TWeight>
    TWeight IEdge<TWeight>::getWeight() const noexcept {
        return m_weight;
    }

    template<class TWeight>
    void IEdge<TWeight>::reverse() noexcept {
        std::swap(m_from, m_to);
    }

    template<class TWeight>
    IEdge<TWeight>::IEdge(const IEdge &other) {
        this->m_from = other.m_from;
        this->m_to = other.m_to;
        this->m_weight = other.m_weight;
    }

    template<class TWeight>
    void IEdge<TWeight>::setWeight(const TWeight &t_weight) {
        this->m_weight = t_weight;
    }

}

#endif //COMPETITIVE_ALGORITHMS_IEDGE_H
