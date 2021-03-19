///----------------------------------------------------------------------------|
/// Тулза для рендера словоря Сатаны из кэш-файла ПИ.
///----------------------------------------------------------------------------:
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <cmath>

#include "config.h"
extern const Config* CFG;

size_t get_size_file(std::ifstream& f)
{   f.seekg  (0, std::ios::end);
    size_t size = f.tellg(    );
    f.seekg  (0, std::ios::beg);
    return size;
}

///-----------------------|
/// Поиск в словаре.      |
///-----------------------:
std::string find_into_vob(std::string s)
{   std::string f;

    if(s.size() != CFG->LENGTH)
    {   std::cout << "ERROR: size of input number must be "
                  << CFG->LENGTH << ".\n";
        return f;
    }

    std::string name("satan_vob_");
                name += std::to_string(CFG->LENGTH) + ".txt";

    std::ifstream fromf(name);
    if(!fromf.is_open())
    {   std::cout << "ERROR: file \"" << name << "\" not igxist ...\n";
        return f;
    }

    std::getline(fromf, f);
    const size_t STARTOFFSET = f.size() + 2;

    {   size_t size      = get_size_file(fromf);
        size_t size_must = (size_t)std::pow(10, CFG->LENGTH)
                         * (CFG->LENGTH * 2 + 3) + STARTOFFSET;

        //l(size)
        //l(size_must)
        //l((size_t)std::pow(10, CFG->LENGTH))

        if(size != size_must)
        {   std::cout << "ERROR: file \"" << name
                      << "\" don't 100% build ...\n";
            f.clear();
            return f;
        }
    }

    std::getline(fromf, f);
    fromf.seekg(0, std::ios::beg);

    const size_t SIZE_NUM = f.size()/2;
    if(SIZE_NUM != 8 && f.size()%2 != 0)
    {   std::cout << "ERROR: file \"" << name << "\" is bad data ...\n";
        f.clear();
        return f;
    }

    try
    {   size_t number = std::stoi(s);
        fromf.seekg(STARTOFFSET  + number * (CFG->LENGTH * 2 + 3));
        f.resize(CFG->LENGTH * 2 + 1, 'x');
        fromf.read((char*)f.data(), CFG->LENGTH * 2 + 1);
    }
    catch(...)
    {   std::cout << "ERROR INPUT: " << s << "\n\n";
        f.clear();
        return f;
    }
    return f;
}

bool test_Find_into_vob(std::string s)
{   if(s[0] == 'q') return false;

    s = find_into_vob(s);
    if(s.empty()) std::cout << win::Color(12) << "Result fail ..";
    else          std::cout << win::Color(10) << s;
    std::cout << ".\n\n";
    return true;
}

///----------------------------------------------------------------------------|
/// Старт.
///----------------------------------------------------------------------------:
void Tools_render_vob()
{
    ///-----------------------|
    /// Длина ключа.          |
    ///-----------------------:
    const size_t LENGTH    = CFG->LENGTH;
    const char*  NAME_FILE = "pi_Legendre.txt";

	std::cout << "LENGTH = " << LENGTH << '\n';

    std::string s;
    {   std::ifstream f(NAME_FILE);
        if(!f.is_open())
        {   std::cout << "ERROR FILE " << NAME_FILE << '\n';
        	std::cin.get();
            return;
        }

        s.reserve(get_size_file(f)+1);
        std::getline(f, s, '\f');
    }

    const size_t N = s.size();

    ///---------------------------------------------|
    /// Тулза для очистки файла от пробелов и т.п.. |
    ///---------------------------------------------:
    if(false) ///<<<---true: включить.
    {   std::ofstream tof(NAME_FILE);
        for(auto i = s.begin(); i != s.end(); ++i)
        {   if( '0' <= *i && *i <= '9') tof << *i;
        }
    }

    const unsigned NEED = (unsigned)std::pow(10, LENGTH);

    std::map<std::string, std::string> cargo;
    size_t now = 0;

    size_t last_num;

    TIMER.start();
    for    (size_t j = 5000, i = LENGTH; j < N; j += 5000)
    {   for(                           ; i < j;   ++ i   )
        {   if(false)
                cargo[s.substr(i, LENGTH)] = s.substr(i-LENGTH, LENGTH);
            else if(cargo.find(s.substr(i, LENGTH)) == cargo.end())
            {   cargo[s.substr(i, LENGTH)] = s.substr(i-LENGTH, LENGTH);
                last_num = i;
            }
        }

        std::cout << "\rFIND: ";
        std::cout << win::Color(10) << cargo.size() << COLORRESET;;

        std::cout << " (from:"  << NEED << ")" << " Progress last iteration: +"
                  << cargo.size() - now << "                  ";
        now = cargo.size();

        if(cargo.size() == NEED)
        {   std::cout << win::Color(14) << "\n\nFind ALL pair !!!\n";
            COLORRESET;
            break;
        }
    }
    std::cout << '\n';
    std::cout << "\nEfficiency: " << (100./NEED)*cargo.size() << "%\n\n";
    TIMER.info();

    std::wcout << L"Последнее найденное число: ";
    std::cout  << win::Color(10) << s.substr(last_num, LENGTH) << COLORRESET;;

    std::wcout << L" на позиции ";
    std::cout  << win::Color(10) << last_num << '\n' << COLORRESET;;

    std::cout << '\n';
    std::string tof_name("satan_vob_");
                tof_name += std::to_string(LENGTH) + ".txt";

    BANNER(
    "///----------------------------|",
    "/// Сохраняем в файл.          |",
    "///----------------------------:")
    {   std::ofstream tof(tof_name);
                      tof << "Satan --> password" << '\n';
        for(const auto& v : cargo)
        {   tof << v.first << ' ' << v.second << '\n';
        }
    }
    std::wcout << L"Файл \"" << tof_name.c_str() << L"\" создан.\n\n";

    BANNER(
    "///-------------------------|",
    "/// Ручной поиск в словаре. |",
    "///-------------------------:")
    std::cout << "Press ESCAPE for stop ...\n";
    std::string spec_input();
    while (test_Find_into_vob(spec_input()));
}
