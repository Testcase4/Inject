/* Implementation derived from http://gpfault.net/posts/dependency-injection-cpp.txt.html  */

#include "Type.hpp"
#include <stack>

namespace Injection
{
    namespace Detail
    {
        template <class... Args>
        inline void ForEachType(Args... args)
        {}
    }

    class Configurator
    {
    public:
        template <class Type, class Deleter, class... Dependencies>
        void AddFactory(FactoryFunction<Type, Deleter, Dependencies...> factoryFunction)
        {
            auto typeId = Detail::TypeId<Type>();

            auto& type = _types[typeId];

            type._initializerFunction = [factoryFunction](Injector& injector)
            {
                auto instance = Detail::InstanceContainer(injector.Inject(factoryFunction));

                injector._instanceMap.put<Type>(std::move(instance));
            };

            type._debugTypeName = typeid(typename std::remove_const<Type>::type).name();

            Detail::ForEachType(AddDependency<Dependencies>(typeId)...);
        }

        Injector CreateInjector();

    private:
        template<class Dependency>
        int AddDependency(int typeId)
        {
            const auto dependencyId = Detail::TypeId<Dependency>();

            _types[dependencyId].AddDependentType(typeId);

            const auto dummy = 0;

            return dummy;
        }

        void SortDependentInitializersTopographically(int typeId);

        std::unordered_map<int, Detail::Type> _types;
        std::unordered_set<int> _unsortedTypes;
        std::stack<InitializerFunction> _sortedInitializers;
    };
}

