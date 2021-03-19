///----------------------------------------------------------------------------|
/// An implementation of Gauss–Legendre Algorithm with GMP Library
///
/// MSYS GCC 10.2
///
/// wiki: "При использовании этой схемы 25 итераций достаточно
/// 	   для получения 45 миллионов десятичных знаков."
///
/// Мои резы:
///     Рендер ПИ размером ~ 20'000'000 за  2  минуты.
///     Рендер ПИ размером ~323'228'498 за ~20 минут.   (AMOUNT_ITERATIONS = 30)
///     GNU MP: Cannot allocate memory (size=4294909984)(AMOUNT_ITERATIONS = 31)
///									         4294967295
///		mpf_set_default_prec(2100000000)(632162992) Time: 54:0  = 99.8%
///----------------------------------------------------------------------------:
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <gmpxx.h>
#include <conio.h>

/// link:
///---------------------------------------|
/// C:/msys64/mingw64/lib/libgmpxx.a      |
/// C:/msys64/mingw64/lib/libgmp.a        |
///---------------------------------------.

#include <windows.h>
namespace color
{
    void set(WORD text = 7, WORD background = 0)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
            (background << 4) | text);
    }
    struct  Set
    {
        Set(WORD text)
        {
            set(text);
        }
        ~Set()
        {
            set();
        }
    };
};

///-------------------------------|
/// Замер времени(RAII).          |
///-------------------------------:
#include <chrono>
#include <thread>
struct  Timer
{
    Timer()
    {
        init();
    }
    ~Timer()
    {
        info();
    }

private:
    std::chrono::high_resolution_clock::time_point past;
    void init()
    {
        past = std::chrono::high_resolution_clock::now();
    }
    double get_delta_time()
    {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = now - past;
        past = now;
        return double(elapsed.count()) / 1000;
    }
    void info()
    {
        const size_t SEC = get_delta_time();
        std::cout << " Time operation: "
            << SEC / 60 << ":" << SEC % 60 << "\n\n";
    }
};

const uint64_t AMOUNT_ITERATIONS = 30;

///-------------------------------|
/// Верификатор.                  |
///-------------------------------:
void test(const std::string& s)
{
    std::string etalon;
    {	std::ifstream fromf("pi.txt");
    fromf >> etalon;
    }

    if (etalon.substr(0, etalon.size()) == s.substr(0, etalon.size()))
    {
        std::cout << "TEST GOOD\n";
    }
    else std::cout << "TEST BAD\n\a\a\a";
}

///-------------------------------|
/// Спец-Ввод.                    |
///-------------------------------:
std::string spec_input()
{
    std::cout << "Satan's numbers: ";
    std::string number;
    do
    {
        for (char c = 0; (c = _getch()) != 13;)
        {
            if ('0' <= c && c <= '9')
            {
                number.push_back(c);
                std::cout << c;
            }
            else if (c == 8)
            {
                if (!number.empty())
                {
                    std::cout << "\b \b";
                    number.pop_back();
                }
            }
            else if (c == 27)
            {
                number.clear();
                number.push_back('q');
                break;
            }
        }
    } while (number.size() == 0);

    std::cout << '\n';
    return number;
}

///-------------------------------|
/// Поиск в строке.               |
///-------------------------------:
bool find(const std::string& pi, const std::string key)
{
    if (key[0] == 'q') return false;

    size_t pos = pi.find(key);

    if (pos != std::string::npos)
    {
        if (pos < 8) return false;

        std::cout << "\nFIND      : "
            << "\n  pos     : " << pos - 8 + 1
            << "\n  password: ";
        {	color::Set c(10);
        std::cout << pi.substr(pos - 8, 8) << "\n\n";
        }
        return true;
    }
    color::Set c(12);
    std::cout << "    Not find ...\n\n";
    return true;
}

///----------------------------------------------------------------------------|
/// Старт.
///----------------------------------------------------------------------------:
int main()
{
    const uint64_t AMOUNT_SIGN = std::pow(2, AMOUNT_ITERATIONS);
    std::cout << " ... wait ...";

    mpf_set_default_prec(2100000000);

    mpf_class	a0(1.),
        b0(1.),
        t0(1.),
        p0(1.),
        an(0.),
        bn(0.),
        pn(0.),
        tn(0.),

        ttt(2.); t0 = t0 / 4;
    ttt = sqrt(ttt);
    b0 = b0 / ttt;
    {	Timer timer;

    for (uint64_t i = 0; a0 != b0; ++i)
    {
        an = (a0 + b0) / 2;
        bn = sqrt(a0 * b0);

        const mpf_class  T(a0 - an); /// [+]27:44 [-]30:22
        tn = t0 - (p0 * (T * T));
        pn = p0 * 2;

        std::swap(a0, an);
        std::swap(b0, bn);
        std::swap(t0, tn);
        std::swap(p0, pn);
    }
    }

    mpf_class pi = ((an + bn) * (an + bn)) / (4 * tn);

    mp_exp_t exp;
    std::string  s = pi.get_str(exp);
    {	color::Set c(10);
    std::cout << "\rPi is Ready!\a\a\a";
    }
    std::cout << " (" << s.size() << " bytes)              \n\n";

    {	std::ofstream tof("pi_Legendre.txt");
    tof << s;
    }

    //test(s);
    find(s, "28638823");

    ///-------------------------|
    /// Ручной тест.            |
    ///-------------------------:
    while (find(s, spec_input()));

    std::cout << "\nProgam FINISH\n";
    std::cin.get();
    return 0;
}