#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Render/Logger.h"

namespace tools {

  const char* Logger::default_log_pattern = "%^%T <%n> [%l] [%t] %v%$";

  std::shared_ptr<spdlog::logger> Logger::_core_logger;
  std::shared_ptr<spdlog::logger> Logger::_client_logger;

  void Logger::Init(spdlog::level::level_enum log_level, const std::string& pattern) {
    _core_logger = spdlog::stdout_color_mt("CORE");
    _client_logger = spdlog::stdout_color_mt("APP");

    spdlog::set_level(log_level);
    spdlog::set_pattern(pattern);

    _core_logger->set_level(log_level);
    _client_logger->set_level(log_level);
  }

  std::shared_ptr<spdlog::logger>& Logger::core() {
    return _core_logger;
  }

  std::shared_ptr<spdlog::logger>& Logger::client() {
    return _client_logger;
  }

} // namespace tools
