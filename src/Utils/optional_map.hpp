#ifndef OPTIONAL_MAP_HPP
#define OPTIONAL_MAP_HPP

#include <functional>
#include <optional>

namespace eventfall::utils {

template <class F, class... Args>
auto optional_map(F &&func, Args &&... args) -> std::enable_if_t<
    !std::is_void_v<std::invoke_result_t<F, decltype(args.value())...>>,
    std::optional<std::invoke_result_t<F, decltype(args.value())...>>> {
  if ((... && args.has_value())) {
    return std::make_optional(std::invoke(func, args.value()...));
  } else {
    return std::nullopt;
  }
}

template <class F, class... Args>
auto optional_map(F &&func, Args &&... args) -> std::enable_if_t<
    std::is_void_v<std::invoke_result_t<F, decltype(args.value())...>>,
    std::nullopt_t> {
  if ((... && args.has_value()))
    std::invoke(func, args.value()...);
  return std::nullopt;
}

} // namespace eventfall::utils

#endif // OPTIONAL_MAP_HPP
