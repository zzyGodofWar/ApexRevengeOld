#pragma once
#include "stdint.h"

constexpr auto packet_magic = 0x12345568;
constexpr auto server_ip = 0x7F000001; // 127.0.0.1

extern uint16_t server_port;
//constexpr auto server_port  = 55935;

enum class PacketType
{
	packet_completed,
	packet_copy_memory,
	packet_allocate_memory,
	packet_free_memory,
	packet_search_memory,
	packet_get_base_address,
	packet_get_module_address,
	packet_terminate_process
};

struct PacketCopyMemory
{
	uint32_t dest_process_id;
	uint64_t dest_address;

	uint32_t src_process_id;
	uint64_t src_address;

	uint32_t size;
};

struct PacketGetBaseAddress
{
	uint32_t process_id;
};

struct PacketTerminateProcess
{
	uint32_t process_id;
};

struct PacketGetMoudleAddress
{
	uint32_t process_id;
	char module_name[128];
};

struct PacketAllocateMemory {
	uint32_t process_id;
	uint32_t allocate_size;
};

struct PacketFreeMemory {
	uint32_t process_id;
	uint32_t memory_address;
};


struct PacketSearchMemory
{
	uint32_t process_id;
	char signature_code[256];
};

struct PackedCompleted
{
	uint64_t result;
};

struct PacketHeader
{
	uint32_t   magic;
	PacketType type;
};

struct Packet
{
	PacketHeader header;
	union
	{
		PackedCompleted		 completed;
		PacketCopyMemory	 copy_memory;
		PacketAllocateMemory allocate_memory;
		PacketFreeMemory 	 free_memory;
		PacketSearchMemory search_memory;
		PacketGetBaseAddress get_base_address;
		PacketGetMoudleAddress get_module_address;
		PacketTerminateProcess terminate_process;

	} data;
};