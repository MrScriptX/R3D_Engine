#ifndef VRED_LOG_TYPE_H
#define VRED_LOG_TYPE_H

#include <format>

enum class LOG_TYPE : unsigned short
{
	ERROR	= 0,
	WARNING = 1,
	INFO	= 2,
	DEBUG	= 3
};

template<class CharT>
struct std::formatter<LOG_TYPE, CharT>
{
    template<typename FormatParseContext>
	constexpr auto parse(FormatParseContext& ctx)
	{
        /*value_format = "{:";
        for (auto it = begin(ctx); it != end(ctx); ++it) 
        {
            char c = *it;
            if (c == 'c')
                curly = true;
            else
                value_format += c;
            if (c == '}')
                return it;
        }*/
        return end(ctx);
	}

    template <typename FormatContext>
    auto format(LOG_TYPE v, FormatContext& ctx) const
    {
        switch (v)
        {
        case LOG_TYPE::ERROR:
            return std::format_to(ctx.out(), "{:7}", "ERROR");
        case LOG_TYPE::WARNING:
            return std::format_to(ctx.out(), "{:7}", "WARNING");
        case LOG_TYPE::INFO:
            return std::format_to(ctx.out(), "{:7}", "INFO");
        case LOG_TYPE::DEBUG:
            return std::format_to(ctx.out(), "{:7}", "DEBUG");
        default:
            return std::format_to(ctx.out(), "{:7}", "");
        }
    }

    bool curly{ false };
    std::string value_format;
};

#endif // !VRED_LOG_TYPE_H
