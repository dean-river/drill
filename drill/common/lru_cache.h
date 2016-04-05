#ifndef DRILL_COMMON_LRU_CACHE_H_
#define DRILL_COMMON_LRU_CACHE_H_

#include <cassert>
#include <cstddef>
#include <functional>
#include <list>
#include <memory>
#include <unordered_map>
#include <utility>

namespace drill {
namespace common {

template <typename Key, typename Value>
class LruCache
{
public:
    typedef std::pair<Key, Value> KeyValuePair;

    typedef std::list<KeyValuePair> List;
    typedef typename List::iterator ListIterator;

    typedef std::unordered_map<Key, ListIterator> Map;

    //! put or replace/touch item in LRU cache
    void put(const Key& key, const Value& value) {

        typename Map::iterator it = _map.find(key);
        if (it != _map.end()) {
            _list.erase(it->second);
            _map.erase(it);
        }
        
        _list.push_front(KeyValuePair(key, value));
        _map[key] = _list.begin();
    }

    //! get and touch value from LRU cache for key.
    bool get(const Key& key, Value &v) {
        typename Map::iterator it = _map.find(key);
        if (it == _map.end()) {
            return false;
        }
        else {
            _list.splice(_list.begin(), _list, it->second);
            v = it->second->second;
            return true;
        }
    }

    //! test if key exists in LRU cache
    bool exists(const Key& key) const {
        return _map.find(key) != _map.end();
    }

    //! return number of items in LRU cache
    size_t size() const {
        return _map.size();
    }

    //! return the least recently used key value pair
    KeyValuePair pop() {
        assert(size());
        typename List::iterator last = _list.end();
        --last;
        KeyValuePair out = *last;
        _map.erase(last->first);
        _list.pop_back();
        return out;
    }

private:
    //! list of entries in least-recently used order.
    List _list;
    //! map for accelerated access to keys
     Map _map;
};


template <typename Key, typename Alloc = std::allocator<Key> >
class LruCacheSet
{
public:
    typedef std::list<Key, Alloc> List;
    typedef typename List::iterator ListIterator;

    typedef typename std::unordered_map<
              Key, ListIterator, std::hash<Key>, std::equal_to<Key>,
              typename Alloc::template rebind<
                  std::pair<const Key, ListIterator> >::other> Map;

    explicit LruCacheSet(const Alloc& alloc = Alloc())
        : _list(alloc), _map(alloc) { }

    //! put or replace/touch item in LRU cache
    void put(const Key& key) {
        // first try to find an existing key
        typename Map::iterator it = _map.find(key);
        if (it != _map.end()) {
            _list.erase(it->second);
            _map.erase(it);
        }

        // insert key into linked list at the front (most recently used)
        _list.push_front(key);
        // store iterator to linked list entry in map
        _map[key] = _list.begin();
    }

    //! touch value from LRU cache for key.
    bool touch(const Key& key) {
        typename Map::iterator it = _map.find(key);
        if (it == _map.end()) {
            return false;
        }
        else {
            _list.splice(_list.begin(), _list, it->second);
            return true;
        }
    }

    //! remove key from LRU cache
    bool erase(const Key& key) {
        typename Map::iterator it = _map.find(key);
        if (it == _map.end()) {
            return false;
        }
        else {
            _list.erase(it->second);
            _map.erase(it);
            return true;
        }
    }

    //! test if key exists in LRU cache
    bool exists(const Key& key) const {
        return _map.find(key) != _map.end();
    }

    //! return number of items in LRU cache
    size_t size() const {
        return _map.size();
    }

    //! return the least recently used key value pair
    Key pop() {
        assert(size());
        typename List::iterator last = _list.end();
        --last;
        Key out = *last;
        _map.erase(out);
        _list.pop_back();
        return out;
    }

private:

    List _list;
    Map _map;
};

} // namespace common
} // namespace thrill

#endif // !THRILL_COMMON_LRU_CACHE_HEADER

/******************************************************************************/
