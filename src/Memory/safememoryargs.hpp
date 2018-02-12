#ifndef SafeMemoryArgs_HPP
#define SafeMemoryArgs_HPP

#include "containers.hpp"
#include "memoryargs.hpp"

namespace eventfall::memory {

class SafeMemoryArgs : public MemoryArgs {

  /**
   * @brief mem_args hashmap of memory pointers
   */
  containers::ConcurrentMap<std::size_t, Argument> mem_args;

  virtual void *get_arg_impl(std::size_t const &) override;
  virtual bool add_arg_impl(std::size_t const &, std::size_t const &,
                            std::function<void(void *)> &&) override;
  virtual void set_arg_impl(std::size_t const &,
                            std::function<void(void *)> &&) override;

public:
  // MemoryArgs
  virtual bool is_threadsafe() const override;
  virtual std::string_view name() const override;

  using iterator_type =
      containers::ConcurrentMap<std::size_t, Argument>::iterator;

  virtual MemoryArgs::args_type
  get_ptr_args(std::vector<std::size_t> const &indices) override;

  virtual MemoryArgs::args_type get_ptr_args() override;

  virtual void erase_arg(const std::size_t &index) override;

  virtual void clear() override;

  virtual ~SafeMemoryArgs() override;

  // Reflect
  OUT_METAINFO(SafeMemoryArgs)
};

} // namespace eventfall::memory

METAINFO(eventfall::memory::SafeMemoryArgs, eventfall::memory::MemoryArgs)
END_METAINFO

#endif // SafeMemoryArgs_HPP
