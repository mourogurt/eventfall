#ifndef FUNCTIONTASK_HPP
#define FUNCTIONTASK_HPP

#include "task.hpp"
#include <functional>
#include <memory>

namespace eventfall::node {

template <class T, class K> class FunctionTask : public Task {
  T setup_func;
  K exec_func;

public:
  FunctionTask(
      std::vector<std::pair<std::shared_ptr<memory::MemoryArgs>, std::string>>
          &&,
      T &&, K &&, std::string &&obj_name = "");
  constexpr static decltype(auto) classname() { return "FunctionTask"_s; }
  virtual std::string_view runtime_classname() override {
    return "FunctionTask";
  }
  virtual NodeIO setup_io(const task_inputs &input) override;
  virtual void execute() override;

  virtual std::size_t allocated_memory() const override {
    return sizeof(FunctionTask<T, K>);
  }
  virtual ~FunctionTask() override;

  IN_METAINFO(FunctionTask, Task)
  REFLECT_STATIC_MTD(classname)
};

template <class T, class K>
FunctionTask<T, K>::FunctionTask(
    std::vector<std::pair<std::shared_ptr<memory::MemoryArgs>, std::string>>
        &&args,
    T &&setup_func, K &&exec_func, std::string &&obj_name)
    : Task(std::move(args), std::move(obj_name)),
      setup_func(std::move(setup_func)), exec_func(std::move(exec_func)) {}

template <class T, class K>
NodeIO FunctionTask<T, K>::setup_io(task_inputs const &inputs) {
  return std::invoke(setup_func, inputs, this);
}

template <class T, class K> void FunctionTask<T, K>::execute() {
  std::invoke(exec_func, this);
}

template <class T, class K> FunctionTask<T, K>::~FunctionTask() {}

template <class T, class K>
std::optional<std::reference_wrapper<Task>> make_func_task_base_ptr(
    std::vector<std::pair<std::shared_ptr<memory::MemoryArgs>, std::string>>
        &&args,
    T &&setup_func, K &&exec_func, std::string &&obj_name = "") {
  auto ptr = new FunctionTask<std::decay_t<T>, std::decay_t<K>>(
      std::move(args), std::move(setup_func), std::move(exec_func),
      std::move(obj_name));
  if (ptr != nullptr)
    return std::make_optional(std::reference_wrapper<Task>(*ptr));
  else
    return std::nullopt;
}

template <class T, class K>
std::optional<std::reference_wrapper<Task>> make_func_task_base_ptr(
    std::vector<std::pair<std::shared_ptr<memory::MemoryArgs>, std::string>>
        &&args,
    Task *task, T &&setup_func, K &&exec_func, std::string &&obj_name = "") {
  if (task->allocated_memory() < sizeof(FunctionTask<T, K>)) {
    return make_func_task_base_ptr(std::move(args), std::move(setup_func),
                                   std::move(exec_func), std::move(obj_name));
  } else {
    task->~Task();
    return std::make_optional(std::reference_wrapper<Task>(
        *(new (task) FunctionTask<std::decay_t<T>, std::decay_t<K>>(
            std::move(args), std::move(setup_func), std::move(exec_func),
            std::move(obj_name)))));
  }
}

} // namespace eventfall::node

#endif // FUNCTIONTASK_HPP
