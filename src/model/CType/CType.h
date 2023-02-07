#pragma once

#include "llvm/IR/IRBuilder.h"

using namespace llvm;

namespace c4::model::ctype {

enum TypeSpecifier {
    VOID,
    INT,
    CHAR
};


class CType {
    protected:
        enum {
            NOT_A_FUNCTION = false,
            FUNCTION = true
        };
    public:
        uint indirections;
        const bool isFunction; 

        CType(uint indirections, bool isFunction) : indirections(indirections), isFunction(isFunction) {}
        virtual Type* getLLVMType(IRBuilder<> &builder) const = 0;
        virtual bool equals(const CType* another) const = 0;

        void incIndirections() {
            indirections++;
        }

        void decIndirections() {
            if(indirections == 0) {
                throw std::logic_error("Decremented indirections below 0!");
            }
            indirections--;
        }
    };
}