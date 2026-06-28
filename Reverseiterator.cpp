#include <iostream>

using namespace std;

class vec {
private:

	int* arr;
	int arr_size;
	int arr_capacity;

public:

	class iterator {
	private:
		int* ptr;
	public:
		iterator(int* p) {
			ptr = p;
		}

		bool operator==(iterator other) {
			return ptr == other.ptr;
		}

		bool operator!=(iterator other) {
			return ptr != other.ptr;
		}

		int& operator*() {
			return *ptr;
		}

		iterator operator+(int other) {
			return iterator(ptr + other);
		}

		void operator+=(int other) {
			ptr += other;
		}

		iterator operator++() {
			return iterator(ptr++);
		}

		iterator operator++(int) {
			return iterator(ptr++);
		}

	};

	class reverse_iterator {
	private:
		int* ptr;
	public:
		reverse_iterator(int* p) {
			ptr = p;
		}

		bool operator==(reverse_iterator other) {
			return ptr == other.ptr;
		}

		bool operator!=(reverse_iterator other) {
			return ptr != other.ptr;
		}

		int& operator*() {
			return *ptr;
		}

		int& operator[](int offset) {
			return *(ptr - offset);
		}

		reverse_iterator operator+(int other) {
			return reverse_iterator(ptr - other);
		}

		reverse_iterator operator-(int other) {
			return reverse_iterator(ptr + other);
		}

		// prefix ++
		reverse_iterator& operator++() {
			ptr--;
			return *this;
		}

		// postfix ++
		reverse_iterator operator++(int) {
			reverse_iterator temp = *this;
			ptr--;
			return temp;
		}

		// prefix --
		reverse_iterator& operator--() {
			ptr++;
			return *this;
		}

		// postfix --
		reverse_iterator operator--(int) {
			reverse_iterator temp = *this;
			ptr++;
			return temp;
		}
	};

	iterator begin() {
		return iterator(arr);
	}

	iterator end() {
		return iterator(arr + arr_size);
	}

	reverse_iterator rbegin() {
		return reverse_iterator(arr + arr_size - 1);
	}

	reverse_iterator rend() {
		return reverse_iterator(arr - 1);
	}

	vec() {
		arr = nullptr;
		arr_size = 0;
		arr_capacity = 0;
	}

	vec(int size) {
		if (size == 0) {
			arr = nullptr;
		}
		else {
			arr = new int[size];
		}
		arr_size = size;
		arr_capacity = size;
	}

	~vec() {
		delete[] arr;
	}

	int size() {
		return arr_size;
	}

	int capacity() {
		return arr_capacity;
	}

	void push_back(int x) {
		if (arr_size < arr_capacity) {
			arr[arr_size] = x;
			arr_size++;
		}
		else {
			if (arr_capacity == 0) {
				arr_capacity = 1;
			}
			else {
				arr_capacity *= 2;
			}
			int* new_arr = new int[arr_capacity];

			for (int i = 0; i < arr_size; i++) {
				new_arr[i] = arr[i];
			}

			new_arr[arr_size] = x;

			delete[] arr;
			arr = new_arr;
			arr_size++;

			cout << "Array resized to " << arr_capacity << endl;
		}
		cout << "Pushed back " << x << endl;
	}

	int pop_back() {
		if (arr_size == 0) {
			cout << "ERROR: cannot pop from empty vector!" << endl;
			return -1;
		}
		arr_size--;
		return arr[arr_size];
	}

	int& at(int idx) {
		return arr[idx];
	}

	int& operator[](int idx) {
		return arr[idx];
	}

};

int main() {

	vec v = vec(5);
	for (int i = 0; i < 5; i++) {
		v[i] = i + 1;  // v = {1, 2, 3, 4, 5}
	}

	cout << "Forward:" << endl;
	for (vec::iterator it = v.begin(); it != v.end(); it++) {
		cout << *it << endl;
	}

	cout << "Reverse:" << endl;
	for (vec::reverse_iterator it = v.rbegin(); it != v.rend(); it++) {
		cout << *it << endl;
	}

}