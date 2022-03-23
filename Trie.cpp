#include "Trie.h"
#include "Constants.h"

#include <queue>

Trie::Node::Node() {
    l_ = nullptr;
    r_ = nullptr;
    term_ = 0;
    symbol_ = NO_SYMBOL;
    weight_ = 0;
}

Trie::Node::Node(int16_t symbol, size_t weight) {
    l_ = nullptr;
    r_ = nullptr;
    term_ = 1;
    symbol_ = symbol;
    weight_ = weight;
}

Trie::Node::Node(Trie::Node *l, Trie::Node *r) {
    l_ = l;
    r_ = r;
    term_ = 0;
    symbol_ = NO_SYMBOL;
    weight_ = l->weight_ + r_->weight_;
}

Trie::Trie() {
    ptr_ = 0;
    nodes_ = new Node*[MAX_NODES];
    for (size_t i = 0; i < MAX_NODES; ++i) {
        nodes_[i] = new Node();
    }
    root_ = nullptr;
}

Trie::~Trie() {
    for (size_t i = 0; i < MAX_NODES; ++i) {
        delete nodes_[i];
    }
    delete[] nodes_;
}

Trie::Node* Trie::NewNode() {
    return nodes_[ptr_++];
}

bool Trie::CompareNodes::operator()(const Trie::Node *lhs, const Trie::Node *rhs) const {
    return lhs->weight_ > rhs->weight_;
}

void Trie::Dfs(Trie::Node *v, TriePath path, std::vector<TriePath>& ans) const {
    if (v->term_) {
        path.symbol_ = v->symbol_;
        ans.push_back(path);
        return;
    }
    TriePath lpath = path;
    ++lpath.sz_;
    lpath.path_ = (lpath.path_ << 1);
    TriePath rpath = lpath;
    ++rpath.path_;
    Dfs(v->l_, lpath, ans);
    Dfs(v->r_, rpath, ans);
}

void Trie::BuildUsingCnt(const std::unordered_map<int16_t, size_t>& cnt) {
    std::priority_queue<Node*, std::vector<Node*>, CompareNodes> order;
    for (const auto& [symbol, weight] : cnt) {
        Node* nw = NewNode();
        *nw = Node(symbol, weight);
        order.push(nw);
    }
    while (order.size() > static_cast<size_t>(1)) {
        Node* l = order.top();
        order.pop();
        Node* r = order.top();
        order.pop();
        Node* nw = NewNode();
        *nw = Node(l, r);
        order.push(nw);
    }
    root_ = order.top();
}

void Trie::AddPath(TriePath path) {
    if (root_ == nullptr) {
        root_ = NewNode();
        *root_ = Node();
    }
    Node* cur = root_;
    for (int16_t i = 0; i < path.sz_; ++i) {
        if (((path.path_ >> i) & 1) == 0) {
            if (cur->l_ == nullptr) {
                cur->l_ = NewNode();
            }
            cur = cur->l_;
        } else {
            if (cur->r_ == nullptr) {
                cur->r_ = NewNode();
            }
            cur = cur->r_;
        }
    }
    cur->term_ = 1;
    cur->symbol_ = path.symbol_;
}

std::vector<TriePath> Trie::FindPaths() const {
    std::vector<TriePath> ans;
    Dfs(root_, TriePath(), ans);
    return ans;
}
