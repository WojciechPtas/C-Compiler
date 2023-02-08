#pragma once

#include "CType.h"

namespace c4::model::ctype {

    class ParametersInfo {
    public:
        std::vector<std::string> names;
        std::vector<std::shared_ptr<const CType>> types;

        // ParametersInfo() = default;
        // ParametersInfo(
        //     std::vector<std::string> names,
        //     std::vector<std::shared_ptr<CType>> types
        // ) {

        // }
        
        std::vector<llvm::Type*> getLLVMTypes(llvm::LLVMContext &ctx) const {
            std::vector<llvm::Type*> ret;
            for(auto& ctype : types) {
                ret.push_back(ctype->getLLVMType(ctx));
            }
            return ret;
        }
    };
}