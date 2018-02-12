#ifndef NODE_HPP
#define NODE_HPP

#include "task.hpp"

namespace eventfall::node {

class Node {

public:
  OUT_METAINFO(Node)
  constexpr static decltype(auto) classname() { return "Node"_s; }
  virtual std::string_view runtime_classname() { return "Node"; }
  virtual std::optional<std::reference_wrapper<Task>>
  gen_task(std::vector<
               std::pair<std::shared_ptr<memory::MemoryArgs>, std::string>> &&,
           std::string &&obj_name = "") = 0;
  virtual std::optional<std::reference_wrapper<Task>>
  init_task(std::vector<
                std::pair<std::shared_ptr<memory::MemoryArgs>, std::string>> &&,
            eventfall::node::Task *, std::string &&obj_name = "") = 0;
  virtual std::size_t allocated_memory() const = 0;
  virtual ~Node();
};

} // namespace eventfall::node

METAINFO(eventfall::node::Node)
REFLECT_STATIC_MTD(classname)
REFLECT_OBJ_MTD(runtime_classname)
REFLECT_OBJ_MTD(
    gen_task,
    std::vector<
        std::pair<std::shared_ptr<eventfall::memory::MemoryArgs>, std::string>>
        &&,
    std::string &&)
REFLECT_OBJ_MTD(
    init_task,
    std::vector<
        std::pair<std::shared_ptr<eventfall::memory::MemoryArgs>, std::string>>
        &&,
    eventfall::node::Task *, std::string &&)
REFLECT_CONST_OBJ_MTD(allocated_memory)
END_METAINFO
#endif // NODE_HPP
