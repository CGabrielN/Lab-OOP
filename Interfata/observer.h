#ifndef LABORATOR14_OBSERVER_H
#define LABORATOR14_OBSERVER_H

#include <vector>
#include <algorithm>

using std::vector;

/*
  Abstract class
  update method need to be implemented by observers
*/
class Observer {
public:
    virtual void update() = 0;
};

/*
  Observable or Subject - class that notify changes in its state
  Derive from this class if you want to provide notifications
*/
class Observable {
private:
    vector<Observer *> observers;

public:
    void addObserver(Observer *obs) {
        observers.push_back(obs);
    }

    void removeObserver(Observer *obs) {
        observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
    }

protected:
    void notify() {
        for (auto obs: observers) {
            obs->update();
        }
    }
};

#endif //LABORATOR14_OBSERVER_H
