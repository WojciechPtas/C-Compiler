#pragma once

#include "CType.h"

namespace c4::model::ctype {

    class ParametersInfo {
    public:
        std::vector<std::string> names;
        std::vector<std::shared_ptr<CType>> types;

        // ParametersInfo() = default;
        // ParametersInfo(
        //     std::vector<std::string> names,
        //     std::vector<std::shared_ptr<CType>> types
        // ) {

        // }
        
        std::vector<Type*> getLLVMTypes(IRBuilder<> &builder) const {
            std::vector<Type*> ret;
            for(std::shared_ptr<CType> ctype : types) {
                ret.push_back(ctype->getLLVMType(builder));
            }
            return ret;
        }
    };
}