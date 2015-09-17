#ifndef READING_COMPARISON_MEM_MAPPED_H_
#define READING_COMPARISON_MEM_MAPPED_H_


#include <windows.h>


namespace mem_mapped
{
	template <typename U, typename V>
	class _FileMap
	{
	private:
		HANDLE file_handle = nullptr;
		HANDLE file_mapping = nullptr;
		V file_data = nullptr;

		friend std::ostream& operator<< (std::ostream& out, const _FileMap source)
		{
			if (source.isOpen())
			{
				out << source.file_data;
			}

			return out;
		}

		friend std::wostream& operator<< (std::wostream& out, const _FileMap source)
		{
			if (source.isOpen())
			{
				out << source.file_data;
			}
				
			return out;
		}
		

	public:
		DWORD _FileMap::OpenFile(_In_ U file_path )
		{
			CloseFile(); //in case OpenFile is called before a CloseFile
			file_handle = ::CreateFile(file_path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (file_handle == INVALID_HANDLE_VALUE)
				return GetLastError();

			file_mapping = ::CreateFileMapping(file_handle, NULL, PAGE_READONLY, 0, 0, 0);
			if (file_mapping == NULL)
				return GetLastError();
			
			file_data = (V)::MapViewOfFile(file_mapping, FILE_MAP_READ, 0, 0, 0);
			if (file_data == NULL)
				return GetLastError();

			return ERROR_SUCCESS;
		}

		DWORD _FileMap::CloseFile()
		{
			//Umap view, and close mapping object as well as file handle
			BOOL status = { 0 };

			if (file_data != nullptr)
				status = UnmapViewOfFile(file_data);

			if (file_mapping != nullptr)
				status = CloseHandle(file_mapping);

			if (file_handle != nullptr)
				status = CloseHandle(file_handle);
			

			//Zero data
			file_handle = nullptr;
			file_mapping = nullptr;
			file_data = nullptr;


			if (!status)
				return GetLastError();
			return ERROR_SUCCESS;
		}

		DWORD _FileMap::ReadFile(_Out_ V* data)
		{
			if (file_data != nullptr)
			{
				*data = file_data;
				return ERROR_SUCCESS;
			}
			return GetLastError();
		}

		BOOL _FileMap::isOpen() const
		{
			if (file_handle && file_mapping && file_data)
			{
				return true;
			}
			return false;
		}

		//dtor
		_FileMap::~_FileMap() noexcept
		{
			CloseFile();
		}

		_FileMap::_FileMap(const U file_path)
		{
			_FileMap::OpenFile(file_path);
		}

	};

	typedef _FileMap<const wchar_t*, wchar_t*> FileMap;
	typedef _FileMap<const wchar_t*, char*> FileMapA;
}


#endif //READING_COMPARISON_MEM_MAPPED_H_
