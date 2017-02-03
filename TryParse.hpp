#pragma once
#include <limits>
#include <string>

inline bool TryParseLong(
	std::string const & str,
	long *p,
	int radix = 10)
{
	char *pEnd{};
	long ret = strtol(str.c_str(), &pEnd, radix);
	if (pEnd && *pEnd) { return false; }
	if (p) { *p = ret; }
	return true;
}

inline bool TryParseInt(
	std::string const & str,
	int *p,
	int radix = 10)
{
#ifdef WIN32
	// Windows always treat int and long as 32-bit
	return TryParseLong(str,
		reinterpret_cast<long*>(p),
		radix);
#else
	long ret{};
	if (!TryParseLong(str, &ret, radix)) { return false; }
	if (ret < INT_MIN || ret > INT_MAX) { return false; }
	return static_cast<int>(ret);
#endif
}
