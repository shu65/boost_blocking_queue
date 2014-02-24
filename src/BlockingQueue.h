/*
 * BlockingQueue.h
 *
 *  Created on: 2014/02/24
 *      Author: shu
 */

#ifndef BLOCKINGQUEUE_H_
#define BLOCKINGQUEUE_H_

#include <queue>
#include <boost/thread.hpp>

namespace boost_blocking_queue {

template<class T, class Container = std::deque<T> >
class BlockingQueue {
public:

	BlockingQueue() {

	}
	virtual ~BlockingQueue() {

	}

	size_t GetSize() {
		boost::mutex::scoped_lock lock (mutex_);
		return queue_.size();
	}

	bool IsEmpty() {
		boost::mutex::scoped_lock lock (mutex_);
		return queue_.empty();
	}

	int Enqueue(const T &item) {
		boost::mutex::scoped_lock lock (mutex_);
		queue_.push(item);
		condition_.notify_one();
		return 0;
	}

	int Dequeue(T &item) {
		boost::mutex::scoped_lock lock (mutex_);
		if (queue_.empty()) {
			return 1;
		}
		item = queue_.front();
		queue_.pop();
		return 0;
	}

	int WaitAndDequeue(T &item) {
		boost::mutex::scoped_lock lock (mutex_);
		while (queue_.empty()) {
			condition_.wait(lock);
		}
		item = queue_.front();
		queue_.pop();
		return 0;
	}

private:
	boost::mutex mutex_;
	boost::condition_variable condition_;
	std::queue<T, Container> queue_;
};

}

#endif /* BLOCKINGQUEUE_H_ */
