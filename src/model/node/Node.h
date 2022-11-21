#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

namespace c4 {
    namespace model {
        namespace node {
            template <class key, class Value>
            class Node{
                private:
                    std::unordered_map<key, std::shared_ptr<Node>> transitions;
                    std::shared_ptr<Value> result;
                public:
                    Node();
                    Node(std::vector<key>& word, const std::shared_ptr<Value> Token);
                    bool addChildNode(key c, std::shared_ptr<Node> child);
                    std::shared_ptr<Node<key, Value>> getChildNode(key c); // auto next=ournoude->getChildNode(c) if(next)
                    void merge(const std::shared_ptr<Node> ptr);
            };
        }
    }
}