#include "functionnode.hpp"
#include "nodemanager.hpp"
#include "unsafememoryargs.hpp"

#include <QtTest/QtTest>

class FunctionNodeTester : public QObject {
  Q_OBJECT
  eventfall::node::NodeManager manager;
private slots:
  void initTestCase();
  void run_task();
  void task_chain();
  void input_multiple_tasks();
  void placement_test();
};

decltype(auto) gen_private_memory() {
  return std::make_pair(std::shared_ptr<eventfall::memory::MemoryArgs>(
                            new eventfall::memory::UnsafeMemoryArgs),
                        "private memory");
}

void FunctionNodeTester::initTestCase() {
  manager.add_node(
      eventfall::node::make_func_node_base_ptr(
          [](eventfall::node::task_inputs const &,
             eventfall::node::Task *ptr) -> eventfall::node::NodeIO {
            auto mem =
                ptr->get_args(ptr->find_by_name("private memory").value());
            mem->add_arg<int>(0);
            mem->set_arg<int>(0, 100);
            return {mem, {0}};
          },
          [](eventfall::node::Task *ptr) {
            auto mem =
                ptr->get_args(ptr->find_by_name("private memory").value());
            mem->set_arg<int>(0, 200);
          }),
      "task 1");
  manager.add_node(
      eventfall::node::make_func_node_base_ptr(
          [](eventfall::node::task_inputs const &io,
             eventfall::node::Task *ptr) -> eventfall::node::NodeIO {
            auto arg = io.inputs[0].get().get_arg<int>(0);
            ptr->set_default_id(ptr->find_by_name("private memory").value());
            auto mem = ptr->get_args(ptr->get_default_id());
            mem->add_arg<int>(0);
            mem->add_arg<std::string>(1);
            mem->set_arg<int>(0, *arg);
            return {mem, {1}};
          },
          [](eventfall::node::Task *ptr) {
            auto mem = ptr->get_args(ptr->get_default_id());
            mem->set_arg<std::string>(1, std::to_string(*mem->get_arg<int>(0)));
          }),
      "task 2");
  manager.add_node(
      eventfall::node::make_func_node_base_ptr(
          [](eventfall::node::task_inputs const &io,
             eventfall::node::Task *ptr) -> eventfall::node::NodeIO {
            auto arg = io.inputs[0].get().get_arg<std::atomic<int>>(0);
            ptr->set_default_id(ptr->find_by_name("private memory").value());
            auto mem = ptr->get_args(ptr->get_default_id());
            mem->add_arg<std::reference_wrapper<std::atomic<int>>>(0);
            mem->set_arg<std::reference_wrapper<std::atomic<int>>>(
                0, std::ref(*arg));
            return {nullptr, {}};
          },
          [](eventfall::node::Task *ptr) {
            auto mem = ptr->get_args(ptr->get_default_id());
            auto arg =
                mem->get_arg<std::reference_wrapper<std::atomic<int>>>(0);
            (*arg).get()++;
          }),
      "task 3");
}

void FunctionNodeTester::run_task() {
  eventfall::node::NodeIO empty_io(
      std::shared_ptr<eventfall::memory::MemoryArgs>(
          new eventfall::memory::UnsafeMemoryArgs),
      {});
  auto task = manager.create_task(manager.lookup_id("task 1").value(),
                                  {gen_private_memory()}, "task");
  QVERIFY(task != nullptr);
  auto io = task->setup_io({{}, {empty_io}});
  QCOMPARE(*io.gen_args_vector().get_arg<int>(0), 100);
  task->execute();
  QCOMPARE(*io.gen_args_vector().get_arg<int>(0), 200);
}

void FunctionNodeTester::task_chain() {
  eventfall::node::NodeIO empty_io(
      std::shared_ptr<eventfall::memory::MemoryArgs>(
          new eventfall::memory::UnsafeMemoryArgs),
      {});
  auto task1 = manager.create_task(manager.lookup_id("task 1").value(),
                                   {gen_private_memory()});
  auto task2 = manager.create_task(manager.lookup_id("task 2").value(),
                                   {gen_private_memory()});
  auto io1 = task1->setup_io({{}, {empty_io}});
  task1->execute();
  QCOMPARE(*io1.get_arg<int>(0), 200);
  auto io2 = task2->setup_io({{}, {io1}});
  task2->execute();
  QCOMPARE(*io2.get_arg<std::string>(0), {"200"});
}

void FunctionNodeTester::input_multiple_tasks() {
  auto task1 = manager.create_task(manager.lookup_id("task 3").value(),
                                   {gen_private_memory()});
  auto task2 = manager.create_task(manager.lookup_id("task 3").value(),
                                   {gen_private_memory()});
  auto task3 = manager.create_task(manager.lookup_id("task 3").value(),
                                   {gen_private_memory()});
  std::shared_ptr<eventfall::memory::MemoryArgs> mem(
      new eventfall::memory::UnsafeMemoryArgs);
  mem->add_arg<std::atomic<int>>(0);
  mem->set_arg<std::atomic<int>>(0, 0);
  eventfall::node::NodeIO io{std::move(mem), {0}};
  eventfall::node::task_inputs inputs{{}, {io}};
  task1->setup_io(inputs);
  task2->setup_io(inputs);
  task3->setup_io(inputs);
  task1->execute();
  task2->execute();
  task3->execute();
  QCOMPARE(*inputs.inputs[0].get().get_arg<std::atomic<int>>(0), 3);
}

void FunctionNodeTester::placement_test() {
  eventfall::node::NodeIO empty_io(
      std::shared_ptr<eventfall::memory::MemoryArgs>(
          new eventfall::memory::UnsafeMemoryArgs),
      {});
  std::unique_ptr<eventfall::node::Task> task = manager.create_task(
      manager.lookup_id("task 1").value(), {gen_private_memory()});
  auto io1 = task->setup_io({{}, {empty_io}});
  task->execute();
  QCOMPARE(*io1.get_arg<int>(0), 200);
  task = manager.create_task(manager.lookup_id("task 2").value(),
                             {gen_private_memory()}, std::move(task));
  auto io2 = task->setup_io({{}, {io1}});
  task->execute();
  QCOMPARE(*io2.get_arg<std::string>(0), {"200"});
}

QTEST_MAIN(FunctionNodeTester)
#include "main.moc"
