#include <string>
#include "ConstantExpression.h"

namespace c4 {
    namespace model {
        namespace expression {
            class CharacterConstantExpression :
                public ConstantExpression<std::string> {
            public:
                CharacterConstantExpression(const std::string &value);
                virtual ~CharacterConstantExpression() { }
            };
        }
    }
}
