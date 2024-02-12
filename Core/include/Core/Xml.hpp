// Copyright 2023 <github.com/razaqq>
#pragma once

#include "Core/Log.hpp"
#include "Core/Result.hpp"

#include <expected>
#include <filesystem>
#include <format>
#include <string>
#include <type_traits>

#include <tinyxml2.h>
#include <stdio.h>


namespace PotatoAlert::Core {

using XmlError = std::string;
template<typename T>
using XmlResult = Result<T, XmlError>;
#define PA_XML_ERROR(...) (::std::unexpected(::PotatoAlert::Core::XmlError(std::format(__VA_ARGS__))))

template<typename T = void>
static inline XmlResult<void> LoadXml(tinyxml2::XMLDocument& doc, const std::filesystem::path& xmlPath)
{
	const tinyxml2::XMLError err = doc.LoadFile(xmlPath.string().c_str());
	if (err != tinyxml2::XML_SUCCESS)
	{
		return PA_XML_ERROR("{}", doc.ErrorStr());
	}
	return {};
}

}  // namespace PotatoAlert::Core
