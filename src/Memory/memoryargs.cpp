#include "memoryargs.hpp"

namespace eventfall::memory {

MemoryArgs::~MemoryArgs() {}

std::string_view MemoryArgs::name() const {
  return boost::hana::to<char const *>(
      reflect::utils::class_name<decltype(*this)>());
}

} // namespace eventfall::memory
