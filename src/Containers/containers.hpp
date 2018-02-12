#ifndef CONCURENT_CONTAINER_TYPES_HPP
#define CONCURENT_CONTAINER_TYPES_HPP

#include <tuple>
#include <utility>

#include "common_defines.hpp"

#include "cds/container/feldman_hashmap_dhp.h"
#include "cds/container/feldman_hashset_dhp.h"

#include "tbb/concurrent_queue.h"
#include "tbb/concurrent_vector.h"

namespace eventfall::containers {

template <class Key, class T, class Hash = cds::opt::v::hash<Key>,
          class Equal = std::equal_to<Key>,
          class Counter = cds::atomicity::cache_friendly_item_counter>
using ConcurrentMap = cds::container::FeldmanHashMap<
    gc, Key, T,
    typename cds::container::feldman_hashmap::make_traits<
        cds::opt::hash<Hash>, cds::opt::equal_to<Equal>,
        cds::container::opt::item_counter<Counter>>::type>;

template <class Key, class Hash = cds::opt::v::hash<Key>,
          class Equal = std::equal_to<Key>,
          class Counter = cds::atomicity::cache_friendly_item_counter>
using ConcurrentSet = cds::container::FeldmanHashSet<
    gc, Key,
    typename cds::container::feldman_hashset::make_traits<
        cds::container::feldman_hashset::hash_accessor<Hash>,
        cds::opt::equal_to<Equal>,
        cds::container::opt::item_counter<Counter>>::type>;

template <class T, class Allocator = tbb::cache_aligned_allocator<T>>
using ConcurrentQueue = tbb::concurrent_queue<T, Allocator>;

/**
 * @todo Vector benchmark
 */
template <class T, class Allocator = tbb::cache_aligned_allocator<T>>
using ConcurrentVector = tbb::concurrent_vector<T, Allocator>;

} // namespace eventfall::containers

#endif // CONCURENT_CONTAINER_TYPES_HPP
