#ifndef COMPETITIVE_ALGORITHMS_EDGE_H
#define COMPETITIVE_ALGORITHMS_EDGE_H

#include "IEdge.h"

namespace cpa {

    class Edge : public IEdge<int> {
    protected:
        void setWeight(const int &t_weight) override {};
    public:
        explicit Edge(std::size_t t_from = 0, std::size_t t_to = 0);

        Edge(const Edge &other);

        [[nodiscard]] int getWeight() const noexcept override;
    };

    Edge::Edge(std::size_t t_from, std::size_t t_to) {
        this->m_from = t_from;
        this->m_to = t_to;
    }

    Edge::Edge(const Edge &other) : IEdge(other) {
        this->m_from = other.m_from;
        this->m_to = other.m_to;
    }


    int Edge::getWeight() const noexcept {
        return 1;
    }

}

#endif //COMPETITIVE_ALGORITHMS_EDGE_H
