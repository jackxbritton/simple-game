#ifndef POOL_H
#define POOL_H

#include <stdlib.h>
#include <assert.h>

template <typename T>
struct Pool {

	int capacity;
	T *buffer;
	int count;
	int *available; // Stack of indexes to free slots.

	Pool(int cap) {
		capacity = cap;
		buffer = new T[capacity];
		count = 0;
		available = new int[capacity];
		for (int i = 0; i < capacity; i++) available[i] = capacity-1-i;
	}

	~Pool() {
		delete[] buffer;
		delete[] available;
	}

	T *alloc() {
		if (count == capacity) return NULL;
		count++;
		return &buffer[available[capacity-count]];
	}

	void free(T *t) {
		int index = t - buffer;
		assert(index >= 0 || index < capacity);
		available[capacity-count] = index;
		count--;
	}

};

#endif
