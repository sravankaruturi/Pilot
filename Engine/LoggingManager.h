#pragma once
#include <string>
#include "GUIHelpers.h"

#define LOGGER pilot::LoggingManager::GetInstance()

namespace pilot {

	enum LogType {
		PE_LOG_INFO,
		PE_LOG_WARN,
		PE_LOG_ERROR
	};

	__forceinline std::string GetTypeString(LogType _type) {
		switch (_type) { 
		case PE_LOG_ERROR:
			return "[Error]: ";
		case PE_LOG_WARN:
			return "[Warning]: ";
		case PE_LOG_INFO:
			return "[Info]: ";
		default:
			return "[Info]: ";
		}
	}

	class LoggingManager {

		std::string log;

		ImGuiLog * imguiLogger = nullptr;

	public:

		static LoggingManager& GetInstance() {
			static LoggingManager instance;
			return instance;
		}

		void SetImGuiLogger(ImGuiLog * _logger) {
			imguiLogger = _logger;
		}

		void AddToLog(std::string _data, LogType _type = PE_LOG_INFO) {

			std::string str_to_log = GetTypeString(_type) + _data + std::string("\n");

			log += str_to_log;

			if (nullptr != imguiLogger) {
				imguiLogger->AddLog(str_to_log.c_str());
			}

			std::cout << str_to_log;
		}

		void ClearLog() {
			log.clear();
		}

		std::string GetLog() const {
			return log;
		}

		void Render(bool * _windowBool)
		{
			this->imguiLogger->Draw("Log", _windowBool);
		}

	};

}
