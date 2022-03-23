#pragma ocne

#include <cstdint>
#include <cstddef>
#include <unordered_map>
#include <vector>

#include "TriePath.h"

class Trie {
public:
    struct Node {
        Node* l_;
        Node* r_;
        bool term_;
        int16_t symbol_;
        size_t weight_;

        Node();
        Node(int16_t symbol, size_t weight);
        Node(Node* l, Node* r);
    };

    Node* root_;

    Trie();
    ~Trie();

    Node* NewNode();
    void BuildUsingCnt(const std::unordered_map<int16_t, size_t>& cnt);
    void AddPath(TriePath path);

    void Dfs(Node* v, TriePath path, std::vector<TriePath>& ans) const;
    std::vector<TriePath> FindPaths() const;

    struct CompareNodes {
        bool operator() (const Node* lhs, const Node* rhs) const;
    };
private:
    Node** nodes_;
    size_t ptr_;
};
