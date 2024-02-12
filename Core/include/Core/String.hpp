// Copyright 2021 <github.com/razaqq>
#pragma once

#include "Core/Encoding.hpp"
#include "Core/Result.hpp"

#include <fmt/xchar.h>
#include <fmt/format.h>

#include <charconv>
#include <span>
#include <string>
#include <vector>


namespace PotatoAlert::Core {

template<typename T>
concept is_string = std::is_same_v<T, std::string> || std::is_same_v<T, std::wstring>;

namespace String {

std::string Trim(std::string_view str);
std::string ToUpper(std::string_view str);
std::string ToLower(std::string_view str);
std::vector<std::string> Split(std::string_view str, std::string_view del);
std::string ReplaceAll(std::string_view str, std::string_view before, std::string_view after);
std::string Join(std::span<std::string_view> v, std::string_view del);
std::string Join(std::span<const std::string> v, std::string_view del);
void ReplaceAll(std::string& str, std::string_view before, std::string_view after);
bool Contains(std::string_view str, std::string_view part);
bool StartsWith(std::string_view str, std::string_view start);
bool EndsWith(std::string_view str, std::string_view end);

template<typename T>
bool ParseNumber(std::string_view str, T& value) requires std::is_integral_v<T> || std::is_floating_point_v<T>
{
	// static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>, "Type must be numeric");
	return std::from_chars(str.data(), str.data() + str.size(), value).ec == std::errc{};
}

bool ParseBool(std::string_view str, bool& value);

#undef STR
#ifdef WIN32
	#define STR(String) L##String
#else
	#define STR(String) String
#endif

}  // namespace String

}  // namespace PotatoAlert::Core::String

struct StringWrap
{
	std::string Str;

	explicit StringWrap(const std::string& str) : Str(str) {}
	explicit StringWrap(std::string&& str) noexcept : Str(str) {}

	~StringWrap()
	{
		
	}
};


template<>
struct fmt::formatter<StringWrap>
{
	[[maybe_unused]] static constexpr auto parse(const format_parse_context& ctx)
			-> format_parse_context::iterator
	{
		return ctx.begin();
	}

	auto format(const StringWrap& val, format_context& ctx) const -> format_context::iterator
	{
		return fmt::format_to(ctx.out(), "{}", val.Str);
	}
};
