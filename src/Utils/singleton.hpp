#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <memory>
#include <mutex>

namespace eventfall::utils {

template <class T> class Singleton {
  static std::unique_ptr<T> ptr;
  static std::once_flag once;

public:
  template <class... Args> static decltype(auto) create_ptr(Args &&... args) {
    std::call_once(once,
                   [&](Args &&... args) {
                     Singleton<T>::ptr.reset(
                         new T(std::forward<Args>(args)...));
                   },
                   std::forward<Args>(args)...);
    return ptr.get();
  }
  static decltype(auto) get_ptr() { return ptr.get(); }
};

template <class T> std::unique_ptr<T> Singleton<T>::ptr = nullptr;
template <class T> std::once_flag Singleton<T>::once;

} // namespace eventfall::utils

#endif // SINGLETON_HPP
