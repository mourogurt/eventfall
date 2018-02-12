#include <QHash>
#include <QMutex>
#include <QtTest/QtTest>
#include <mutex>
#include <thread>
#include <unordered_map>

#include "containers.hpp"

class MapBench : public QObject {
  Q_OBJECT
  const unsigned N = 10000000;

private slots:
  void writing_data();
  void writing();
};

void MapBench::writing_data() {
  QTest::addColumn<int>("conc_cont_comp");
  QTest::newRow("std unordered_map") << 0;
  QTest::newRow("QHash+ QMutex") << 1;
  QTest::newRow("ConcurrentMap") << 2;
}

void MapBench::writing() {
  QFETCH(int, conc_cont_comp);

  if (conc_cont_comp == 0) {
    std::mutex mut;
    std::unordered_map<std::string, std::size_t> map;
    QBENCHMARK {
      std::vector<std::thread> workers;
      for (unsigned i = 0u; i < std::thread::hardware_concurrency(); ++i) {
        workers.emplace_back([&] {
          for (std::size_t i = 0u; i < N; ++i) {
            mut.lock();
            map.emplace(std::to_string(i), i);
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
    QHash<QString, std::size_t> map2;
    QBENCHMARK {
      std::vector<std::thread> workers;
      for (unsigned i = 0u; i < std::thread::hardware_concurrency(); ++i) {
        workers.emplace_back([&] {
          for (unsigned i = 0u; i < N; ++i) {
            mut2.lock();
            map2.insert(QString::number(i), i);
            mut2.unlock();
          }
        });
      }
      for (auto &&worker : workers) {
        worker.join();
      }
    }
  } else if (conc_cont_comp == 2) {
    eventfall::containers::ConcurrentMap<std::string, std::size_t> map4;
    QBENCHMARK {
      std::vector<std::thread> workers;
      for (unsigned i = 0u; i < std::thread::hardware_concurrency(); ++i) {
        workers.emplace_back([&] {
          eventfall::ThreadGC thread;
          for (unsigned i = 0u; i < N; ++i) {
            map4.emplace(std::to_string(i), i);
          }
        });
      }
      for (auto &&worker : workers) {
        worker.join();
      }
    }
  }
}

QTEST_MAIN(MapBench)
#include "main.moc"
