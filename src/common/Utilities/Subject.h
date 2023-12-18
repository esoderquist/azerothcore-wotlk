#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>

#include "Observer.h"

template <typename T>
class Subject
{
    public:
        void Attach(Observer<T> *observer)
        {
            std::lock_guard<std::mutex> lock(_observerMutex);
            _observers.push_back(observer);
        }

        void Detach(Observer<T> *observer)
        {
            std::lock_guard<std::mutex> lock(_observerMutex);
            _observers.erase(std::remove(_observers.begin(), _observers.end(), observer));
        }

        void Notify(const T& data)
        {
            std::lock_guard<std::mutex> lock(_observerMutex);

            for (Observer<T>* observer : _observers)
            {
                observer->Update(data);
            }
        }

    private:
        std::vector<Observer<T>*> _observers;
        std::mutex _observerMutex;
};

#endif