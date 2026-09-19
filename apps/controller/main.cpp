#include <Windows.h>

#include <logger.h>

#include <iostream>
#include <stdexcept>
#include <format>
#include <print>

class Process
{
private:

	STARTUPINFOW m_startup_info;
	PROCESS_INFORMATION m_process_info;

public:

	Process(wchar_t* command)
		: m_startup_info{}
		, m_process_info{}
	{
		m_startup_info.cb = sizeof(m_startup_info);

		auto ok = CreateProcessW(
			nullptr,
			command,
			nullptr,
			nullptr,
			FALSE,
			0,
			nullptr,
			nullptr,
			&m_startup_info,
			&m_process_info
		);

		if (!ok)
		{
			throw std::runtime_error(std::format("CreateProcessW failed with error: {}", GetLastError()));
		}

		Log::Info(L"executed command '{}' with process id '{}'", command, m_process_info.dwProcessId);
	}

	~Process()
	{
		if (m_process_info.hProcess)
		{
			CloseHandle(m_process_info.hProcess);
		}

		if (m_process_info.hThread)
		{
			CloseHandle(m_process_info.hThread);
		}
	}

	auto Wait()
	{
		WaitForSingleObject(m_process_info.hProcess, INFINITE);

		auto exit_code = DWORD{};
		GetExitCodeProcess(m_process_info.hProcess, &exit_code);

		Log::Info(L"process id '{}' exited with code '{}'", m_process_info.dwProcessId, exit_code);
	}

};

auto main() -> int
{
	wchar_t server_cmd[] = L"C:\\Users\\matth.MATT\\OneDrive\\Desktop\\database\\build\\bin\\Debug\\Server.exe";
	auto server_process = Process(server_cmd);

	wchar_t client_cmd[] = L"C:\\Users\\matth.MATT\\OneDrive\\Desktop\\database\\build\\bin\\Debug\\Client.exe";
	auto client_process = Process(client_cmd);

	server_process.Wait();
	client_process.Wait();

	return 0;
}
