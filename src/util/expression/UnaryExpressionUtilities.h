#pragma once

#include <memory>
#include <set>
#include "../../model/expression/UnaryExpressionType.h"

namespace c4 {
    namespace util {
        namespace expression {
            std::unique_ptr<std::set<model::expression::UnaryExpressionType>> decompose(
                model::expression::UnaryExpressionType type
            );
        }
    }
}
