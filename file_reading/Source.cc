#include <chrono>
#include <iostream>
#include <fstream>
#include <Strsafe.h>
#include <codecvt>
#include <memory>

#include "mem_mapped.h"

BOOL RunTest();

int main()
{
	BOOL status = RunTest();
	if (status)
	{
		std::cout << "Please make sure you're running as Administrator if needed, and that the 200 data files are located in C:\\data\\";
	}

	return 0;
}

//Returns -1 if there is a problem reading any file
BOOL RunTest()
{
	signed long long time_mem[200] = { 0 };
	signed long long time_ifs[200] = { 0 };
	auto time_start = std::chrono::high_resolution_clock::now();
	auto time_end = std::chrono::high_resolution_clock::now();
	
	for (int j = 0; j < 10; j++)
	{
		for (int i = 1; i <= 200; i++)
		{
			wchar_t path[16];
			StringCbPrintf(path, sizeof(path), L"%s%d%s", L"C:\\data\\", i, L".dat");


			//Memory mapped file portion
			time_start = std::chrono::high_resolution_clock::now();
			mem_mapped::FileMap file_mem(path);
			wchar_t* buffer_mem = nullptr;
			if (file_mem.isOpen())
			{
				if (file_mem.ReadFile(&buffer_mem))
				{
					return -1;
				}

			}
			file_mem.CloseFile();
			time_end = std::chrono::high_resolution_clock::now();
			time_mem[i - 1] += std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start).count();



			//Input file stream
			time_start = std::chrono::high_resolution_clock::now();
			std::wifstream file_ifs(path, std::ios::in);
			file_ifs.imbue(std::locale(file_ifs.getloc(),
				new std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>));

			if (file_ifs.good())
			{

				file_ifs.seekg(0, file_ifs.end);
				std::streamsize length = file_ifs.tellg();
				file_ifs.seekg(0, file_ifs.beg); //Reset the position
				auto buffer_ifs = std::unique_ptr<wchar_t>(new wchar_t[length]);

				if (file_ifs.good())
				{
					file_ifs.read(buffer_ifs.get(), length);
					if (buffer_ifs.get() == nullptr)
					{
						return -1;
					}
				}
			}
			file_ifs.close();
			time_end = std::chrono::high_resolution_clock::now();
			time_ifs[i - 1] += std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start).count();



		}
	}

	std::ofstream file_out_mem;
	file_out_mem.open("data_mem.txt", std::ios::trunc);
	for (auto e : time_mem)
	{
		file_out_mem << (e/10.0) << "\n";
	}


	std::ofstream file_out_ifs;
	file_out_ifs.open("data_ifs.txt", std::ios::trunc);
	for (auto e : time_ifs)
	{
		file_out_ifs << (e/10.0) << "\n";
	}
	return 1;
}