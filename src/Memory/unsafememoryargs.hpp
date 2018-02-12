#ifndef UNSAFEMEMORYARGS_HPP
#define UNSAFEMEMORYARGS_HPP

#include <functional>
#include <unordered_map>

#include "memoryargs.hpp"

namespace eventfall::memory {

class UnsafeMemoryArgs : public MemoryArgs {

  /**
   * @brief mem_args hashmap of memory pointers
   */
  std::unordered_map<std::size_t, Argument> mem_args;

  virtual bool add_arg_impl(std::size_t const &, std::size_t const &,
                            std::function<void(void *)> &&) override;
  virtual void *get_arg_impl(std::size_t const &) override;
  virtual void set_arg_impl(std::size_t const &,
                            std::function<void(void *)> &&) override;

public:
  // MemoryArgs
  virtual bool is_threadsafe() const override;
  virtual std::string_view name() const override;

  using iterator_type = std::unordered_map<std::size_t, Argument>::iterator;

  virtual void erase_arg(const std::size_t &index) override;

  virtual void clear() override;

  virtual MemoryArgs::args_type
  get_ptr_args(std::vector<std::size_t> const &indices) override;

  virtual MemoryArgs::args_type get_ptr_args() override;

  virtual ~UnsafeMemoryArgs() override;

  // Reflect
  OUT_METAINFO(UnsafeMemoryArgs)
};

} // namespace eventfall::memory

METAINFO(eventfall::memory::UnsafeMemoryArgs, eventfall::memory::MemoryArgs)
REFLECT_CONST_OBJ_MTD(is_threadsafe)
REFLECT_CONST_OBJ_MTD(name)
END_METAINFO

#endif // MEMORYARGS_HPP
