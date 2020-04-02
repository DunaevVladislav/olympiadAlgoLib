#ifndef COMPETITIVE_ALGORITHMS_TREAP_H
#define COMPETITIVE_ALGORITHMS_TREAP_H

#include <random>
#include <chrono>
#include <memory>
#include <stack>
#include "IMutableDataStructure.h"
#include "ISegmentSolver.h"

namespace cpa {

    template<class TType, class TFunction = std::function<TType(const TType &, const TType &)>>
    class Treap :
            public IMutableDataStructure<TType>,
            public ISegmentSolver<TType, TFunction> {
    private:
        std::mt19937 m_rnd;
        TFunction m_function;

        struct node {
            std::size_t size;
            unsigned int prior;
            TType value;
            TType segmentValue;
            std::unique_ptr<node> left, right;

            explicit node(const TType &value, unsigned int prior) :
                    size(1),
                    prior(prior),
                    value(value),
                    segmentValue(value),
                    left(nullptr),
                    right(nullptr) {}

            node(const node &other) :
                    size(other.size),
                    prior(other.prior),
                    value(other.value),
                    segmentValue(other.segmentValue),
                    left(other->left == nullptr ? nullptr : std::make_unique<node>(*other->left)),
                    right(other->right == nullptr ? nullptr : std::make_unique<node>(*other->right)) {}

            void update(const TFunction &t_function);
        };

        void initial_random();

        template<class TIter>
        void build(TIter t_begin, TIter t_end);

        const TType &getValueAt(const std::unique_ptr<node> &t_current, std::size_t t_index) const;

        void changeValueAt(std::unique_ptr<node> &t_current, std::size_t t_index, const TType &t_value);

        TType
        valueOnSegment(const std::unique_ptr<node> &t_current, std::size_t t_left_index,
                       std::size_t t_right_index) const;

    public:
        std::unique_ptr<node> m_root = nullptr;

        explicit Treap(const TFunction &t_function = nullptr);

        template<class TIter>
        Treap(TIter t_begin, TIter t_end, const TFunction &t_function = nullptr);

        Treap(const Treap<TType, TFunction> &other);

        Treap(Treap<TType, TFunction> &&other) noexcept;

        const TType &getValueAt(std::size_t t_index) const override;

        void changeValueAt(std::size_t t_index, const TType &t_value) override;

        TType valueOnSegment(std::size_t t_left_index, std::size_t t_right_index) const override;
    };

    template<class TType, class TFunction>
    void Treap<TType, TFunction>::node::update(const TFunction &t_function) {
        this->size = 1;
        this->segmentValue = this->value;
        if (this->left != nullptr) {
            this->size += this->left->size;
            this->segmentValue = t_function(this->segmentValue, this->left->segmentValue);
        }
        if (this->right != nullptr) {
            this->size += this->right->size;
            this->segmentValue = t_function(this->segmentValue, this->right->segmentValue);
        }
    }

    template<class TType, class TFunction>
    Treap<TType, TFunction>::Treap(const TFunction &t_function) {
        this->m_function = t_function;
        this->m_root = nullptr;
        initial_random();
        this->m_size = 0;
    }

    template<class TType, class TFunction>
    template<class TIter>
    Treap<TType, TFunction>::Treap(TIter t_begin, TIter t_end, const TFunction &t_function) {
        this->m_function = t_function;
        initial_random();
        build(t_begin, t_end);
        this->m_size = this->m_root == nullptr ? 0 : this->m_root->size;
    }

    template<class TType, class TFunction>
    Treap<TType, TFunction>::Treap(const Treap<TType, TFunction> &other) {
        this->m_function = other.m_function;
        this->m_rnd = other.m_rnd;
        this->m_size = other.m_size;
        this->m_root = other.m_root == nullptr ? nullptr : std::make_unique<node>(*other.m_root);
    }

    template<class TType, class TFunction>
    Treap<TType, TFunction>::Treap(Treap<TType, TFunction> &&other) noexcept {
        this->m_function = std::move(other.m_function);
        this->m_rnd = std::move(other.m_rnd);
        this->m_size = std::move(other.m_size);
        this->m_root = std::move(other.m_root);
    }

    template<class TType, class TFunction>
    template<class TIter>
    void Treap<TType, TFunction>::build(TIter t_begin, TIter t_end) {
        std::stack<std::unique_ptr<node>> st;
        st.push(std::make_unique<node>(*t_begin++, this->m_rnd()));
        for (auto it = t_begin; it != t_end; ++it) {
            auto new_node = std::make_unique<node>(*it, this->m_rnd());
            while (!st.empty() && new_node->prior < st.top()->prior) {
                st.top()->right = std::move(new_node->left);
                st.top()->update(this->m_function);
                new_node->left = std::move(st.top());
                st.pop();
            }
            st.push(std::move(new_node));
        }
        while (st.size() > 1) {
            auto current_node = std::move(st.top());
            current_node->update(this->m_function);
            st.pop();
            st.top()->right = std::move(current_node);
        }
        this->m_root = std::move(st.top());
        this->m_root->update(this->m_function);
    }

    template<class TType, class TFunction>
    void Treap<TType, TFunction>::initial_random() {
        this->m_rnd.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    }

    template<class TType, class TFunction>
    const TType &Treap<TType, TFunction>::getValueAt(std::size_t t_index) const {
        return getValueAt(this->m_root, t_index);
    }

    template<class TType, class TFunction>
    void Treap<TType, TFunction>::changeValueAt(std::size_t t_index, const TType &t_value) {
        changeValueAt(this->m_root, t_index, t_value);
    }

    template<class TType, class TFunction>
    TType Treap<TType, TFunction>::valueOnSegment(std::size_t t_left_index, std::size_t t_right_index) const {
        return valueOnSegment(this->m_root, t_left_index, t_right_index);
    }

    template<class TType, class TFunction>
    const TType &
    Treap<TType, TFunction>::getValueAt(const std::unique_ptr<node> &t_current, std::size_t t_index) const {
        std::size_t left_size = t_current->left == nullptr ? 0 : t_current->left->size;
        if (left_size == t_index) {
            return t_current->value;
        }
        if (left_size > t_index) {
            return getValueAt(t_current->left, t_index);
        } else {
            return getValueAt(t_current->right, t_index - left_size - 1);
        }
    }

    template<class TType, class TFunction>
    void
    Treap<TType, TFunction>::changeValueAt(std::unique_ptr<node> &t_current, std::size_t t_index,
                                           const TType &t_value) {
        std::size_t left_size = t_current->left == nullptr ? 0 : t_current->left->size;
        if (left_size == t_index) {
            t_current->value = t_value;
        } else if (left_size > t_index) {
            changeValueAt(t_current->left, t_index, t_value);
        } else {
            changeValueAt(t_current->right, t_index - left_size - 1, t_value);
        }
        t_current->update(this->m_function);
    }

    template<class TType, class TFunction>
    TType Treap<TType, TFunction>::valueOnSegment(const std::unique_ptr<node> &t_current, std::size_t t_left_index,
                                                  std::size_t t_right_index) const {
        if (t_left_index == 0 && t_right_index + 1 == t_current->size) {
            return t_current->segmentValue;
        }
        std::size_t left_size = t_current->left == nullptr ? 0 : t_current->left->size;
        bool left_has = t_current->left != nullptr && t_left_index < left_size;
        bool current_has = t_left_index <= left_size && left_size <= t_right_index;
        bool right_has = t_current->right != nullptr && left_size + 1 <= t_right_index;
        if (left_has && !current_has) {
            return valueOnSegment(t_current->left, t_left_index, t_right_index);
        }
        if (right_has && !current_has) {
            return valueOnSegment(t_current->right, t_left_index - left_size - 1, t_right_index - left_size - 1);
        }
        auto result = t_current->value;
        if (left_has) {
            result = this->m_function(result, valueOnSegment(t_current->left, t_left_index,
                                                             std::min(t_right_index, left_size)));
        }
        if (right_has) {
            result = this->m_function(result, valueOnSegment(t_current->right, 0, t_right_index - left_size - 1));
        }
        return result;
    }

}

#endif //COMPETITIVE_ALGORITHMS_TREAP_H
