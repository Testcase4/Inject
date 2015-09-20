#include <memory>

namespace Injection
{
    namespace Detail
    {
        class InstanceContainer
        {
        public:
            InstanceContainer() = default;

            InstanceContainer(InstanceContainer&& other)
                : _concept(std::move(other._concept))
            {}

            InstanceContainer operator = (InstanceContainer&& other)
            {
                _concept = std::move(other._concept);
            }

            template <class Type, class Deleter>
            InstanceContainer(std::unique_ptr<Type, Deleter> value)
                : _concept(new TypeErasureModel<Type, Deleter>(std::move(value)))
            {}

            void* Get() const
            {
                return _concept->Get();
            }

        private:
            struct TypeErasureConcept
            {
                virtual ~TypeErasureConcept() = default;
                virtual void* Get() const = 0;
            };

            template <class Type, class Deleter>
            struct TypeErasureModel : public TypeErasureConcept
            {
                TypeErasureModel(TypeErasureModel&& other)
                    : _value(std::move(other._value))
                {}

                TypeErasureModel operator = (TypeErasureModel&& other)
                {
                    _value = std::move(other._value);
                }

                TypeErasureModel(std::unique_ptr<Type, Deleter> value)
                    : _value(std::move(value))
                {}

                void* Get() const override
                {
                    return _value.get();
                }

                std::unique_ptr<Type, Deleter> _value;
            };

            std::unique_ptr<TypeErasureConcept> _concept;
        };
    }
}

