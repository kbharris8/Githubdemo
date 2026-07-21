#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <stdexcept>

template <typename T>
class PriorityQueue {
private:
    std::vector<T> data;
    std::function<double(const T&)> priorityFn;

public:
    PriorityQueue(std::function<double(const T&)> fn)
        : priorityFn(fn) {}

  
    void push(const T& value) {
        data.push_back(value);
        std::sort(data.begin(), data.end(),
            [&](const T& a, const T& b) {
                return priorityFn(a) > priorityFn(b);
            });
    }

   
    T pop() {
        if (data.empty()) {
            throw std::runtime_error("PriorityQueue is empty");
        }
        T top = data.front();
        data.erase(data.begin());
        return top;
    }

    
    const T& top() const {
        if (data.empty()) {
            throw std::runtime_error("PriorityQueue is empty");
        }
        return data.front();
    }

    bool empty() const {
        return data.empty();
    }

    size_t size() const {
        return data.size();
    }
};




struct Job {
    std::string name;
    int urgency;
};

int main() {
  
    PriorityQueue<Job> pq([](const Job& j) {
        return j.urgency;
    });

    pq.push({"Clean room", 1});
    pq.push({"Finish homework", 5});
    pq.push({"Pay bills", 3});

    while (!pq.empty()) {
        Job j = pq.pop();
        std::cout << j.name << " (urgency " << j.urgency << ")\n";
    }

    return 0;
}
