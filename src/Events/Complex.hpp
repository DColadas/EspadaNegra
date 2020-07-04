#pragma once

#include <memory>
#include <vector>

#include "OutputEvent.hpp"

// Composite OutputEvent, formed by many others
class Complex final : public OutputEvent {
   private:
    std::vector<std::unique_ptr<OutputEvent>> events;

   protected:
    bool isEqual(const OutputEvent& obj) const final {
        if (!OutputEvent::isEqual(obj)) {
            return false;
        }
        const auto& v = static_cast<const Complex&>(obj);
        // Compare the vectors of events
        if (events.size() != v.events.size()) {
            return false;
        }
        for (std::size_t i = 0; i < events.size(); ++i) {
            if (*events[i] != *v.events[i]) {
                return false;
            }
        }
        return true;
    }

   public:
    Complex() : OutputEvent(Type::Complex) {}

    // Adds a new event
    void add(std::unique_ptr<OutputEvent> event) {
        events.push_back(std::move(event));
    }

    // Returns a view to the included events
    const std::vector<std::unique_ptr<OutputEvent>>& getEvents() const {
        return events;
    }
};