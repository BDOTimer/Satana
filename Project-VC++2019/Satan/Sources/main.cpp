﻿///----------------------------------------------------------------------------|
/// An implementation of Gauss–Legendre Algorithm.
/// MSYS GCC 10.2 + GMP  Library
/// VC++ 2019     + MPIR Library
///
/// wiki: "При использовании этой схемы 25 итераций достаточно
/// 	   для получения 45 миллионов десятичных знаков."
/// 
/// Мои резы:
///     Рендер ПИ размером ~ 20'000'000 за  2  минуты.
///     Рендер ПИ размером ~323'228'498 за ~20 минут.   (AMOUNT_ITERATIONS = 30)
///		mpf_set_default_prec(2100000000)(632'162'992) Time: 54:0  = 99.8%
/// 
/// Иттераций: 25, 80'807'125 bytes
/// [VC++] : Time: ?:?
/// [MSYS] : Time: ?:?
/// 
/// Иттераций: 23, 20'201'783 bytes
/// [VC++] : Time: 00:04:37
/// [MSYS] : Time: ?:?
/// 
/// Иттераций: 29, ... bytes
/// [VC++] : Time: ?:?
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

#include "config.h"
const Config* CFG;
Timer TIMER;

void Tools_render_vob();

///-------------------------------|
/// Верификатор.                  |
///-------------------------------:
void test(const std::string& s)
{   std::string etalon;
    {   std::ifstream fromf("pi.txt");
        fromf >> etalon;
    }

    if (etalon.substr(0, etalon.size()) == s.substr(0, etalon.size()))
    {   std::cout << "TEST GOOD\n";
    }
    else std::cout << "TEST BAD\n\a\a\a";
}

///-------------------------------|
/// Спец-Ввод.                    |
///-------------------------------:
std::string spec_input()
{   std::cout << win::Color(11) << "INPUT Satan's numbers: "; COLORRESET;
    std::string number;
    while(number.size() == 0)
    {   for (char c = 0; (c = _getch()) != 13;)
        {   if ('0' <= c && c <= '9')
            {   number.push_back(c);
                std::cout << c;
            }
            else if (c == 8 && !number.empty())
            {   number.pop_back();
                std::cout << "\b \b";
            }
            else if (c == 27)
            {   number.clear();
                number.push_back('q');
                break;
            }
        }
    }
    std::cout << '\n';
    return number;
}

///-------------------------------|
/// Поиск в строке.               |
///-------------------------------:
bool find(const std::string& pi, const std::string key)
{   if (key[0] == 'q') return false;

    size_t pos = pi.find(key);

    if (pos != std::string::npos)
    {   if (pos < 8) return false;

        std::cout <<   "FIND      : "
                  << "\n  position: " << pos - 8 + 1
                  << "\n  password: "
                  << win::Color(10) << pi.substr(pos - 8, 8) << "\n\n"; 
        COLORRESET;
        return true;
    }
    std::cout << win::Color(12) << "    Not find ...\n\n"; 
    COLORRESET;
    return true;
}

///----------------------------------------------------------------------------|
/// Старт.
///----------------------------------------------------------------------------:
int main()
{
    win::setname();
    setlocale(0, "");
    //system   ("mode 80,50");

    CFG = load_config();

    win::Color(14);
    BANNER(
        "///-------------------------|",
        "/// Render числа PI.        |",
        "///-------------------------:")
    win::Color();

    const int AMOUNT_ITERATIONS = CFG->AMOUNT_ITERATIONS;
    std::string file_name = "pi_Legendre.txt";

    bool b;
    {   std::ifstream f(file_name);
        b = f.is_open();
    }
    if  (!b || CFG->is_rewrite_pi_file)
    {
        const size_t AMOUNT_BIT = (size_t)std::pow(2, AMOUNT_ITERATIONS+3);
        std::cout << "AMOUNT_BIT = " << AMOUNT_BIT << "\n";

        std::cout << "Init vars ...";
        mpf_set_default_prec(AMOUNT_BIT);

        TIMER.start();

        ///-------------------------------|
        /// Gauss–Legendre Algorithm.     |
        ///-------------------------------:
        mpf_class pi;
        int       i = AMOUNT_ITERATIONS;
        {   
            Timer timer2;
                  timer2.start();

        mpf_class	a0(1.),
                    b0(1.),
                    t0(1.),
                    p0(1.),
                    an(0.),
                    bn(0.),
                    pn(0.),
                    tn(0.),

                    tmp,
                    ttt(2.);         t0 = t0 / 4;
                    ttt = sqrt(ttt); b0 = b0 / ttt;
            
            for (; a0 != b0; --i)
            {   std::cout << "\rAMOUNT_ITERATIONS = "
                          << win::Color(14) << i << COLORRESET;
                std::cout << " ... wait ...  ";

                if (i >= 0)
                {   std::wcout  << L" (Прогноз на оставшееся время: "
                                << timer2.second_to_display(
                                   timer2.get_delta_seconds() * (i + i)) << ")"
                                << "     ";
                }
                else std::wcout << L" Всё нормально - добавка для точности ...";

                an = (a0 + b0) / 2;
                bn = sqrt(a0 * b0);

                                 tmp = a0 - an; /// [+]27:44 [-]30:22
                tn = t0 - (p0 * (tmp * tmp));
                pn = p0 * 2;

                a0.swap(an);
                b0.swap(bn);
                t0.swap(tn);
                p0.swap(pn);
            }
                  tmp = an + bn;
            pi = (tmp * tmp) / (4 * tn);
        }
        std::wcout << L"\rВсего итераций: " << AMOUNT_ITERATIONS - i 
        << "                                                                \n";

        TIMER.info();

        BANNER(
            "///-------------------------|",
            "/// Запись на диск.         |",
            "///-------------------------:")
        mp_exp_t exp;
        std::string  s = pi.get_str(exp);

        std::cout << win::Color(14) << "\rPi is Ready!\a";
        std::cout << " ("
                  << win::Color(15) << s.size()
                  << win::Color( 7) << " bytes)              \n";

        {   std::ofstream tof(file_name);
            tof << s;
            TIMER.info();
        }

        //test(s);
        find(s, "28638823");

        BANNER(
            "///-------------------------|",
            "/// Ручной тест.            |",
            "///-------------------------:")
        if (!CFG->is_skip_hand_test)
        {   std::cout << "Press ESCAPE for stop ...\n";
            while (find(s, spec_input()));
            std::cout << "... exit\n\n";
        }
        else std::cout << "Пропущен согласно конфигу ...\n\n";
    }
    else
    {   std::wcout << L"WARNING: File \"" << file_name.c_str()
                   << L"\" существует!\n"
                   << win::Color(12)
                   << L"Render PI отменён ...\n\n";
    }

    win::Color(14);
    BANNER(
        "///-------------------------|",
        "/// Render словаря Сатаны.  |",
        "///-------------------------:")
    win::Color();
         Tools_render_vob();

    std::cout << win::Color(2) << "\nProgram FINISHED.\n\n";
    while(std::cin.get());
    return 0;
}