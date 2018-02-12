#include <QHash>
#include <QMutex>
#include <QtTest/QtTest>
#include <mutex>
#include <thread>
#include <unordered_set>

#include "containers.hpp"

class SetBench : public QObject {
  Q_OBJECT
  const unsigned N = 10000000;

private slots:
  void writing_data();
  void writing();
  // void reading();
  // void writing_reading();
};

void SetBench::writing_data() {
  QTest::addColumn<int>("conc_cont_comp");
  QTest::newRow("std::unordered_set + std::mutex") << 0;
  QTest::newRow("QSet + QMutex") << 1;
  QTest::newRow("ConcurrentSet") << 2;
}

void SetBench::writing() {
  QFETCH(int, conc_cont_comp);

  if (conc_cont_comp == 0) {
    std::mutex mut;
    std::unordered_set<std::string> set;
    QBENCHMARK {
      std::vector<std::thread> workers;
      for (unsigned i = 0u; i < std::thread::hardware_concurrency(); ++i) {
        workers.emplace_back([&] {
          for (unsigned i = 0u; i < N; ++i) {
            mut.lock();
            set.insert(std::to_string(i));
            mut.unlock();
          }
        });
      }
      for (auto &&worker : workers) {
        worker.join();
      }
    }
  } else if (conc_cont_comp == 1) {
    QMutex mut2;
    QSet<QString> set2;
    QBENCHMARK {
      std::vector<std::thread> workers;
      for (unsigned i = 0u; i < std::thread::hardware_concurrency(); ++i) {
        workers.emplace_back([&] {
          for (unsigned i = 0u; i < N; ++i) {
            mut2.lock();
            set2.insert(QString::number(i));
            mut2.unlock();
          }
        });
      }
      for (auto &&worker : workers) {
        worker.join();
      }
    }
  } else if (conc_cont_comp == 2) {
    eventfall::containers::ConcurrentSet<std::string> set3;
    QBENCHMARK {
      std::vector<std::thread> workers;
      for (unsigned i = 0u; i < std::thread::hardware_concurrency(); ++i) {
        workers.emplace_back([&] {
          eventfall::ThreadGC thread;
          for (unsigned i = 0u; i < N; ++i) {
            set3.insert(std::to_string(i));
          }
        });
      }
      for (auto &&worker : workers) {
        worker.join();
      }
    }
  }
}

QTEST_MAIN(SetBench)
#include "main.moc"
