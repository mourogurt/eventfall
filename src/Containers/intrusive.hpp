#ifndef INTRUSIVE_HPP
#define INTRUSIVE_HPP

#include <tuple>
#include <utility>

#include "common_defines.hpp"

#include "cds/intrusive/feldman_hashset_dhp.h"
#include "cds/opt/hash.h"

#include "tbb/concurrent_queue.h"
#include "tbb/concurrent_vector.h"

namespace eventfall::intrusive {

template <class Key, class Hash = cds::opt::v::hash<Key>,
          class Equal = std::equal_to<Key>,
          class Counter = cds::atomicity::cache_friendly_item_counter>
using IntrusiveConcurrentSet = cds::intrusive::FeldmanHashSet<
    gc, Key,
    typename cds::intrusive::feldman_hashset::make_traits<
        cds::intrusive::feldman_hashset::hash_accessor<Hash>,
        cds::opt::equal_to<Equal>,
        cds::intrusive::opt::item_counter<Counter>>::type>;

} // namespace eventfall::intrusive

#endif // INTRUSIVE_HPP
