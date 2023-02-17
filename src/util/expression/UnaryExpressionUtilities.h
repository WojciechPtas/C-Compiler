#pragma once

#include <memory>
#include <set>
#include <string>

#include "../../model/expression/UnaryExpressionType.h"

namespace c4 {
    namespace util {
        namespace expression {
            std::unique_ptr<std::set<model::expression::UnaryExpressionType>> decompose(
                model::expression::UnaryExpressionType type
            );

            const std::string stringify(
                model::expression::UnaryExpressionType type
            );

            //Will print AddressOf instead of '&'
            const std::string stringifyExplicit(
                model::expression::UnaryExpressionType type
            );

        }
    }
}
