#ifndef TASK_HPP
#define TASK_HPP

#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

#include "memoryargs.hpp"
#include "reflect.hpp"

namespace eventfall::node {

using namespace boost::hana::literals;

class NodeIO {
private:
  std::shared_ptr<eventfall::memory::MemoryArgs> args;
  std::vector<std::size_t> indices;

public:
  NodeIO(std::shared_ptr<eventfall::memory::MemoryArgs> const &args,
         std::vector<std::size_t> const &indices)
      : args(args), indices(indices) {}

  bool empty() const { return (args == nullptr) || (indices.empty()); }

  decltype(auto) gen_args_vector() const { return args->get_ptr_args(indices); }

  template <class T> decltype(auto) get_arg(std::size_t const &index) const {
    return args->get_arg<T>(indices[index]);
  }
};

struct task_inputs {
  containers::ConcurrentMap<std::string_view, std::size_t> search_table;
  containers::ConcurrentVector<std::reference_wrapper<NodeIO const>> inputs;
};

class Task {
  std::string obj_name;
  struct {
    std::atomic<std::size_t> default_id{0};
    std::vector<std::shared_ptr<memory::MemoryArgs>> args;
    std::vector<std::string> args_name;
  } mem_table;

public:
  OUT_METAINFO(Task)
  Task(std::vector<std::pair<std::shared_ptr<memory::MemoryArgs>, std::string>>
           &&,
       std::string &&obj_name = "");

  constexpr static decltype(auto) classname() { return "Task"_s; }
  virtual std::string_view runtime_classname() { return "Task"; }
  virtual std::string_view name() { return obj_name; }

  virtual NodeIO setup_io(task_inputs const &inputs) = 0;
  virtual void execute() = 0;

  std::shared_ptr<memory::MemoryArgs> get_args(std::size_t const &);
  std::string_view get_args_name(std::size_t const &);
  std::optional<std::size_t> find_by_name(std::string_view const &);

  std::size_t get_default_id();
  void set_default_id(std::size_t const &);

  virtual std::size_t allocated_memory() const = 0;
  virtual ~Task();
};

} // namespace eventfall::node

METAINFO(eventfall::node::Task)
REFLECT_STATIC_MTD(classname)
REFLECT_OBJ_MTD(runtime_classname)
REFLECT_OBJ_MTD(name)
REFLECT_OBJ_MTD(setup_io, eventfall::node::task_inputs const &)
REFLECT_OBJ_MTD(execute)
REFLECT_OBJ_MTD(get_args, std::size_t const &)
REFLECT_OBJ_MTD(get_args_name, std::size_t const &)
REFLECT_OBJ_MTD(find_by_name, std::string_view const &)
REFLECT_OBJ_MTD(get_default_id)
REFLECT_OBJ_MTD(set_default_id, std::size_t const &)
REFLECT_CONST_OBJ_MTD(allocated_memory)
END_METAINFO

#endif // INODE_HPP
