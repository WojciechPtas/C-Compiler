#pragma once
#include <memory>
#include <vector>
#include "../statement/IStatement.h"
namespace c4 {
    namespace model {
        namespace declaration {
            class IDeclaration:public statement::IStatement{};
            }
        }
    }