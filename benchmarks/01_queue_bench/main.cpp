#include <QMutex>
#include <QQueue>
#include <QtTest/QtTest>
#include <mutex>
#include <queue>
#include <thread>

#include "containers.hpp"

class QueueBench : public QObject {
  Q_OBJECT
  const unsigned N = 10000000;

private slots:
  void enqueue_data();
  void enqueue();
  // void reading();
  // void writing_reading();
};

void QueueBench::enqueue_data() {
  QTest::addColumn<int>("conc_cont_comp");
  QTest::newRow("std::queue") << 0;
  QTest::newRow("QQueue") << 1;
  QTest::newRow("ConcurrentQueue") << 2;
}

void QueueBench::enqueue() {
  QFETCH(int, conc_cont_comp);

  if (conc_cont_comp == 0) {
    std::mutex mut;
    std::queue<std::string> queue1;
    QBENCHMARK {
      std::vector<std::thread> workers;
      for (unsigned i = 0u; i < std::thread::hardware_concurrency(); ++i) {
        workers.emplace_back([&mut, &queue1, this] {
          for (std::size_t i = 0u; i < N; ++i) {
            mut.lock();
            queue1.emplace(std::to_string(i));
            mut.unlock();
          }
        });
      }
      for (auto &&worker : workers) {
        worker.join();
      }
      workers.clear();
      for (unsigned i = 0u; i < std::thread::hardware_concurrency(); ++i) {
        workers.emplace_back([&mut, &queue1, this] {
          for (std::size_t i = 0u; i < N; ++i) {
            mut.lock();
            queue1.pop();
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
    QQueue<QString> queue2;
    QBENCHMARK {
      std::vector<std::thread> workers;
      for (unsigned i = 0u; i < std::thread::hardware_concurrency(); ++i) {
        workers.emplace_back([&mut2, &queue2, this] {
          for (unsigned i = 0u; i < N; ++i) {
            mut2.lock();
            queue2.enqueue(QString::number(i));
            mut2.unlock();
          }
        });
      }
      for (auto &&worker : workers) {
        worker.join();
      }
      workers.clear();
      for (unsigned i = 0u; i < std::thread::hardware_concurrency(); ++i) {
        workers.emplace_back([&mut2, &queue2, this] {
          for (unsigned i = 0u; i < N; ++i) {
            mut2.lock();
            queue2.dequeue();
            mut2.unlock();
          }
        });
      }
      for (auto &&worker : workers) {
        worker.join();
      }
    }
  } else if (conc_cont_comp == 2) {
    eventfall::containers::ConcurrentQueue<std::string> queue3;
    QBENCHMARK {
      std::vector<std::thread> workers;
      for (unsigned i = 0u; i < std::thread::hardware_concurrency(); ++i) {
        workers.emplace_back([&queue3, this] {
          for (unsigned i = 0u; i < N; ++i) {
            queue3.push(std::to_string(i));
          }
        });
      }
      for (auto &&worker : workers) {
        worker.join();
      }
      workers.clear();
      for (unsigned i = 0u; i < std::thread::hardware_concurrency(); ++i) {
        workers.emplace_back([&queue3, this] {
          std::string str;
          for (unsigned i = 0u; i < N; ++i) {
            queue3.try_pop(str);
          }
        });
      }
      for (auto &&worker : workers) {
        worker.join();
      }
    }
  }
}

QTEST_MAIN(QueueBench)
#include "main.moc"
