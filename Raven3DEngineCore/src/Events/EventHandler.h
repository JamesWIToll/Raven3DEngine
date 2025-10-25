#include <utility>

//
// Created by wesley on 8/6/25.
//

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

namespace Raven3DEngineCore::Events {

    extern RAVEN_U_LONG GetNextEventListenerID();

    class Listener {
        std::vector<std::tuple<RAVEN_U_LONG,std::function<void(const Event&)>>> _listeners {};

    public:
        void RegisterListenerFunction(const std::function<void(const Event&)> &_listener, const RAVEN_U_LONG _listenerID) {
            _listeners.emplace_back(_listenerID, _listener);
        }

        void UnregisterListener(const RAVEN_U_LONG _listenerID) {
            erase_if(_listeners, [&](auto item) { auto [id, listener] = std::move(item); return id == _listenerID; } );
        }

        void Notify(const Event& event) const {
            for (const auto& [id, listener] : _listeners) {
                listener(event);
            }
        }
    };

    class EventHandler {
        std::map<EventType, Listener> _listenerRegistry {};

    public:

        void RegisterEventListener(const EventType type, const std::function<void(const Event&)> &_listener, const RAVEN_U_LONG _listenerID) {
            if (_listenerRegistry.contains(type)) {
                _listenerRegistry.at(type).RegisterListenerFunction(_listener, _listenerID);
            } else {
                auto listener = Listener{};
                listener.RegisterListenerFunction(_listener, _listenerID);
                _listenerRegistry.emplace(type, listener);
            }
        }

        void UnregisterEventListener(const EventType type, const RAVEN_U_LONG _listenerID) {
            if (_listenerRegistry.contains(type)) {
                _listenerRegistry.at(type).UnregisterListener(_listenerID);
            }
        }

        void UnregisterListenerAllEvents(const RAVEN_U_LONG _listenerID) {
            for (const auto &type: _listenerRegistry | std::views::keys) {
                _listenerRegistry.at(type).UnregisterListener(_listenerID);
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
