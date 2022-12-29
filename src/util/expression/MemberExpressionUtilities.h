#pragma once

#include <memory>
#include <set>
#include "../../model/expression/MemberExpressionType.h"

namespace c4 {
    namespace util {
        namespace expression {
            std::unique_ptr<std::set<model::expression::MemberExpressionType>> decompose(
                model::expression::MemberExpressionType type
            );
        }
    }
}
