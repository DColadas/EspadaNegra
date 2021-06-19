#include "IOHandler.hpp"

#include "Manager/MatchManager.hpp"

IOHandler::IOHandler(const std::shared_ptr<MatchManager>& manager_)
    : manager(manager_) {}

void IOHandler::join() {
    manager->join(*this);
}

void IOHandler::leave() {
    manager->leave(*this);
}
