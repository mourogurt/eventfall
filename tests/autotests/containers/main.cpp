#include "search_table.hpp"

#include "optional_map.hpp"

#include <QtTest/QtTest>

class ContainersTester : public QObject {
  Q_OBJECT
  eventfall::containers::search_table<int> table;
private slots:
  void init();
  void add_argument();
  void remove_argument();
  void find_name();
  void find_hash();
};

void ContainersTester::init() { table.clear(); }

void ContainersTester::add_argument() {
  int var = 0;
  QCOMPARE(eventfall::utils::optional_map(
               &eventfall::containers::search_table<int>::get_item,
               std::make_optional(std::ref(table)),
               table.add_item(std::make_pair(100, "test")))
               .value_or(std::ref(var)),
           100);
}

void ContainersTester::remove_argument() {
  QCOMPARE(eventfall::utils::optional_map(
               &eventfall::containers::search_table<int>::remove_item,
               std::make_optional(std::ref(table)),
               table.add_item(std::make_pair(100, "test")),
               std::make_optional(true))
               .value_or(false),
           true);
}

void ContainersTester::find_name() {
  QCOMPARE(eventfall::utils::optional_map(
               &eventfall::containers::search_table<int>::find_key,
               std::make_optional(std::ref(table)),
               table.add_item(std::make_pair(100, "test")))
               .value_or(""),
           "test");
  QCOMPARE(eventfall::utils::optional_map(
               &eventfall::containers::search_table<int>::remove_item,
               std::make_optional(std::ref(table)), table.find_hash("test"),
               std::make_optional(false))
               .value_or(false),
           true);
  QCOMPARE(eventfall::utils::optional_map(
               &eventfall::containers::search_table<int>::find_key,
               std::make_optional(std::ref(table)), table.find_hash("test"))
               .value_or(""),
           "test");
  table.shrink_table();
  QCOMPARE(eventfall::utils::optional_map(
               &eventfall::containers::search_table<int>::find_key,
               std::make_optional(std::ref(table)), table.find_hash("test"))
               .value_or(""),
           "");
}

void ContainersTester::find_hash() {
  QCOMPARE(table.add_item(std::make_pair(100, "test")),
           table.find_hash("test"));
}

QTEST_MAIN(ContainersTester)
#include "main.moc"
