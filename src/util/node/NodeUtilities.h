#pragma once

#include <initializer_list>
#include <memory>
#include <string>

#include "../../model/node/Node.h"

namespace c4 {
    namespace util {
        namespace node {
            template<class TPayload>
            std::shared_ptr<model::node::Node<char, TPayload>> makeBranch(
                std::string word,
                TPayload payload
            ) {
                auto rootNode =
                    std::make_shared<model::node::Node<char, TPayload>>();

                auto parentNode = rootNode;
                
                for (
                    auto iterator = word.cbegin();
                    iterator != word.cend();
                    iterator++
                ) {
                    std::shared_ptr<model::node::Node<char, TPayload>> curNode;
                    
                    if (iterator < word.cend() - 1) {
                        curNode =
                            std::make_shared<model::node::Node<char, TPayload>>();
                    } else {
                        curNode =
                            std::make_shared<model::node::Node<char, TPayload>>(
                                payload
                            );
                    }
                    
                    parentNode->addChildNode(*iterator, curNode);
                    parentNode = curNode;
                }

                return rootNode;
            }

            template<class TPayload>
            std::shared_ptr<model::node::Node<char, TPayload>> buildTree(
                std::initializer_list<std::shared_ptr<model::node::Node<char, TPayload>>> branches
            ) {
                auto rootNode =
                    std::make_shared<model::node::Node<char, TPayload>>();
                
                for (
                    auto iterator = branches.begin();
                    iterator != branches.end();
                    iterator++
                ) {
                    rootNode->merge(*iterator);
                }
                
                return rootNode;
            }
        }
    }
}
