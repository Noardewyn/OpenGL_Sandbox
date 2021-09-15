#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <spdlog/spdlog.h>

namespace tools {

class Logger {
public:
  static const char* default_log_pattern;

  static void Init(spdlog::level::level_enum log_level, const std::string& pattern = default_log_pattern);
  static std::shared_ptr<spdlog::logger>& core();
  static std::shared_ptr<spdlog::logger>& client();

private:
  std::string _log_file;

  static std::shared_ptr<spdlog::logger> _core_logger;
  static std::shared_ptr<spdlog::logger> _client_logger;
};

void CheckGLError(const char* func, const char* file, int line);

} // namespace tools

#define LOG_CORE_TRACE(...) ::tools::Logger::core()->trace(__VA_ARGS__)
#define LOG_CORE_DEBUG(...) ::tools::Logger::core()->debug(__VA_ARGS__)
#define LOG_CORE_INFO(...) ::tools::Logger::core()->info(__VA_ARGS__)
#define LOG_CORE_WARN(...) ::tools::Logger::core()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...) ::tools::Logger::core()->error(__VA_ARGS__)
#define LOG_CORE_CRITICAL(...) ::tools::Logger::core()->critical(__VA_ARGS__)
#define LOG_CORE_FATAL(...) ::tools::Logger::core()->fatal(__VA_ARGS__)

#define LOG_APP_TRACE(...) ::tools::Logger::client()->trace(__VA_ARGS__)
#define LOG_APP_DEBUG(...) ::tools::Logger::client()->debug(__VA_ARGS__)
#define LOG_APP_INFO(...) ::tools::Logger::client()->info(__VA_ARGS__)
#define LOG_APP_WARN(...) ::tools::Logger::client()->warn(__VA_ARGS__)
#define LOG_APP_ERROR(...) ::tools::Logger::client()->error(__VA_ARGS__)
#define LOG_APP_CRITICAL(...) ::tools::Logger::client()->critical(__VA_ARGS__)
#define LOG_APP_FATAL(...) ::tools::Logger::client()->fatal(__VA_ARGS__)

#define CHECK_OPENGL_ERROR() ::tools::CheckGLError(__FUNCTION__, __FILE__, __LINE__)

#endif // __LOGGER_H__
