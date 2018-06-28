#ifndef VECTOR_H
#define VECTOR_H

#include <cassert>

template <typename T>
class Vector {
	T* data;
	size_t allocated;
	size_t used;
	const static size_t init_size = 15;
	const static size_t chop_size = 20;
public:
	Vector() : data(new T[init_size]), allocated(init_size), used(0) {
	}
	// Reserva espacio para count elementos y les asigna el valor value:
	//
	Vector(size_t count, T const & value = T{}) :
		data(new T[count]{value}),
		allocated(count),
		used(count) 
	{
	}
	Vector(const Vector& v) : 
		data(new T[v.used]),
		allocated(v.used), 
		used(v.used) 
	{
		for (size_t i = 0; i < used; ++i)
			data[i] = (v.data)[i];
	}
	~Vector() {
		delete[] data;
		data = nullptr;
	}
	Vector& operator=(Vector const & v) {
		// Check for self-assignment:
		//
		if (this == &v)
			return *this;

		// Same size optimization:
		//
		if (used == v.used) {
			for (size_t i = 0; i < used; ++i)
				data[i] = (v.data)[i];
			return *this;
		}
		delete[] data;
		used = v.used;
		data = new T[used];
		allocated = used;
		for (size_t i = 0; i < used; ++i)
			data[i] = (v.data)[i];
		return *this;
	}
	// versión const:
	//
	const T& operator[](size_t position) const {
		if (position >= used)
			assert("Illegal position.");
		return data[position];
	}
	// versión no const:
	//
	T& operator[](size_t position) {
		if (position >= used)
			assert("Illegal position.");
		return data[position];
	}
	size_t size() const {
		return used;
	}
	size_t capacity() const {
		return allocated;
	}
	bool empty() const {
		return (bool) used;
	}
	// agrega un elemento al final:
	//
	void push_back(T const & value) {
		if (used == allocated)
			reserve(allocated + chop_size);
		data[used] = value;
		++used;
	}
	// llena el vector con count copias de valor value
	//
	void assign(size_t count, T const & value) {
		if (count > allocated)
			reserve(count);
		used = count;
		for (size_t i = 0; i < used; ++i)
			data[i] = value;
	}
	// reserva espacio para new_capacity elementos
	//
	void reserve(size_t new_capacity) {
		if (new_capacity > allocated) {
			T* new_data = new T[new_capacity];
			allocated = new_capacity;
			for (size_t i = 0; i < used; ++i)
				new_data[i] = data[i];
			delete[] data;
			data = new_data;
		}
	}
	void clear() {
		for (size_t i = 0; i < allocated; ++i)
			data[i] = 0;
		used = 0;
	}
};

#endif	//VECTOR_H
