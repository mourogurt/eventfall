#ifndef COMMON_DEFINES_HPP
#define COMMON_DEFINES_HPP

#include "cds/gc/dhp.h"
#include "cds/init.h"

namespace eventfall {

typedef cds::gc::DHP gc;

class ThreadGC {
public:
  ThreadGC();
  ~ThreadGC();
};

class ContainerManager {
  gc current_gc = gc(1024);
  ThreadGC *main_thread;

public:
  ContainerManager();
  ~ContainerManager();
};

extern ContainerManager manager;

} // namespace eventfall

#endif // COMMON_DEFINES_HPP
