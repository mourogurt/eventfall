#include "tasklogic.hpp"

namespace eventfall::node {

task_logic::task_logic(
    std::unique_ptr<Task> &&task,
    std::vector<std::reference_wrapper<task_logic>> const &parents)
    : task(std::move(task)), result_io(nullptr, {}) {
  counter = parents.size();
  for (std::size_t i = 0; i < parents.size(); ++i) {
    if (!parents[i].get().task->name().empty())
      parents_input.search_table.emplace(parents[i].get().task->name(), i);
    parents_input.inputs.push_back(parents[i].get().result());
  }
  for (auto &&parent : parents) {
    if (parent.get().add_child(std::ref(*this)).has_value()) {
      missed_flag = true;
    } else
      counter--;
  }
}

bool task_logic::finished() const { return finished_flag; }
bool task_logic::runnable() const {
  return (counter == 0) && (task != nullptr);
}

std::reference_wrapper<eventfall::node::NodeIO const>
task_logic::result() const {
  return std::cref(result_io);
}

void task_logic::execute_internal() {

  result_io = task->setup_io(parents_input);
  task->execute();
  // TODO: Benchmark this
  finished_flag = true;
  for (auto &&child : childs)
    child.get().counter.store(
        child.get().counter.load(std::memory_order_relaxed) - 1,
        std::memory_order_relaxed);
}

void task_logic::execute() {
  if (!missed_flag) {
    execute_internal();
  } else {
    std::call_once(once_flag, &task_logic::execute_internal, this);
  }
}

std::optional<std::size_t> task_logic::add_child(task_logic &task_logic) {
  if (!finished_flag) {
    auto iter = childs.push_back(task_logic);
    if (!finished_flag) {
      if (iter != childs.end()) {
        return std::make_optional(iter - childs.begin());
      }
    }
  }
  return std::nullopt;
}

eventfall::containers::ConcurrentVector<std::reference_wrapper<task_logic>>
task_logic::get_childs() {
  return childs;
}

} // namespace eventfall::node
