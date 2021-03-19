///----------------------------------------------------------------------------|
/// Тулза для рендера словоря Сатаны из кэш-файла ПИ.
///----------------------------------------------------------------------------:
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <cmath>

#define BANNER(v1,v2,v3) std::cout << v1 << "\n" << v2 << "\n" << v3 << "\n";
 
///-----------------------|
/// Длина ключа.          |
///-----------------------:
const size_t LENGTH    = 8;
const char*  NAME_FILE = "pi_Legendre.txt";

///----------------------------------------------------------------------------|
/// Старт.
///----------------------------------------------------------------------------:
int main()
{   setlocale(0,"");
	std::cout << "LENGTH = " << LENGTH << '\n';
 
    std::string s;
    {   std::ifstream f(NAME_FILE);
        if(!f.is_open())
        {   std::cout << "ERROR FILE " << NAME_FILE << '\n';
        	std::cin.get();
            return 2;
        }
        f.seekg  (0, std::ios::end);
        s.reserve(f.tellg(       ));
        f.seekg  (0, std::ios::beg);
        
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
    for    (size_t j = 1000, i = 2+LENGTH; j < N; j += 1000)
    {   for(                             ; i < j;   ++ i   )
        {   cargo[s.substr(i, LENGTH)] = s.substr(i-LENGTH, LENGTH);
        }
        
        std::cout << "\rFIND: " << cargo.size() 
                  << " (from:"  << NEED << ")" << " Progress last iteration: +" 
                  << cargo.size() - now << "                  ";
        now = cargo.size();
        
        if(cargo.size() == NEED)
        {   std::cout << "\n\nFind ALL pair !!!\n"
                      << "Amount iteration: <" << i << "\n";
            break;
        }
    }
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
    std::cout << "Файл \"" << tof_name << "\" создан.\n\n";
    
    std::cout << "\nEfficiency: " << (100./NEED)*cargo.size() << "%\n\n";
    std::cout << "Program FINISHED.\n\n";
    while(std::cin.get());
}

