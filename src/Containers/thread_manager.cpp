#include "common_defines.hpp"

namespace eventfall {

ContainerManager manager;

ContainerManager::ContainerManager() {
  cds::Initialize();
  main_thread = new ThreadGC;
}

ContainerManager::~ContainerManager() {
  delete main_thread;
  cds::Terminate();
}

ThreadGC::ThreadGC() { cds::threading::Manager::attachThread(); }

ThreadGC::~ThreadGC() { cds::threading::Manager::detachThread(); }

} // namespace eventfall
