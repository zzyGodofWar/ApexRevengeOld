#pragma once
#include <WinSock2.h>
#include <cstdint>

namespace driver
{
	void	initialize();
	void	deinitialize();

	SOCKET	connect();
	void	disconnect(SOCKET connection);

	uint32_t read_memory(SOCKET connection, uint32_t process_id, uint64_t address, uintptr_t buffer, size_t size);
	uint32_t write_memory(SOCKET connection, uint32_t process_id, uint64_t address, uintptr_t buffer, size_t size);
	uint64_t get_process_base_address(SOCKET connection, uint32_t process_id);
	uint64_t get_process_module_address(SOCKET connection, uint32_t process_id, char* modulename_address);
	bool terminate_process(SOCKET connection, uint32_t process_id);
	uint64_t seacrh_process_address(SOCKET connection, uint32_t process_id, char* search_content);
	uint64_t allocate_memory(SOCKET connection, uint32_t process_id, size_t allocate_size);
	bool free_memory(SOCKET connection, uint32_t process_id, uintptr_t memory_address);

	template <typename T>
	T read(const SOCKET connection, const uint32_t process_id, const uint64_t address)
	{
		T buffer{};
		read_memory(connection, process_id, address, uint64_t(&buffer), sizeof(T));
		return buffer;
	}

	template <typename T>
	void write(const SOCKET connection, const uint32_t process_id, const uint64_t address, const T& buffer)
	{
		write_memory(connection, process_id, address, uint64_t(&buffer), sizeof(T));
	}
}