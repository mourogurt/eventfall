#ifndef NODEMANAGER_HPP
#define NODEMANAGER_HPP

#include "node.hpp"
#include "task.hpp"

#include "search_table.hpp"

namespace eventfall::node {

class NodeManager {
  /**
   * @todo Вместо удаления и аллоцирования сделать чтобы выделялся кусок,
   *  а потом использовался placement new когда необходим
   */
  containers::search_table<std::unique_ptr<Node>> nodes;
  /*struct {
    containers::ConcurrentSet<std::unique_ptr<Node>> nodes;
    containers::ConcurrentMap<std::string, std::size_t> lookup_table;
  } nodes_table;*/

public:
  std::optional<std::size_t> add_node(std::unique_ptr<Node> &&,
                                      std::string &&name = "");

  bool remove_node(std::size_t const &, bool remove_lookup_name = true);

  std::optional<std::size_t> lookup_id(std::string_view const &);
  std::optional<std::string_view> task_name(std::size_t const &);

  std::unique_ptr<Task> create_task(
      std::size_t const &,
      std::vector<std::pair<std::shared_ptr<eventfall::memory::MemoryArgs>,
                            std::string>> &&,
      std::string &&str = "");

  std::unique_ptr<Task> create_task(
      std::size_t const &,
      std::vector<std::pair<std::shared_ptr<eventfall::memory::MemoryArgs>,
                            std::string>> &&,
      std::unique_ptr<Task> &&, std::string &&str = "");

  std::unique_ptr<Task> create_task_buffer(
      std::initializer_list<std::size_t const &> &&,
      std::initializer_list<std::pair<
          std::shared_ptr<eventfall::memory::MemoryArgs>, std::string>> &&,
      std::string &&str = "");

  std::unique_ptr<Task> create_task_buffer(
      std::initializer_list<std::size_t const &> &&,
      std::initializer_list<std::pair<
          std::shared_ptr<eventfall::memory::MemoryArgs>, std::string>> &&,
      std::unique_ptr<Task> &&, std::string &&str = "");

  void shrink_lookup_table();

  void clear();
};

} // namespace eventfall::node

#endif // NODEMANAGER_HPP
