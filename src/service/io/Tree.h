#pragma once
#include "Node.h"


namespace c4 {
    namespace service {
        namespace Tree {
                    class Tree{
                        private:
                            std::shared_ptr<c4::service::Node::Node> head;

                        public:
                            Tree();
                            void merge(const Tree& t);
                            
                    };




        
        
        
        
                }
}
}