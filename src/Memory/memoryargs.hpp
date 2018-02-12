#ifndef MEMORYARGS_HPP
#define MEMORYARGS_HPP

#include <reflect.hpp>
#include <string_view>
#include <vector>

#include "containers.hpp"

namespace eventfall::memory {

class MemoryArgs {
protected:
  struct Argument {
    Argument(void *ptr) : ptr(ptr) {}
    Argument(Argument const &) = delete;
    Argument(Argument &&arg)
        : ptr(arg.ptr), init_flag(arg.init_flag.load()),
          destructor(arg.destructor), deallocator(arg.deallocator) {}
    Argument &operator=(Argument const &) = delete;
    void *ptr{nullptr};
    std::atomic<bool> init_flag{false};
    std::function<void()> destructor;
    std::function<void()> deallocator;

    // Reflection
    IN_METAINFO(Argument)
    REFLECT_OBJ_VAR(ptr)
    REFLECT_OBJ_VAR(init_flag)
    REFLECT_OBJ_VAR(destructor)
    REFLECT_OBJ_VAR(deallocator)
  };

  class ArgumentHelper {
    std::vector<gc::guarded_ptr<Argument>> args;

  public:
    ArgumentHelper(std::vector<gc::guarded_ptr<Argument>> &&args)
        : args(std::move(args)) {}
    template <class T> T *const get_arg(std::size_t const &index) const;

    template <class T, class... Args>
    void set_arg(std::size_t const &index, Args &&... params);
  };

  virtual bool add_arg_impl(std::size_t const &, std::size_t const &,
                            std::function<void(void *)> &&) = 0;
  virtual void *get_arg_impl(std::size_t const &) = 0;
  virtual void set_arg_impl(std::size_t const &,
                            std::function<void(void *)> &&) = 0;

public:
  using args_type = ArgumentHelper;

  template <class T> bool add_arg(std::size_t const &index) {
    return add_arg_impl(index, sizeof(T), [](void *iptr) {
      T *ptr = static_cast<T *>(iptr);
      ptr->~T();
    });
  }
  template <class T> T *const get_arg(std::size_t const &index) {
    return static_cast<T *const>(get_arg_impl(index));
  }
  template <class T, class... Args>
  void set_arg(std::size_t const &index, Args &&... args) {
    set_arg_impl(index, [&args...](void *ptr) {
      new (ptr) T(::std::forward<Args>(args)...);
    });
  }
  virtual void erase_arg(const std::size_t &index) = 0;
  virtual void clear() = 0;
  virtual bool is_threadsafe() const = 0;
  virtual args_type get_ptr_args(std::vector<std::size_t> const &indices) = 0;
  virtual args_type get_ptr_args() = 0;
  virtual std::string_view name() const;
  virtual ~MemoryArgs();

  // Reflection
  OUT_METAINFO(MemoryArgs)
};

template <class T>
T *const MemoryArgs::ArgumentHelper::get_arg(std::size_t const &index) const {
  if (!args.at(index).empty()) {
    return static_cast<T *const>(args.at(index)->ptr);
  } else
    return nullptr;
}

template <class T, class... Args>
void MemoryArgs::ArgumentHelper::set_arg(std::size_t const &index,
                                         Args &&... params) {
  if (!args.at(index).empty())
    args.at(index)->destructor();
  new (args.at(index)->ptr) T(::std::forward<Args>(params)...);
}

} // namespace eventfall::memory

METAINFO(eventfall::memory::MemoryArgs)
REFLECT_CONST_OBJ_MTD(name)
REFLECT_OBJ_MTD(erase_arg, std::size_t const &)
REFLECT_CONST_OBJ_MTD(is_threadsafe)
REFLECT_OBJ_MTD(get_ptr_args, std::vector<std::size_t> const &)
REFLECT_OBJ_MTD(get_ptr_args)
END_METAINFO

#endif // MEMORYARGS_HPP
