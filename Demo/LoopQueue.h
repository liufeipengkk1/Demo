#ifndef UTIL_LOOPQUEUE_H
#define UTIL_LOOPQUEUE_H

#include <iostream>
#include <functional>

#include "optixcore_base_define.h"
using namespace std;

#define CAPACITY = 1000;
template <class T> 
class LoopQueue{
public:
	typedef function<bool(const T&, const T&)> SortFunc;

	LoopQueue() :
		_front(0),
		_end(0)
		{}
	~LoopQueue(){}

	bool empty() const {
		return _front == _end;
	}

	const T& front() const {
		Assert(_front != _end);
		return _queue[_front];
	}

	T& front() {
		Assert(_front != _end);
		return _queue[_front];
	}

	const T& back() const {
		return _queue[_end];
	}

	T& back() {
		Assert(_front != _end);
		return _queue[_end];
	}

	unsigned int size() const {
		return (_end - _front + CAPACITY) % CAPACITY;
	}

	bool pop_front() {
		Assert(_front != _end);
		_front = inc[_front];
		return true;
	}

	bool push_back(const T& val) {
		Assert(_front != (_end + 1) % CAPACITY);
		_queue[_end] = val;
		_end = inc[_end];
		return true;
	}

	void sort(const SortFuc& cmp) {
		if (size() < 1) return;
		for (unsigned int i = _front; i != _end;) {
			for (unsigned int p = (i + 1) % CAPACITY; p != _end;) {
				if (!cmp(_queue[i], _queue[p])) {
					T temp = _queue[i];
					_queue[i] = _queue[p];
					_queue[p] = temp;
				}
				p = inc[p];
			}
			i = inc[i];
		}
	}

	void quickSort(const SortFuc& cmp) {
		if (size() < 1) return;
		quickSort(_front, dec[_end], cmp);
	}

	inline T& operator[] (unsigned int idx) {
		return _queue[(_front + idx) % CAPACITY];
	}

	inline const T& operator[](unsigned int idx) const {
		return  _queue[(_front + idx) % CAPACITY];
	}
private:
	unsigned int inc(unsigned int iter) {
		++iter;
		return iter % CAPACITY;
	}

	unsigned int dec(unsigned int iter) {
		--iter;
		return iter != 0 ? --iter : CAPACITY - 1;
	}

	unsigned int partition(unsigned int low, unsigned int high,
		const SortFunc & cmpFunc){
		T privotKey = _queue[low];
		while (low != high) {
			while (low != high && cmpFunc(privotKey, _queue[high]))
			{
				high = dec[high];
			}
			
			while (low != high && cmpFunc(_queue[low], privotKey)) {
				low = inc[low];
			}

			if (low != high) {
				std::swap(_queue[low], _queue[high]);
			}
		}

		return low;
	}

	void quickSort(unsigned int low, unsigned int high,
		const SortFunc & cmpFunc){
		if (low != high) {
			unsigned int privotLoc = partition(low, high, cmpFunc);
			if (low != privotLoc) {
				quickSort(low, dec[privotLoc], cmpFunc);
			}
			if (high != privotLoc)
			{
				quickSort(inc[privotLoc], high, cmpFunc);
			}
		}
	}

private:
	T _queue[CAPACITY];
	unsigned int _front;
	unsigned int _end;
};

#endif
