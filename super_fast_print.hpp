#ifndef SUPER_FAST_PRINT_37248923
#define SUPER_FAST_PRINT_37248923

#include <stdio.h>
#include <type_traits>

namespace SuperFastPrint
{

    namespace details
    {

        class PrintToFile
        {
            FILE* file;
        public:
            PrintToFile(FILE* file) : file(file) {}
            inline void PutCh(const char ch) { fputc(ch, file); }
        };

        class PrintToString
        {
            const char* start;
            const char* end;
            char* current;
        public:

            template<int N>
            PrintToString(char(&arr)[N])
                : start(arr)
                , end(arr + N)
                , current(arr)
            {}

            inline void PutCh(const char ch) { *current++ = ch; }

            inline ~PrintToString()
            {
                *current = 0; // Terminate string

                              // TODO: Array bounds checking here?
                if (current >= end)
                {
                    //... Shit the bed
                    std::terminate();
                }
            }
        };

        template<typename Device>
        inline void PrintSingle(Device& dev, const char* p)
        {
            while (*p) dev.PutCh(*p++);
        }

        template<typename Device>
        inline void PrintSingle(Device& dev, const char p)
        {
            dev.PutCh(p);
        }

        template <typename Device, typename Integer,
            typename = std::enable_if_t<std::is_integral<Integer>::value> >
            inline void PrintSingle(Device& dev, Integer i, typename std::enable_if
                <std::is_unsigned<Integer>::value>::type* = 0,
                typename std::enable_if<(sizeof(Integer) > 4)>::type* = 0)
        {
            // Unsigned 64bit int
            static const int sz = 24;
            char buf[sz];
            char* p = buf + sz;
            char* end = p;

            uint32_t temp;

            while (i > std::numeric_limits<uint32_t>::max())
            {
                temp = i % 1000000000;
                i /= 1000000000;

                for (int j = 0; j != 9; ++j)
                {
                    *--p = '0' + (temp % 10);
                    temp /= 10;
                }
            }

            temp = static_cast<uint32_t>(i);

            do
            {
                *--p = '0' + (temp % 10);
                temp /= 10;
            } while (temp);

            while (p != end) dev.PutCh(*p++);
        }

        template <typename Device, typename Integer,
            typename = std::enable_if_t<std::is_integral<Integer>::value> >
            inline void PrintSingle(Device& dev, Integer in, typename std::enable_if
                <std::is_signed<Integer>::value>::type* = 0,
                typename std::enable_if<(sizeof(Integer) > 4)>::type* = 0)
        {
            // Signed 64bit int
            static const int sz = 24;
            char buf[sz];
            char* p = buf + sz;
            char* end = p;
            auto i = (in < 0) ? -in : in;

            uint32_t temp;

            while (i > std::numeric_limits<uint32_t>::max())
            {
                temp = i % 1000000000;
                i /= 1000000000;

                for (int j = 0; j != 9; ++j)
                {
                    *--p = '0' + (temp % 10);
                    temp /= 10;
                }
            }

            temp = static_cast<uint32_t>(i);

            do
            {
                *--p = '0' + (temp % 10);
                temp /= 10;
            } while (temp);

            if (in < 0) dev.PutCh('-');

            while (p != end) dev.PutCh(*p++);
        }

        template <typename Device, typename Integer,
            typename = std::enable_if_t<std::is_integral<Integer>::value> >
            inline void PrintSingle(Device& dev, Integer i, typename std::enable_if
                <std::is_unsigned<Integer>::value>::type* = 0,
                typename std::enable_if<(sizeof(Integer) <= 4)>::type* = 0)
        {
            static const int sz = 24;
            char temp[sz];
            char* p = temp + sz;
            char* end = p;

            do
            {
                *--p = '0' + (i % 10);
                i /= 10;
            } while (i);

            while (p != end) dev.PutCh(*p++);
        }

        template <typename Device, typename Integer,
            typename = std::enable_if_t<std::is_integral<Integer>::value> >
            inline void PrintSingle(Device& dev, Integer i, typename std::enable_if
                <std::is_signed<Integer>::value>::type* = 0,
                typename std::enable_if<(sizeof(Integer) <= 4)>::type* = 0)
        {
            static const int sz = 24;
            char temp[sz];
            Integer num = (i >= 0) ? i : -i;
            char* p = temp + sz;
            char* end = p;

            do
            {
                *--p = '0' + (num % 10);
                num /= 10;
            } while (num);

            if (i < 0) dev.PutCh('-');

            while (p != end) dev.PutCh(*p++);
        }

        template<typename Device, typename T>
        inline void PrintImpl(Device& dev, T t)
        {
            PrintSingle(dev, t);
        }

        template<typename Device, typename T, typename... Ts>
        inline void PrintImpl(Device& dev, T t, Ts... ts)
        {
            PrintSingle(dev, t);
            PrintImpl(dev, ts...);
        }

        template<typename Device, typename... Ts>
        inline void Print(Device&& dev, Ts... ts)
        {
            PrintImpl(dev, ts...);
        }

    }

    template<int N, typename... Ts>
    inline void Print(char(&arr)[N], Ts... ts)
    {
        details::Print(details::PrintToString(arr), ts...);
    }

    template<typename... Ts>
    inline void Print(FILE* file, Ts... ts)
    {
        details::Print(details::PrintToFile(file), ts...);
    }

}


#endif // SUPER_FAST_PRINT_37248923

