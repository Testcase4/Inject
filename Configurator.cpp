#include "Configurator.hpp"

using namespace Injection;

Injector Configurator::CreateInjector()
{
    _unsortedTypes = {};
    for (auto& type : _types)
    {
        type.second._sortedState = Detail::Type::State::Unsorted; 
        _unsortedTypes.insert(type.first);
    }

    _sortedInitializers = {};
    while (!_unsortedTypes.empty())
    {
        SortDependentInitializersTopographically(*_unsortedTypes.begin());
    }

    auto injector = Injector();
    while (!_sortedInitializers.empty())
    {
        _sortedInitializers.top()(injector);
        _sortedInitializers.pop();
    }

    return std::move(injector);
}

void Configurator::SortDependentInitializersTopographically(int typeId)
{
    auto& type = _types[typeId];

    if (type._sortedState == Detail::Type::State::Sorting)
    {
        throw std::runtime_error(type._debugTypeName + " appears to be part of a cycle");
    }

    if (type._sortedState == Detail::Type::State::Unsorted)
    {
        _unsortedTypes.erase(typeId);
        type._sortedState = Detail::Type::State::Sorting;

        for (auto dependentTypeId : type._dependentTypeIds)
        {
            SortDependentInitializersTopographically(dependentTypeId);
        }

        type._sortedState = Detail::Type::State::Sorted;
        if (type.IsInitializerFunctionSet())
        {
            _sortedInitializers.push(type._initializerFunction);
        }
    }
}

