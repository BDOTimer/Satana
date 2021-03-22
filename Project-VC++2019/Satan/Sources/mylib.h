///----------------------------------------------------------------------------|
/// mylib.h
///----------------------------------------------------------------------------:
#ifndef MYLIB_H
#define MYLIB_H

#define BANNER(v1,v2,v3) std::wcout << L##v1 << L"\n" \
                                    << L##v2 << L"\n" \
                                    << L##v3 << L"\n";
///-------------------------------|
/// Эргономика.                   |
///-------------------------------:
namespace win
{
    #include <windows.h>
    inline void set(WORD text = 7, WORD background = 0)
    {   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                (background << 4) | text);
    }
    struct  Color
    {       Color(WORD text){   set(text);}
            Color(         ){   set(    );}
    };
    inline void setname()
    {   SetConsoleTitleW(L"Тайны числа ПИ");
        HWND  hwnd = GetConsoleWindow();
        SetWindowLong(hwnd, GWL_STYLE , WS_CAPTION | WS_SIZEBOX);
        ShowWindow(hwnd, SW_NORMAL);
    }
};
inline std::ostream& operator<<(std::ostream& o, const win::Color c){ return o;}
inline std::wostream&operator<<(std::wostream&o, const win::Color c){ return o;}
#define COLORRESET win::Color()

///-------------------------------|
/// Замер времени.                |
///-------------------------------:
#include <chrono>
#include <thread>
struct  Timer
{
    void start()
    {   past = std::chrono::high_resolution_clock::now();
    }

    void info()
    {   const size_t SEC = (size_t)get_delta_time();
        std::wcout << win::Color(9)
                   << "Time operation: "
                   << second_to_display(SEC) << COLORRESET << "\n\n";
    }

    size_t get_delta_seconds()
    {   return (size_t)get_delta_time();
    }

    std::wstring second_to_display(size_t sec)
    {   size_t h =  sec / 3600;
        size_t m = (sec % 3600) / 60;
        size_t s =  sec %   60;
        return  std::to_wstring(h) + L":" +
                std::to_wstring(m) + L":" +
                std::to_wstring(s);
    }

private:
    std::chrono::high_resolution_clock::time_point past;
    double get_delta_time()
    {   auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = now - past;
        past = now;
        return double(elapsed.count()) / 1000;
    }
};
extern Timer TIMER;
///------------------------------------------------------------------END_mylib.h

#endif // MYLIB_H
