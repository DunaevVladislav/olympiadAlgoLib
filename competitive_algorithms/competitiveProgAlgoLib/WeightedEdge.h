#ifndef COMPETITIVE_ALGORITHMS_WEIGHTEDEDGE_H
#define COMPETITIVE_ALGORITHMS_WEIGHTEDEDGE_H

#include <algorithm>
#include "IEdge.h"

namespace cpa {

    template<class TWeight>
    class WeightedEdge : public IEdge<TWeight> {
    public:
        explicit WeightedEdge(std::size_t t_from, std::size_t t_to, const TWeight &t_weight = TWeight());

        WeightedEdge(const WeightedEdge<TWeight> &other);

        WeightedEdge(WeightedEdge<TWeight> &&other) noexcept;

        void setWeight(const TWeight &t_weight) override;
    };

    template<class TWeight>
    WeightedEdge<TWeight>::WeightedEdge(std::size_t t_from, std::size_t t_to, const TWeight &t_weight) {
        this->m_from = t_from;
        this->m_to = t_to;
        this->m_weight = t_weight;
    }

    template<class TWeight>
    WeightedEdge<TWeight>::WeightedEdge(const WeightedEdge<TWeight> &other) {
        this->m_from = other.m_from;
        this->m_to = other.m_to;
        this->m_weight = other.m_weight;
    }

    template<class TWeight>
    WeightedEdge<TWeight>::WeightedEdge(WeightedEdge<TWeight> &&other) noexcept {
        this->m_from = other.m_from;
        this->m_to = other.m_to;
        this->m_weight = std::move(other.m_weight);
    }

    template<class TWeight>
    void WeightedEdge<TWeight>::setWeight(const TWeight &t_weight) {
        this->m_weight = t_weight;
    }

}

#endif //COMPETITIVE_ALGORITHMS_WEIGHTEDEDGE_H
