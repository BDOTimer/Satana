///----------------------------------------------------------------------------|
/// Класс для работы с конфигом "config.txt"
///----------------------------------------------------------------------------:
///-------------------------------|
/// "config.h"                    |
///-------------------------------:
#ifndef CONFIG_H
#define CONFIG_H

struct Config
{
    ///-------------------|
    /// Render PI.        |
    ///-------------------:
    int  AMOUNT_ITERATIONS;    /// 2^(AMOUNT_ITERATIONS+3) = размер ПИ в битах.
    bool is_skip_hand_test;    /// 1 - Пропустить ручной тест.

    ///-------------------|
    /// Build vob.        |
    ///-------------------:
    size_t LENGTH;             ///     Длина ключа.
    bool   is_rewrite_pi_file; /// 1 - Перезаписывать pi_Legendre.txt

    ///-------------------|
    /// Конфиг.           |
    ///-------------------:
    bool is_visible;           /// 1 - Показать загруженный конфиг.
};

const Config cfg_default =
{   20,
    true,
    8,
    true,
    true
};

extern const Config* CFG;
       const Config* load_config();

#define  l(v)  std::cout << std::setw(22) << #v << " = " << v << "\n";

///-------------------------------|
/// Это можно спрятать в .cpp     |
///-------------------------------:
#include <iostream>
#include <string>
#include <iomanip>
#include "mylib.h"

struct ConfigWork
{   ConfigWork(Config& Cfg) : cfg(Cfg)
    {   load();
    }

private:
    Config& cfg;
    void load()
    {
        std::ifstream fromf("config.txt");
        if(!fromf.is_open())
        {   std::cout << "WARNING: \"config.txt\" not find\n";
            return;
        }

        std::string s;
        std::getline(fromf, s, '\f');

        try
        {   load("AMOUNT_ITERATIONS" , s, cfg.AMOUNT_ITERATIONS );
            load("is_skip_hand_test" , s, cfg.is_skip_hand_test );

            load("LENGTH"            , s, cfg.LENGTH            );
            load("is_rewrite_pi_file", s, cfg.is_rewrite_pi_file);

            load("is_visible"        , s, cfg.is_visible        );
        }
        catch(...)
        {   std::cout << "WARNING: catch load ???: \"" << errword << "\"\n";
        }
    }
    std::string errword;
    template<typename T>
    void load(const std::string& f, const std::string& s, T& val)
    {   size_t p = 0, e;
        errword = f;
        if((p  = s.find(f)) != std::string::npos)
        {   p += f.size();
            if((p = s.find_first_not_of(" =", p)) != std::string::npos)
            {
                if((e = s.find_first_of(" \n,", p)) == std::string::npos)
                {   e = s.size();
                }
                val = T(std::stod(s.substr(p, e - p)));
            }
        }
    }
};

inline const Config* load_config()
{   static Config cfg = cfg_default;
    ConfigWork work(cfg);

    if(!cfg.is_visible) return &cfg;

    BANNER(
    "///-------------------------|",
    "/// Загруженный конфиг.     |",
    "///-------------------------:")
        l(cfg.AMOUNT_ITERATIONS)
        l(cfg.is_skip_hand_test)
        l(cfg.LENGTH)
        l(cfg.is_rewrite_pi_file)
        l(cfg.is_visible)

    std::cout << '\n';
    return &cfg;
}

#endif // CONFIG_H
