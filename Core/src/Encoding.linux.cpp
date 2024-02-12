// Copyright 2023 <github.com/razaqq>

#include "Core/Encoding.hpp"

#include "Core/Result.hpp"

#include <filesystem>
#include <span>
#include <string>
#include <system_error>


using PotatoAlert::Core::Result;

Result<std::string> PotatoAlert::Core::PathToUtf8(const std::filesystem::path& path)
{
	return path.native();
}

Result<std::filesystem::path> PotatoAlert::Core::Utf8ToPath(std::string_view string)
{
	return std::filesystem::path(string);
}
