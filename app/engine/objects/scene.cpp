#include "objects/scene.hpp"

namespace Nova {

void Scene::process(double) {

}

void Scene::enqueueRegistration(std::weak_ptr<Object> obj) {
    auto target = obj.lock();

    auto it = std::find_if(
        unregistered.begin(),
        unregistered.end(),
        [&](const std::weak_ptr<Object>& w) {
            return w.lock() == target;
        }
    ); // Chat GPT wrote this monstrosity

    if (it == unregistered.end()) {
        unregistered.push_back(obj);
    }
}

}