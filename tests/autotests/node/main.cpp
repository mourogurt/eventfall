#include "functionnode.hpp"
#include "nodemanager.hpp"

#include <QtTest/QtTest>

class NodeTester : public QObject {
  Q_OBJECT
  eventfall::node::NodeManager manager;
private slots:
  void init();
  void add_node();
  void remove_node();
  void find_node();
  void placement_test();
};

void NodeTester::init() { manager.clear(); }

void NodeTester::add_node() {
  auto id = manager.add_node(eventfall::node::make_func_node_base_ptr(
      [](auto &&, auto &&) -> eventfall::node::NodeIO {
        return {nullptr, {}};
      },
      [](auto) {}));
  QVERIFY(id.has_value());
  auto task = manager.create_task(id.value(), {});
  QVERIFY(task != nullptr);
}

void NodeTester::remove_node() {
  auto id = manager.add_node(eventfall::node::make_func_node_base_ptr(
      [](auto &&, auto &&) -> eventfall::node::NodeIO {
        return {nullptr, {}};
      },
      [](auto) {}));
  QVERIFY(id.has_value());
  QVERIFY(manager.remove_node(id.value()));
}

void NodeTester::find_node() {
  auto id =
      manager.add_node(eventfall::node::make_func_node_base_ptr(
                           [](auto &&, auto &&) -> eventfall::node::NodeIO {
                             return {nullptr, {}};
                           },
                           [](auto) {}),
                       "Test node");
  QCOMPARE(id, manager.lookup_id("Test node"));
  QVERIFY(manager.remove_node(id.value(), false));
  QCOMPARE(id, manager.lookup_id("Test node"));
  manager.shrink_lookup_table();
  QCOMPARE(manager.lookup_id("Test node"), std::nullopt);
  id = manager.add_node(eventfall::node::make_func_node_base_ptr(
                            [](auto &&, auto &&) -> eventfall::node::NodeIO {
                              return {nullptr, {}};
                            },
                            [](auto) {}),
                        "Test node");
  QVERIFY(manager.remove_node(id.value()));
  QCOMPARE(manager.lookup_id("Test node"), std::nullopt);
  id = manager.add_node(eventfall::node::make_func_node_base_ptr(
                            [](auto &&, auto &&) -> eventfall::node::NodeIO {
                              return {nullptr, {}};
                            },
                            [](auto) {}),
                        "Test node");
  manager.add_node(eventfall::node::make_func_node_base_ptr(
                       [](auto &&, auto &&) -> eventfall::node::NodeIO {
                         return {nullptr, {}};
                       },
                       [](auto) {}),
                   "Test node2");
  QVERIFY(manager.remove_node(id.value(), false));
  id = manager.add_node(eventfall::node::make_func_node_base_ptr(
                            [](auto &&, auto &&) -> eventfall::node::NodeIO {
                              return {nullptr, {}};
                            },
                            [](auto) {}),
                        "Test node");
  QCOMPARE(manager.lookup_id("Test node"), id);
}

void NodeTester::placement_test() {
  auto node = eventfall::node::make_func_node_base_ptr(
      [](auto &&, auto &&) -> eventfall::node::NodeIO {
        return {nullptr, {}};
      },
      [](auto) {});
  QVERIFY(node != nullptr);
  node = eventfall::node::make_func_node_base_ptr(
      std::move(node),
      [](auto &&, auto &&) -> eventfall::node::NodeIO {
        return {nullptr, {}};
      },
      [](auto) {});
  QVERIFY(node != nullptr);
  QVERIFY(node->gen_task({}, "").has_value());
}

QTEST_MAIN(NodeTester)
#include "main.moc"
