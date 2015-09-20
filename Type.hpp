#include "Injector.hpp"
#include <unordered_set>

namespace Injection
{
    namespace Detail
    {
        struct Type
        {
            enum class State { Unsorted, Sorting, Sorted };

            bool IsInitializerFunctionSet() const
            {
                return _initializerFunction ? true : false;
            }

            void AddDependentType(int typeId)
            {
                _dependentTypeIds.insert(typeId);
            }

            State _sortedState;
            std::string _debugTypeName;
            InitializerFunction _initializerFunction;
            std::unordered_set<int> _dependentTypeIds;
        };
    }
}

