#ifndef SEARCH_TABLE_HPP
#define SEARCH_TABLE_HPP

#include "containers.hpp"

namespace eventfall::containers {

template <class T, class Key = std::string_view, class Holder = std::string,
          class Set = ConcurrentSet<T>, class Hash = cds::opt::v::hash<T>,
          class Map = ConcurrentMap<Holder, std::size_t>,
          class ReverseMap = ConcurrentMap<std::size_t, Key>>
class search_table {
  Set items;
  Map hash_map;
  ReverseMap reverse_hash_map;

public:
  std::optional<std::size_t> add_item(std::pair<T, Key> &&item) {
    auto hash = Hash()(item.first);
    if (items.emplace(std::move(item.first))) {
      if (!item.second.empty()) {
        if (!hash_map.emplace(Holder{item.second}, hash)) {
          hash_map.erase(item.second);
          hash_map.emplace(Holder{item.second}, hash);
        }
        if (!reverse_hash_map.emplace(hash, hash_map.get(item.second)->first)) {
          reverse_hash_map.erase(hash);
          reverse_hash_map.emplace(hash, hash_map.get(item.second)->first);
        }
      }
      return {hash};
    }
    return std::nullopt;
  }

  bool remove_item(std::size_t const &hash, bool remove_lookup_name = true) {
    if (items.erase(hash)) {
      if (remove_lookup_name) {
        auto item = reverse_hash_map.get(hash);
        if (!item.empty()) {
          hash_map.erase(item->second);
          reverse_hash_map.erase(item->first);
        }
      }
      return true;
    }
    return false;
  }

  std::optional<std::reference_wrapper<T>> get_item(std::size_t const &hash) {
    auto item = items.get(hash);
    if (!item.empty())
      return std::make_optional(std::ref(*item));
    else
      return std::nullopt;
  }

  std::optional<std::size_t> find_hash(Key const &key) {
    auto item = hash_map.get(key);
    if (!item.empty())
      return {item->second};
    else
      return std::nullopt;
  }

  std::optional<Key> find_key(std::size_t const &hash) {
    auto item = reverse_hash_map.get(hash);
    if (!item.empty())
      return {item->second};
    else
      return std::nullopt;
  }

  void clear() {
    items.clear();
    hash_map.clear();
    reverse_hash_map.clear();
  }

  void shrink_table() {
    for (auto item = hash_map.begin(); item != hash_map.end(); ++item) {
      if (!items.contains(item->second)) {
        reverse_hash_map.erase(item->second);
        hash_map.erase_at(item);
      }
    }
  }
};

} // namespace eventfall::containers

#endif // SEARCH_TABLE_HPP
