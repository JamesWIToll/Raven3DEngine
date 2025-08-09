//
// Created by wesley on 8/7/25.
//

#ifndef LOGHANDLER_H
#define LOGHANDLER_H

namespace Raven3DEngineCore::Logging {

    enum class LogLevel {
        Fatal = BITSHIFT_TO_POS(1),
        Error = BITSHIFT_TO_POS(2),
        Warning = BITSHIFT_TO_POS(3),
        Debug = BITSHIFT_TO_POS(4),
        Info = BITSHIFT_TO_POS(5),
    };

    static std::string GetLevelName(const LogLevel level) {
        switch (level) {
            case LogLevel::Fatal:
                return "Fatal";
            case LogLevel::Error:
                return "Error";
            case LogLevel::Warning:
                return "Warning";
            case LogLevel::Debug:
                return "Debug";
            case LogLevel::Info:
                return "Info";
        }
        return "";
    }

    struct LogMessage {
        LogLevel level;
        std::string message;
    };

    class ILogListener {
        u_short _levelMask = static_cast<u_short>(LogLevel::Fatal)   |
                             static_cast<u_short>(LogLevel::Error)   |
                             static_cast<u_short>(LogLevel::Warning) |
                             static_cast<u_short>(LogLevel::Debug)   |
                             static_cast<u_short>(LogLevel::Info);
    public:
        virtual ~ILogListener() = default;

        bool watchesLevel(const LogLevel level) const {
            return _levelMask & static_cast<u_short>(level);
        }

        void addLevel(const LogLevel level) {
            _levelMask |= static_cast<u_short>(level);
        }

        void removeLevel(const LogLevel level) {
            _levelMask &= ~static_cast<u_short>(level);
        }

        void setLevelMask(const u_short mask) {
            _levelMask = mask;
        }

        virtual void SubmitMessage(LogLevel level, const std::string& message) = 0;
    };

    class ConsoleLog final : public ILogListener {
        static std::string GetMessageColor(const LogLevel level) {
            switch (level) {
                case LogLevel::Fatal:
                    return "\x1b[31m\x1b[40m";
                case LogLevel::Error:
                    return "\033[31m";
                case LogLevel::Warning:
                    return "\033[33m";
                case LogLevel::Debug:
                    return "\033[36m";
                case LogLevel::Info:
                    return "\x1b[37m";
            }
            return "\x1b[37m";
        }

        std::string _lastMessage;
        bool _repeated;
        long int _repeatCount = 0;
    public:
        ConsoleLog() = default;
        void SubmitMessage(const LogLevel level, const std::string &message) override {
            auto dateTime = std::chrono::system_clock::now();
            if (watchesLevel(level)) {
                std::string output = std::format("\n{}Log: [{}] ({}) : {}\x1b[0m", GetMessageColor(level), GetLevelName(level), dateTime, message);
                const auto time = std::chrono::system_clock::now();
                if (_lastMessage == message && !_repeated) {
                    _repeated = true;
                    output = " [repeated]";
                    _repeatCount++;
                } else if (_lastMessage == message){
                    output = "";
                    _repeatCount++;
                } else if (_repeated){
                    output = std::format("<{} times>", _repeatCount) + output;
                    _repeated = false;
                    _repeatCount = 1;
                } else {
                    _repeated = false;
                    _repeatCount = 1;
                }
                _lastMessage = message;
                switch (level) {
                    case LogLevel::Fatal:
                    case LogLevel::Error:
                        std::cerr << output;
                        break;
                    case LogLevel::Warning:
                    case LogLevel::Debug:
                    case LogLevel::Info:
                        std::cout << output;
                        break;
                }
            }
        }
    };


    extern std::vector<ILogListener*> listeners;

    static void RegisterListener(ILogListener* listener) {
        listeners.push_back(listener);
    }

    static void SubmitMessage(const LogLevel level, const std::string& message) {
        for (const auto& listener : listeners) {
            listener->SubmitMessage(level, message);
        }
    }
    ;
}

#define RAVEN_REGISTER_LOG_LISTENER(listener) Raven3DEngineCore::Logging::RegisterListener(listener);

#define RAVEN_LOG_FATAL(format_str, ...)    Raven3DEngineCore::Logging::SubmitMessage(Raven3DEngineCore::Logging::LogLevel::Fatal,    std::format(format_str, ##__VA_ARGS__))
#define RAVEN_LOG_WARNING(format_str, ...)  Raven3DEngineCore::Logging::SubmitMessage(Raven3DEngineCore::Logging::LogLevel::Warning,  std::format(format_str, ##__VA_ARGS__))
#define RAVEN_LOG_ERROR(format_str, ...)    Raven3DEngineCore::Logging::SubmitMessage(Raven3DEngineCore::Logging::LogLevel::Error,    std::format(format_str, ##__VA_ARGS__))
#define RAVEN_LOG_INFO(format_str, ...)     Raven3DEngineCore::Logging::SubmitMessage(Raven3DEngineCore::Logging::LogLevel::Info,     std::format(format_str, ##__VA_ARGS__))

#ifdef RAVEN_CORE_DEBUG
#define RAVEN_LOG_DEBUG(format_str, ...)    Raven3DEngineCore::Logging::SubmitMessage(Raven3DEngineCore::Logging::LogLevel::Debug,    std::format(format_str, ##__VA_ARGS__))
#else
#define RAVEN_LOG_DEBUG(format_str, ...)
#endif


#endif //LOGHANDLER_H
