#include <unordered_map>
#include <atomic>

namespace Injection
{
    namespace Detail
    {
        extern std::atomic<int> _typeCounter;

        template <class T>
        static int RemovedConstTypeId()
        {
            static int id = ++_typeCounter;
            return id;
        }

        template <class T>
        static int TypeId()
        {
            using TRemovedConst = typename std::remove_const<T>::type;

            return RemovedConstTypeId<TRemovedConst>();
        }

        template <class T>
        class TypeMap
        {
            using Storage = std::unordered_map<int, T>;

        public:
            using iterator = typename Storage::iterator;

            using const_iterator = typename Storage::const_iterator;

            iterator begin() { return _storage.begin(); }

            iterator end() { return _storage.end(); }

            const_iterator begin() const { return _storage.begin(); }

            const_iterator end() const { return _storage.end(); }

            const_iterator cbegin() const { return _storage.cbegin(); }

            const_iterator cend() const { return _storage.cend(); }

            template <class Key>
            iterator find() { return _storage.find(TypeId<Key>()); }

            template <class Key>
            const_iterator find() const { return _storage.find(TypeId<Key>()); }

            template <class Key>
            void put(T&& t) { _storage[TypeId<Key>()] = std::forward<T>(t); }

        private:
            Storage _storage;
        };
    }
}

