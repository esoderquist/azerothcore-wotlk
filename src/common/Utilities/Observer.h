#ifndef OBSERVER_H
#define OBSERVER_H

#include <mutex>
#include <condition_variable>

template <typename T>
class Observer
{
    public:
        void Update(const T& data)
        {
            std::lock_guard<std::mutex> lock(_eventMutex);
            _lastEvent = data;
            _eventAvailable = true;
            _eventCondition.notify_one();
        }

        T WaitNextEvent(std::chrono::milliseconds timeout)
        {
            std::unique_lock<std::mutex> lock(_eventMutex);

            if (_eventCondition.wait_for(lock, timeout, [this] { return _eventAvailable; }))
            {
                T event = _lastEvent;
                _eventAvailable = false;
                return event;
            }

            return -1; // Timeout
        }

    private:
        std::mutex _eventMutex;
        std::condition_variable _eventCondition;
        T _lastEvent;
        bool _eventAvailable = false;
};

#endif