#pragma once
#include <unordered_map>
#include <memory>

namespace c4 {
    namespace service {
        namespace Node {
            class Node{
                    private:
                        std::unordered_map<char, std::shared_ptr<Node>> transitions;
                        bool is_finite;
                    public:
                        Node();
                        bool isFinite(){return is_finite;}
            };



        }}}