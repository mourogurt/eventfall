#include "nodemanager.hpp"

namespace eventfall::node {

std::optional<std::size_t> NodeManager::add_node(std::unique_ptr<Node> &&node,
                                                 std::string &&name) {
  return nodes.add_item(std::make_pair(std::move(node), std::move(name)));
}

bool NodeManager::remove_node(std::size_t const &id, bool remove_lookup_name) {
  return nodes.remove_item(id, remove_lookup_name);
}

std::optional<std::size_t>
NodeManager::lookup_id(const std::string_view &name) {
  return nodes.find_hash(name);
}

std::optional<std::string_view> NodeManager::task_name(std::size_t const &id) {
  return nodes.find_key(id);
}

std::unique_ptr<Task> NodeManager::create_task(
    std::size_t const &id,
    std::vector<std::pair<std::shared_ptr<eventfall::memory::MemoryArgs>,
                          std::string>> &&mem,
    std::string &&str) {
  auto item = nodes.get_item(id);
  if (item.has_value()) {
    auto ptr = item.value();
    auto value = ptr.get()->gen_task(std::move(mem), std::move(str));
    if (value.has_value())
      return std::unique_ptr<Task>(&value.value().get());
    else
      return {nullptr};
  }
  return {nullptr};
}

std::unique_ptr<Task> NodeManager::create_task(
    std::size_t const &id,
    std::vector<std::pair<std::shared_ptr<eventfall::memory::MemoryArgs>,
                          std::string>> &&mem,
    std::unique_ptr<Task> &&task, std::string &&str) {
  auto item = nodes.get_item(id);
  if (item.has_value()) {
    auto ptr = std::move(item.value());
    auto value = ptr.get()->init_task(std::move(mem), std::move(task.release()),
                                      std::move(str));
    if (value.has_value())
      return std::unique_ptr<Task>(&value.value().get());
    else
      return {nullptr};
  }
  return {nullptr};
}

void NodeManager::shrink_lookup_table() { nodes.shrink_table(); }

void NodeManager::clear() { nodes.clear(); }

} // namespace eventfall::node
