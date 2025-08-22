//
// Created by wesley on 8/6/25.
//

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

namespace Raven3DEngineCore::Events {



    class Listener {
        std::vector<std::function<void(const Event&)>> _listeners {};

    public:
        void RegisterListenerFunction(const std::function<void(const Event&)> &_listener) {
            _listeners.push_back(_listener);
        }

        void Notify(const Event& event) const {
            for (const auto& listener : _listeners) {
                listener(event);
            }
        }
    };

    class EventHandler {
        std::map<EventType, Listener> _listenerRegistry {};

    public:

        void RegisterEventListener(const EventType type, const std::function<void(const Event&)> &_listener) {
            if (_listenerRegistry.contains(type)) {
                _listenerRegistry.at(type).RegisterListenerFunction(_listener);
            } else {
                auto listener = Listener{};
                listener.RegisterListenerFunction(_listener);
                _listenerRegistry.emplace(type, listener);
            }
        }

        void Notify(const Event& event) const {
            const auto type = event.getEventType();
            event.logEvent();

            if (!_listenerRegistry.contains(type)) { return; }

            const auto listener = _listenerRegistry.at(type);
            listener.Notify(event);
        }

    };

    class EventNotifier {
    protected:
        EventHandler *_eventHandler {nullptr};
    public:
        virtual ~EventNotifier() = default;

        virtual void SetEventHandler(EventHandler *eventHandler) {
            _eventHandler = eventHandler;
        }

        virtual EventHandler *GetEventHandler() const {
            return _eventHandler;
        }
    };

}

#endif //EVENTHANDLER_H
