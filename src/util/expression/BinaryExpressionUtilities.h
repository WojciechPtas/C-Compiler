#pragma once

#include <memory>
#include <set>
#include <string>

#include "../../model/expression/BinaryExpressionType.h"

namespace c4 {
    namespace util {
        namespace expression {
            std::unique_ptr<std::set<model::expression::BinaryExpressionType>> decompose(
                model::expression::BinaryExpressionType type
            );

            const std::string stringify(
                model::expression::BinaryExpressionType type
            );
        }
    }
}
