#include "Node.h"

using namespace c4::model::node;

template<class key, class Value>
Node<key, Value>::Node(std::vector<key>& word, std::shared_ptr<Value> Token){
    if(word.size()==0) {
        this->result=Token;
    }
    else{
        key _key=word[0];
        word.erase(word.begin());
        this->result=nullptr;
        this->transitions[_key]=std::make_shared<Node>(word, Token);
    }
}

template<class key, class Value>
bool Node<key, Value>::addChildNode(key c, std::shared_ptr<Node> child){
    if(this->transitions.find(c)==this->transitions.end()){
        this->transitions[c]=child;
        return true;
    }
    else{
        return false;
    }
}
template<class key, class Value>
std::shared_ptr<Node<key, Value>> Node<key, Value>::getChildNode(key c){
    if(this->transitions.find(c)==this->transitions.end()){
        return nullptr;
    }
    else{
        return this->transitions[c];
    }
}

template<class key, class Value>
void Node<key, Value>::merge(const std::shared_ptr<Node> ptr){
    this->result=ptr->result;
    for(auto it = ptr->transitions.begin(); it!=ptr->transitions.end(); ++it){
        if(this->transitions.find(it->first)==this->transitions.end()){
            this->transitions[it->first]=it->second;
        }
        else{
            this->transitions[it->first]->merge(ptr->transitions[it->first]);
        }
    }
}