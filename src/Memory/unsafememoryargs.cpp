#include "unsafememoryargs.hpp"

namespace eventfall::memory {

bool UnsafeMemoryArgs::add_arg_impl(std::size_t const &index,
                                    std::size_t const &size,
                                    std::function<void(void *)> &&func) {
  /**
   * @todo Custom allocators
   */
  void *mem = ::operator new(size);
  auto iter = mem_args.emplace(index, mem);
  if (!iter.second)
    return iter.second;
  iter.first->second.destructor = [
    func{std::move(func)}, flag(std::ref(iter.first->second.init_flag)),
    iptr(iter.first->second.ptr)
  ] {
    if (flag.get() == true) {
      func(iptr);
      flag.get() = false;
    }
  };
  iter.first->second.deallocator = [
    destructor(std::ref(iter.first->second.destructor)),
    iptr(iter.first->second.ptr)
  ] {
    destructor();
    ::operator delete(iptr);
  };
  return iter.second;
}

void *UnsafeMemoryArgs::get_arg_impl(std::size_t const &index) {
  auto item = mem_args.find(index);
  if (item != mem_args.end())
    return item->second.ptr;
  else
    return nullptr;
}

void UnsafeMemoryArgs::set_arg_impl(std::size_t const &index,
                                    std::function<void(void *)> &&func) {
  auto item = mem_args.find(index);

  if (item == mem_args.end())
    return;
  if (item->second.init_flag) {
    item->second.destructor();
  }
  func(item->second.ptr);
  item->second.init_flag = true;
}

bool UnsafeMemoryArgs::is_threadsafe() const { return false; }

std::string_view UnsafeMemoryArgs::name() const {
  return boost::hana::to<char const *>(
      reflect::utils::class_name<decltype(*this)>());
}

MemoryArgs::args_type
UnsafeMemoryArgs::get_ptr_args(std::vector<std::size_t> const &indices) {
  std::vector<gc::guarded_ptr<MemoryArgs::Argument>> vec;
  for (auto &&i : indices) {
    vec.emplace_back(&mem_args.at(i));
  }
  return {std::move(vec)};
}

MemoryArgs::args_type UnsafeMemoryArgs::get_ptr_args() {
  std::vector<gc::guarded_ptr<MemoryArgs::Argument>> vec;
  for (auto i = mem_args.begin(); i != mem_args.end(); ++i)
    vec.emplace_back(&i->second);
  return {std::move(vec)};
}

void UnsafeMemoryArgs::erase_arg(std::size_t const &index) {
  auto iter = mem_args.find(index);
  if (iter == mem_args.end())
    return;
  iter->second.deallocator();
  mem_args.erase(index);
}

void UnsafeMemoryArgs::clear() {
  for (auto &&arg : mem_args)
    arg.second.deallocator();
  mem_args.clear();
}

UnsafeMemoryArgs::~UnsafeMemoryArgs() { clear(); }

} // namespace eventfall::memory
