#include <sentinel/core/logger.h>
#include <sentinel/common.h>

logger::logger() {
	if (fileLoggingEnabled) {
		logFile.open(logFilePath, std::ios::app);
	}
}

logger::~logger() {
	if (logFile.is_open()) {
		logFile.close();
	}
}

void logger::info(const std::string& message)
{
	log("INFO", message);
}

void logger::warning(const std::string& message)
{
	log("WARN", message);
}

void logger::error(const std::string& message)
{
	log("ERR", message);
}

void logger::success(const std::string& message)
{
	log("SUC", message);
}

void logger::debug(const std::string& message)
{
	log("DBG", message);
}

void logger::fatal(const std::string& message)
{
	log("FATAL", message);
}

std::string logger::getColorForLevel(const std::string& level)
{
	static const std::unordered_map<std::string, std::string> levelColors = {
		{"INFO", INFO},
		{"WARN", WARNING},
		{"ERR", ERROR},
		{"DBG", SUCCESS},
		{"SUC", SUCCESS},
		{"FATAL", FATAL}
	};

	auto colorEntry  = levelColors.find(level);
	if (colorEntry  != levelColors.end()) {
		return colorEntry ->second; //value from the key-value pair :D
	}

	return RESET;
}

void logger::log(const std::string& level, const std::string& message)
{
	std::string colorCode = getColorForLevel(level);

	if (consoleLoggingEnabled) {
		std::cout << colorCode << "[" << level << "]" << RESET << " " << message << "\n";
	}

	if (fileLoggingEnabled) {
		if (!logFile.is_open()) { // this is called a lazy init apparently
			logFile.open(logFilePath, std::ios::app);
		}

		logFile << "[" << level << "] " << message << "\n";
		logFile.flush();
	}
}