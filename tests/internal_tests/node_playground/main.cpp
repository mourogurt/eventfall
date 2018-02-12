#include <iostream>

#include "safememoryargs.hpp"
#include "unsafememoryargs.hpp"

#include "functionnode.hpp"

#include "nodemanager.hpp"

#include "singleton.hpp"

#include "tasklogic.hpp"

#include <queue>

using namespace eventfall;

// Register io-event
// Register start-event

// enum class NodeLogic { RUN_AFTER, RUN_PRE, RUN_FOREACH, RUN_SINGLE, RUN_FORCE
// };

/*

#if (ATOMIC_LLONG_LOCK_FREE == 2)
  std::vector<std::atomic<std::bitset<sizeof(long long)>>> atomic_vec(1);
#elif (ATOMIC_LONG_LOCK_FREE == 2)
  std::vector<std::atomic<std::bitset<sizeof(long)>>> atomic_vec(1);
#elif (ATOMIC_INT_LOCK_FREE == 2)
  std::vector<std::atomic<std::bitset<sizeof(int)>>> atomic_vec(1);
#else
#warning "operations might be slow"
  std::vector<std::vector<std::atomic<bool>>> atomic_vec(1);
#endif
  std::cout << atomic_vec[0].is_always_lock_free << " "
            << atomic_vec[0].is_lock_free() << std::endl;
*/

/*class SimpleScheduler {
  containers::ConcurrentVector<node::task_logic> tasks;
  containers::ConcurrentQueue<containers::ConcurrentVector<node::task_logic>>
      childs_queue;
  std::atomic<std::size_t> buffer_index{0};

public:
  // NodePackage ()
  std::optional<std::size_t> add_task(node::task_logic &&);
  containers::ConcurrentVector<std::reference_wrapper<node::task_logic>>
  get_runnable(std::size_t const &);
};*/

class NodePackage {
public:
};

decltype(auto) make_init_node() {
  return node::make_func_node_base_ptr(
      [](eventfall::node::task_inputs const &,
         eventfall::node::Task *ptr) -> node::NodeIO {
        auto private_memory =
            ptr->get_args(ptr->find_by_name("private memory").value());
        private_memory->add_arg<std::size_t>(0);
        return {private_memory, {0}};
      },
      [](node::Task *ptr) {
        auto private_memory =
            ptr->get_args(ptr->find_by_name("private memory").value());
        private_memory->set_arg<std::size_t>(0, 100);
      });
}

/*decltype(auto) make_inplace_node() {
  return node::make_func_node_base_ptr();
}*/

decltype(auto) make_node() {
  return node::make_func_node_base_ptr(
      [](eventfall::node::task_inputs const &input,
         eventfall::node::Task *ptr) -> node::NodeIO {
        std::cout << "Inputs:\n";
        for (auto &&item : input.search_table) {
          std::cout << "Name: " << item.first << "\tIndex: " << item.second
                    << std::endl;
        }
        std::cout << std::endl;
        auto inp_vec = input.inputs[0].get().gen_args_vector();
        auto id = inp_vec.get_arg<std::size_t>(0);
        auto private_memory =
            ptr->get_args(ptr->find_by_name("private memory").value());
        private_memory->add_arg<std::size_t>(0);
        private_memory->set_arg<std::size_t>(0, *id);
        return {private_memory, {0}};
      },
      [](node::Task *ptr) {
        auto private_memory =
            ptr->get_args(ptr->find_by_name("private memory").value());
        std::cout << "Class name of task: " << ptr->runtime_classname()
                  << std::endl
                  << "Task name: " << ptr->name() << std::endl
                  << "id: " << *private_memory->get_arg<std::size_t>(0)
                  << std::endl
                  << std::endl;
        private_memory->set_arg<std::size_t>(
            0, *private_memory->get_arg<std::size_t>(0) + 100);
      });
}

//Генерация не потокобезопастной приватной памяти
decltype(auto) private_memory() {
  return std::make_pair(
      std::shared_ptr<memory::MemoryArgs>(new memory::UnsafeMemoryArgs),
      "private memory");
}

//Ссылка на текущую задачу
using task_ref = std::reference_wrapper<node::task_logic>;

//Добавление нод в менеджер
void register_nodes();

//Главный цикл обработки задач
void tasks_loop(std::queue<containers::ConcurrentVector<task_ref>> &&);

int main() {
  register_nodes();
  auto manager = utils::Singleton<node::NodeManager>::get_ptr();
  //Находим первую ноду
  auto id = manager->lookup_id("init node").value();
  //Создаем первую задачу из первой ноды
  auto task = node::task_logic(
      manager->create_task(id, {private_memory()}, "init task"), {});
  //Находим вторую ноду
  id = manager->lookup_id("id node").value();
  //создаем две задачи из второй ноды
  auto task_child1 = node::task_logic(
      manager->create_task(id, {private_memory()}, "task 1"), {task});
  auto task_child2 =
      node::task_logic(manager->create_task(id, {private_memory()}, "task 2"),
                       {task_child1, task});
  //Создаем очередь выполнения задач
  std::queue<containers::ConcurrentVector<task_ref>> tasks;
  //Добавляем точку входа
  tasks.push({std::ref(task)});
  //Отправляем на выполнение
  tasks_loop(std::move(tasks));
  return 0;
}

void tasks_loop(std::queue<containers::ConcurrentVector<task_ref>> &&tasks) {
  //Текущий буффер задач
  containers::ConcurrentVector<task_ref> curr_tasks_buff;
  //Выполняем пока у нас есть задачи
  while (!tasks.empty()) {
    //Берем текущий буффер задач из очереди
    curr_tasks_buff = tasks.front();
    for (auto &&task : curr_tasks_buff) {
      //Если задачу можно выполнить и она не завершена, то выполняем её
      if ((task.get().runnable()) && (!task.get().finished()))
        task.get().execute();
      //Добавляем в очередь дочерние задачи
      tasks.push(task.get().get_childs());
    }
    //Сдвигаем очередь
    tasks.pop();
  }
}

void register_nodes() {
  //Менеджер нод: добавление, удаление, поиск нод, создание задач
  auto manager = utils::Singleton<node::NodeManager>::create_ptr();
  //Добавляем первую ноду (на вход 0 аргументов, на выход 1 (std::size_t))
  //Имя можно не указывать, тогда её нельзя будет найти в lookup таблице
  manager->add_node(make_init_node(), "init node").value();
  //Еще одна нода(1 арг. на вход, 1 на выход)
  manager->add_node(make_node(), "id node");
}
