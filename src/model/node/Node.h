#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

namespace c4 {
    namespace model {
        namespace node {
            template <class key, class Value>
            class Node {
                private:
                    std::unordered_map<key, std::shared_ptr<Node>> transitions;
                    std::shared_ptr<Value> result;

                public:
                    Node() : result(nullptr) { }

                    Node(Value result) : result(std::make_shared<Value>(result)) { }

                    bool addChildNode(key c, std::shared_ptr<Node> child) {
                        if(this->transitions.find(c)==this->transitions.end()){
                            this->transitions[c]=child;
                            return true;
                        }
                        
                        return false;
                    }

                    std::shared_ptr<const Node<key, Value>> getChildNode(key c) const {
                        if(this->transitions.find(c)==this->transitions.end()){
                            return nullptr;
                        }
                        
                        return this->transitions.at(c);
                    }

                    void merge(const std::shared_ptr<Node> ptr) {
                        this->result = ptr->result;
                        for(auto it = ptr->transitions.begin(); it!=ptr->transitions.end(); ++it){
                            if(this->transitions.find(it->first)==this->transitions.end()){
                                this->transitions[it->first]=it->second;
                            }else{
                                this->transitions[it->first]->merge(ptr->transitions[it->first]);
                            }
                        }
                    }
            };
        }
    }
}