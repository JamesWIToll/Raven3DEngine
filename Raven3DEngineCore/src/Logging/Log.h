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
        RAVEN_U_SHORT _levelMask = static_cast<RAVEN_U_SHORT>(LogLevel::Fatal)   |
                             static_cast<RAVEN_U_SHORT>(LogLevel::Error)   |
                             static_cast<RAVEN_U_SHORT>(LogLevel::Warning) |
                             static_cast<RAVEN_U_SHORT>(LogLevel::Debug)   |
                             static_cast<RAVEN_U_SHORT>(LogLevel::Info);
    public:
        virtual ~ILogListener() = default;

        bool watchesLevel(const LogLevel level) const {
            return _levelMask & static_cast<RAVEN_U_SHORT>(level);
        }

        void addLevel(const LogLevel level) {
            _levelMask |= static_cast<RAVEN_U_SHORT>(level);
        }

        void removeLevel(const LogLevel level) {
            _levelMask &= ~static_cast<RAVEN_U_SHORT>(level);
        }

        void setLevelMask(const RAVEN_U_SHORT mask) {
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
        RAVEN_LONG _repeatCount = 0;
    public:
        ConsoleLog() = default;
        void SubmitMessage(const LogLevel level, const std::string &message) override {
            if (watchesLevel(level)) {
                std::string output = std::format("{}Log: [{}] ({}) : {}\x1b[0m", GetMessageColor(level), GetLevelName(level), RAVEN_GET_NOW(), message);
                if (_lastMessage == message && !_repeated) {
                    _repeated = true;
                    output = " [repeated]";
                    _repeatCount++;
                } else if (_lastMessage == message){
                    output = "";
                    _repeatCount++;
                } else if (_repeated){
                    output = std::format("<{} times>\n", _repeatCount) + output;
                    _repeated = false;
                    _repeatCount = 1;
                } else {
                    _repeated = false;
                    _repeatCount = 1;
                }
                _lastMessage = message;
                if (output.empty()) {
                    return;
                }
                switch (level) {
                    case LogLevel::Fatal:
                    case LogLevel::Error:
                        std::cerr << output << "\n";
                        break;
                    case LogLevel::Warning:
                    case LogLevel::Debug:
                    case LogLevel::Info:
                        std::cout << output << "\n";
                        break;
                }
            }
            std::flush(std::cout);
            std::flush(std::cerr);
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

    static void FlushLog() {
        std::cout.flush();
        std::cerr.flush();
    }
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

#define RAVEN_LOG_FLUSH() Raven3DEngineCore::Logging::FlushLog()

#endif //LOGHANDLER_H
