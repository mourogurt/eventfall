#include "task.hpp"

namespace eventfall::node {

Task::Task(std::vector<std::pair<std::shared_ptr<memory::MemoryArgs>,
                                           std::string>> &&mem_list,
           std::string &&obj_name)
    : obj_name(obj_name) {
  for (auto &&item : mem_list) {
    mem_table.args.push_back(item.first);
    if (!item.second.empty())
      mem_table.args_name.push_back(std::move(item.second));
  }
}

std::shared_ptr<memory::MemoryArgs> Task::get_args(std::size_t const &id) {
  return mem_table.args.at(id);
}

std::string_view Task::get_args_name(std::size_t const &id) {
  return mem_table.args_name.at(id);
}

std::optional<std::size_t> Task::find_by_name(std::string_view const &str) {
  std::size_t res = 0;
  for (auto &&item : mem_table.args_name) {
    if (item.size() == str.size())
      if (str == item)
        return std::make_optional(res);
    ++res;
  }
  return std::nullopt;
}

void Task::set_default_id(std::size_t const &id) {
  mem_table.default_id.exchange(id);
}

std::size_t Task::get_default_id() { return mem_table.default_id; }

Task::~Task() {}

} // namespace eventfall::node
