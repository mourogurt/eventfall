#include "safememoryargs.hpp"
#include "unsafememoryargs.hpp"

#include <QtTest/QtTest>

class MemoryTester : public QObject {
  Q_OBJECT
  std::unique_ptr<eventfall::memory::MemoryArgs> unsafe_args{
      new eventfall::memory::UnsafeMemoryArgs()};
  std::unique_ptr<eventfall::memory::MemoryArgs> safe_args{
      new eventfall::memory::SafeMemoryArgs()};
private slots:
  void init();
  void argument();
  void get_ptr_args();
};

void MemoryTester::init() {
  unsafe_args->clear();
  safe_args->clear();
}

void MemoryTester::argument() {
  QVERIFY(unsafe_args->add_arg<std::size_t>(0));
  QVERIFY(unsafe_args->add_arg<std::string>(1));
  QVERIFY((unsafe_args->add_arg<std::array<int, 3>>(2)));
  QCOMPARE(unsafe_args->add_arg<bool>(0), false);
  unsafe_args->set_arg<std::size_t>(0, 1);
  unsafe_args->set_arg<std::string>(1, "Hello world");
  std::array<int, 3> arr{{1, 2, 3}};
  arr[0] = 1;
  arr[1] = 2;
  arr[2] = 3;
  unsafe_args->set_arg<std::array<int, 3>>(2, arr);
  QCOMPARE(*unsafe_args->get_arg<std::size_t>(0), 1lu);
  QCOMPARE(*unsafe_args->get_arg<std::string>(1), {"Hello world"});
  QCOMPARE((*unsafe_args->get_arg<std::array<int, 3>>(2)), arr);
  unsafe_args->erase_arg(0);
  QCOMPARE(unsafe_args->get_arg<void>(0), nullptr);

  QVERIFY(safe_args->add_arg<std::size_t>(0));
  QVERIFY(safe_args->add_arg<std::string>(1));
  QVERIFY((safe_args->add_arg<std::array<int, 3>>(2)));
  QCOMPARE(safe_args->add_arg<bool>(0), false);
  safe_args->set_arg<std::size_t>(0, 1);
  safe_args->set_arg<std::string>(1, "Hello world");
  safe_args->set_arg<std::array<int, 3>>(2, arr);
  QCOMPARE(*safe_args->get_arg<std::size_t>(0), 1lu);
  QCOMPARE(*safe_args->get_arg<std::string>(1), {"Hello world"});
  QCOMPARE((*safe_args->get_arg<std::array<int, 3>>(2)), arr);
  safe_args->erase_arg(0);
  QCOMPARE(safe_args->get_arg<void>(0), nullptr);
}

void MemoryTester::get_ptr_args() {
  unsafe_args->add_arg<std::size_t>(0);
  unsafe_args->add_arg<std::unique_ptr<int>>(1);
  unsafe_args->add_arg<std::string>(2);
  auto ptrs = unsafe_args->get_ptr_args({0, 2, 1});
  ptrs.set_arg<std::size_t>(0, 100);
  ptrs.set_arg<std::string>(1, "Test string");
  ptrs.set_arg<std::unique_ptr<int>>(2, new int[3]{1, 2, 3});
  QCOMPARE(*ptrs.get_arg<std::size_t>(0), 100ul);
  QCOMPARE(*ptrs.get_arg<std::string>(1), {"Test string"});
  QCOMPARE(unsafe_args->get_arg<std::unique_ptr<int>>(1)->get()[0], 1);
  QCOMPARE(unsafe_args->get_arg<std::unique_ptr<int>>(1)->get()[1], 2);
  QCOMPARE(unsafe_args->get_arg<std::unique_ptr<int>>(1)->get()[2], 3);

  safe_args->add_arg<std::size_t>(0);
  safe_args->add_arg<std::unique_ptr<int>>(1);
  safe_args->add_arg<std::string>(2);
  ptrs = safe_args->get_ptr_args({0, 2, 1});
  ptrs.set_arg<std::size_t>(0, 100);
  ptrs.set_arg<std::string>(1, "Test string");
  ptrs.set_arg<std::unique_ptr<int>>(2, new int[3]{1, 2, 3});
  QCOMPARE(*ptrs.get_arg<std::size_t>(0), 100ul);
  QCOMPARE(*ptrs.get_arg<std::string>(1), {"Test string"});
  QCOMPARE(safe_args->get_arg<std::unique_ptr<int>>(1)->get()[0], 1);
  QCOMPARE(safe_args->get_arg<std::unique_ptr<int>>(1)->get()[1], 2);
  QCOMPARE(safe_args->get_arg<std::unique_ptr<int>>(1)->get()[2], 3);
}

QTEST_MAIN(MemoryTester)
#include "main.moc"
