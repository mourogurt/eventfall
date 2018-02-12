#include "safememoryargs.hpp"

namespace eventfall::memory {

// MemoryArgs

void *SafeMemoryArgs::get_arg_impl(std::size_t const &index) {
  auto item = mem_args.get(index);
  if (!item.empty())
    return item->second.ptr;
  else
    return nullptr;
}

bool SafeMemoryArgs::add_arg_impl(std::size_t const &index,
                                  std::size_t const &size,
                                  std::function<void(void *)> &&func) {
  /**
   * @todo custom allocators;
   */
  void *mem = ::operator new(size);
  bool res = mem_args.emplace(index, mem);
  mem_args.find(index, [func{std::move(func)}](auto &&item) {
    item.second.destructor = [
      flag(std::ref(item.second.init_flag)), iptr(item.second.ptr),
      func{std::move(func)}
    ] {
      if (flag.get() == true) {
        func(iptr);
        flag.get() = false;
      }
    };
    item.second.deallocator = [
      destructor(std::ref(item.second.destructor)), iptr(item.second.ptr)
    ] {
      destructor();
      ::operator delete(iptr);
    };
  });
  return res;
}

void SafeMemoryArgs::set_arg_impl(std::size_t const &index,
                                  std::function<void(void *)> &&func) {
  mem_args.find(index, [func{std::move(func)}](auto &&item) {
    if (item.second.init_flag) {
      item.second.destructor();
    }
    func(item.second.ptr);
    item.second.init_flag = true;
  });
}

bool SafeMemoryArgs::is_threadsafe() const { return true; }

std::string_view SafeMemoryArgs::name() const {
  return boost::hana::to<char const *>(
      reflect::utils::class_name<decltype(*this)>());
}

MemoryArgs::args_type
SafeMemoryArgs::get_ptr_args(std::vector<std::size_t> const &indices) {
  std::vector<gc::guarded_ptr<MemoryArgs::Argument>> vec;
  for (auto &&i : indices)
    vec.emplace_back(&mem_args.get(i)->second);
  return {std::move(vec)};
}

MemoryArgs::args_type SafeMemoryArgs::get_ptr_args() {
  std::vector<gc::guarded_ptr<MemoryArgs::Argument>> vec;
  for (std::size_t i = 0; i < mem_args.size(); ++i)
    vec.emplace_back(&mem_args.get(i)->second);
  return {std::move(vec)};
}

void SafeMemoryArgs::erase_arg(std::size_t const &index) {
  auto ptr = mem_args.get(index);
  if (ptr.empty())
    return;
  ptr->second.deallocator();
  mem_args.erase(index);
}

void SafeMemoryArgs::clear() {
  for (auto i = mem_args.begin(); i != mem_args.end(); ++i) {
    i->second.deallocator();
    mem_args.erase_at(i);
  }
}

SafeMemoryArgs::~SafeMemoryArgs() { clear(); }

} // namespace eventfall::memory
