#ifndef TASKLOGIC_HPP
#define TASKLOGIC_HPP

#include "task.hpp"

#include <atomic>
#include <mutex>

namespace eventfall::node {

class task_logic {
  std::atomic<std::size_t> counter{0};
  volatile std::atomic<bool> finished_flag{false};
  std::unique_ptr<eventfall::node::Task> task;
  task_inputs parents_input;
  NodeIO result_io;
  containers::ConcurrentVector<std::reference_wrapper<task_logic>> childs;

  bool missed_flag{false};
  std::once_flag once_flag;

  void execute_internal();

public:
  task_logic(std::unique_ptr<Task> &&,
             std::vector<std::reference_wrapper<task_logic>> const &);

  bool finished() const;
  bool runnable() const;

  std::reference_wrapper<eventfall::node::NodeIO const> result() const;

  void execute();

  std::optional<std::size_t> add_child(task_logic &task_logic);

  containers::ConcurrentVector<std::reference_wrapper<task_logic>> get_childs();
};

} // namespace eventfall::node

#endif // TASKLOGIC_HPP
