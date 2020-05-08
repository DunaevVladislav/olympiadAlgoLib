#ifndef COMPETITIVE_ALGORITHMS_SUFFIXAUTOMATON_H
#define COMPETITIVE_ALGORITHMS_SUFFIXAUTOMATON_H

#include <map>
#include <vector>
#include <queue>

namespace cpa {

    class SuffixAutomaton {
    public:
        struct Node {
            friend class SuffixAutomaton;
        private:
            bool invLinkWas = false;
        public:
            std::map<char, Node *> child;
            std::vector<Node *> invLinks;
            Node *suffixLink;
            std::size_t length;
            int position;

            explicit Node(std::size_t length = 0);

            Node(const Node &other) = default;
        };

        void assign(const std::string &t_s);

        explicit SuffixAutomaton(const std::string &t_s = std::string());

        SuffixAutomaton(const SuffixAutomaton &other);

        SuffixAutomaton(SuffixAutomaton &&other) noexcept;

        [[nodiscard]] Node *getRoot() const noexcept;

        [[nodiscard]] int find(const std::string &t_s) const;

        [[nodiscard]] std::vector<int> findAll(const std::string &t_s) const;

        ~SuffixAutomaton();

    private:
        Node *m_root{};
        Node *m_last{};

        void add_character(char t_c, std::size_t t_length);

        void buildInvLinks(Node *current);
    };

    void SuffixAutomaton::add_character(char t_c, std::size_t t_length) {
        auto new_node = new Node(t_length);
        auto p = m_last;
        while (p != nullptr && p->child.count(t_c) == 0) {
            p->child[t_c] = new_node;
            p = p->suffixLink;
        }
        if (p == nullptr) {
            new_node->suffixLink = m_root;
        } else {
            auto q = p->child[t_c];
            if (q->length == p->length + 1) {
                new_node->suffixLink = q;
            } else {
                auto clone_node = new Node(*q);
                clone_node->length = p->length + 1;
                q->suffixLink = clone_node;
                new_node->suffixLink = clone_node;
                while (p != nullptr && p->child[t_c] == q) {
                    p->child[t_c] = clone_node;
                    p = p->suffixLink;
                }
            }
        }
        m_last = new_node;
    }

    void SuffixAutomaton::buildInvLinks(Node *current) {
        current->invLinkWas = true;
        if (current->suffixLink != nullptr) {
            current->suffixLink->invLinks.push_back(current);
        }
        for (auto &p : current->child) {
            if (!p.second->invLinkWas){
                buildInvLinks(p.second);
            }
        }
    }

    void SuffixAutomaton::assign(const std::string &t_s) {
        m_root = new Node();
        m_last = m_root;
        for (std::size_t i = 0; i < t_s.length(); ++i) {
            add_character(t_s[i], i + 1);
        }
        buildInvLinks(m_root);
    }

    SuffixAutomaton::SuffixAutomaton(const std::string &t_s) {
        assign(t_s);
    }


    SuffixAutomaton::SuffixAutomaton(const SuffixAutomaton &other) {
        std::string s;
        auto cur = other.m_root;
        while (!cur->child.empty()) {
            char nxt;
            for (auto &p : cur->child) {
                if (p.second->position == cur->position + 1) {
                    nxt = p.first;
                    break;
                }
            }
            s.push_back(nxt);
            cur = cur->child[nxt];
        }
        assign(s);
    }

    SuffixAutomaton::SuffixAutomaton(SuffixAutomaton &&other) noexcept {
        this->m_root = other.m_root;
        this->m_last = other.m_last;
        other.m_root = nullptr;
        other.m_last = nullptr;
    }

    int SuffixAutomaton::find(const std::string &t_s) const {
        auto cur = this->m_root;
        for (auto c : t_s) {
            if (cur->child.count(c) == 0) {
                return -1;
            }
            cur = cur->child[c];
        }
        return static_cast<int>(cur->position + 1 - t_s.length());
    }

    std::vector<int> SuffixAutomaton::findAll(const std::string &t_s) const {
        auto cur = this->m_root;
        for (auto c : t_s) {
            if (cur->child.count(c) == 0) {
                return {};
            }
            cur = cur->child[c];
        }
        std::vector<int> pos;
        std::queue<Node *> q;
        q.push(cur);
        while (!q.empty()) {
            auto v = q.front();
            q.pop();
            if (v->position + 1 == v->length) {
                pos.push_back(static_cast<int>(v->position + 1 - t_s.length()));
            }
            for (auto &u : v->invLinks) {
                q.push(u);
            }
        }
        return pos;
    }

    SuffixAutomaton::~SuffixAutomaton() {
        std::vector<Node *> q;
        q.push_back(this->m_root);
        this->m_root->position = -2;
        for (std::size_t i = 0; i < q.size(); ++i) {
            auto v = q[i];
            for (auto &p : v->child) {
                if (p.second->position != -2) {
                    p.second->position = -2;
                    q.push_back(p.second);
                }
            }
        }
        for (auto &v : q) {
            delete v;
        }
        this->m_root = nullptr;
        this->m_last = nullptr;
    }

    SuffixAutomaton::Node *SuffixAutomaton::getRoot() const noexcept {
        return this->m_root;
    }


    SuffixAutomaton::Node::Node(std::size_t length) {
        suffixLink = nullptr;
        this->length = length;
        this->position = static_cast<int>(length) - 1;
    }

}

#endif //COMPETITIVE_ALGORITHMS_SUFFIXAUTOMATON_H
