#ifndef FUNCTIONNODE_HPP
#define FUNCTIONNODE_HPP

#include "functiontask.hpp"
#include "node.hpp"

namespace eventfall::node {

template <class T, class K> class FunctionNode : public Node {
  T setup_func;
  K exec_func;

public:
  FunctionNode(T &&setup_func, K &&exec_func)
      : setup_func(std::move(setup_func)), exec_func(std::move(exec_func)) {}
  constexpr static decltype(auto) classname() { return "FunctionNode"_s; }
  virtual std::string_view runtime_classname() override {
    return "FunctionNode";
  }
  virtual std::optional<std::reference_wrapper<Task>>
  gen_task(std::vector<
               std::pair<std::shared_ptr<memory::MemoryArgs>, std::string>> &&,
           std::string &&obj_name = "") override;
  virtual std::optional<std::reference_wrapper<Task>>
  init_task(std::vector<
                std::pair<std::shared_ptr<memory::MemoryArgs>, std::string>> &&,
            Task *, std::string &&obj_name = "") override;

  virtual std::size_t allocated_memory() const override {
    return sizeof(FunctionNode<T, K>);
  }
  virtual ~FunctionNode() override {}

  IN_METAINFO(FunctionNode, Node)
  REFLECT_STATIC_MTD(classname)
};

template <class T, class K>
std::optional<std::reference_wrapper<Task>> FunctionNode<T, K>::gen_task(
    std::vector<
        std::pair<std::shared_ptr<memory::MemoryArgs>, std::string>> &&args,
    std::string &&obj_name) {
  return make_func_task_base_ptr(std::move(args), T(setup_func), K(exec_func),
                                 std::move(obj_name));
}

template <class T, class K>
std::optional<std::reference_wrapper<Task>> FunctionNode<T, K>::init_task(
    std::vector<
        std::pair<std::shared_ptr<memory::MemoryArgs>, std::string>> &&args,
    Task *task, std::string &&obj_name) {
  return make_func_task_base_ptr(std::move(args), std::move(task),
                                 T(setup_func), K(exec_func),
                                 std::move(obj_name));
}

template <class T, class K>
decltype(auto) make_func_node_base_ptr(T &&setup_func, K &&exec_func) {
  return std::unique_ptr<Node>(
      new FunctionNode<std::decay_t<T>, std::decay_t<K>>(std::move(setup_func),
                                                         std::move(exec_func)));
}

template <class T, class K>
decltype(auto) make_func_node_base_ptr(std::unique_ptr<Node> &&node,
                                       T &&setup_func, K &&exec_func) {
  if (node->allocated_memory() < sizeof(FunctionNode<T, K>)) {
    return make_func_node_base_ptr(setup_func, exec_func);
  } else {
    auto mem = node.release();
    mem->~Node();
    return std::unique_ptr<Node>(
        new (mem) FunctionNode<std::decay_t<T>, std::decay_t<K>>(
            std::move(setup_func), std::move(exec_func)));
  }
}

} // namespace eventfall::node

#endif // FUNCTIONNODE_HPP
