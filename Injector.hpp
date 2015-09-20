#include "TypeMap.hpp"
#include "InstanceContainer.hpp"
#include <memory>

namespace Injection
{
    template <class Type, class Deleter, class ...Dependencies>
    using FactoryFunction = std::unique_ptr<Type, Deleter>(*)(Dependencies*...);

    class Injector
    {
        friend class Configurator;

    public:
        Injector(Injector&& other)
        {
            *this = std::move(other);
        }

        Injector& operator=(Injector&& other)
        {
            _instanceMap = std::move(other._instanceMap);
        }

        template <class Type, class DependentType = std::nullptr_t>
        Type* Get() const
        {
            auto found = _instanceMap.find<Type>();

            if (found == _instanceMap.end())
            {
                throw Error<Type, DependentType>();
            }

            return static_cast<Type*>(found->second.Get());
        }

        template <class Type, class Deleter, class... Dependencies>
        std::unique_ptr<Type, Deleter> Inject(FactoryFunction<Type, Deleter, Dependencies...> factoryFunction) const
        {
            return factoryFunction(GetDependency<Dependencies, Type>()...);
        }

    private:
        Injector() = default;

        template<class Type, class DependentType>
        Type* GetDependency() const
        {
            return Get<Type, DependentType>();
        }

        template <class Type, class DependentType>
        std::runtime_error Error() const
        {
            const auto name = std::string(typeid(Type).name());

            const auto dependentName = std::string(typeid(DependentType).name());

            return std::runtime_error(name + ": unsatisfied dependency of " + dependentName);
        }

        Detail::TypeMap<Detail::InstanceContainer> _instanceMap;
    };

    using InitializerFunction = std::function<void(Injector&)>;
}

