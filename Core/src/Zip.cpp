// Copyright 2021 <github.com/razaqq>

#include "Core/Encoding.hpp"
#include "Core/Result.hpp"
#include "Core/Zip.hpp"

#include "zip.h"

#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <vector>


using PotatoAlert::Core::Zip;
namespace fs = std::filesystem;

namespace {

template<typename T>
static constexpr T CreateHandle(const zip_t* handle)
{
	return static_cast<T>(reinterpret_cast<uintptr_t>(handle));
}

template<typename T>
static constexpr T UnwrapHandle(Zip::Handle handle)
{
	return reinterpret_cast<T>(static_cast<uintptr_t>(handle));
}

}

Zip Zip::Open(const fs::path& path, int compressionLevel, Mode mode)
{
	const zip_t* zip = nullptr;
	zip = zip_open(path.string().c_str(), compressionLevel, static_cast<char>(mode));

	if (zip != nullptr)
	{
		return Zip(CreateHandle<Handle>(zip));
	}
	return Zip(Handle::Null);
}

void Zip::Close()
{
	zip_close(UnwrapHandle<zip_t*>(std::exchange(m_handle, Handle::Null)));
}

bool Zip::OpenEntry(std::string_view name) const
{
	return zip_entry_open(UnwrapHandle<zip_t*>(m_handle), name.data()) == 0;
}

bool Zip::OpenEntry(int index) const
{
	return zip_entry_openbyindex(UnwrapHandle<zip_t*>(m_handle), index) == 0;
}

bool Zip::CloseEntry() const
{
	return zip_entry_close(UnwrapHandle<zip_t*>(m_handle)) == 0;
}

std::optional<std::string> Zip::EntryName() const
{
	if (const char* entryName = zip_entry_name(UnwrapHandle<zip_t*>(m_handle)))
		return entryName;
	return std::nullopt;
}

std::optional<int> Zip::EntryIndex() const
{
	const int res = zip_entry_index(UnwrapHandle<zip_t*>(m_handle));
	if (res > 0)
		return res;
	return std::nullopt;
}

std::optional<bool> Zip::EntryIsDir() const
{
	const int isDir = zip_entry_isdir(UnwrapHandle<zip_t*>(m_handle));
	if (isDir > 0)
		return isDir == 1;
	return std::nullopt;
}

bool Zip::WriteEntry(std::string_view entryName, const std::string& data) const
{
	auto zip = UnwrapHandle<zip_t*>(m_handle);

	if (!zip_entry_open(zip, entryName.data()))
	{
		const int success = zip_entry_write(zip, data.c_str(), data.size());
		zip_entry_close(zip);
		return success == 0;
	}
	return false;
}

bool Zip::Create(std::string_view archiveName, std::span<std::string_view> fileNames)
{
	std::vector<const char*> arr;
	arr.reserve(fileNames.size());

	for (std::string_view fileName : fileNames)
	{
		arr.push_back(fileName.data());
	}
	return zip_create(archiveName.data(), arr.data(), arr.size()) == 0;
}

bool Zip::RawExtract(const char* file, const char* dir, int (*callback)(const char* fileName, void* context), void* context)
{
	return zip_extract(file, dir, callback, context) == 0;
}

uint64_t Zip::SizeEntry(std::string_view entryName) const
{
	return zip_entry_size(UnwrapHandle<zip_t*>(m_handle));
}

int Zip::EntryCount() const
{
	return zip_total_entries(UnwrapHandle<zip_t*>(m_handle));
}
