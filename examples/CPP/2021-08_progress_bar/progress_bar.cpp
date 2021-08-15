#include <iostream>
#include <iomanip>
#include <cmath>
// Pour le test
#include <vector>
#include <thread>
#include <chrono>

// #define TEST_ANSI 1

// Codes ANSI utilisés
constexpr auto reset   = "\x1b[0m";
constexpr auto bgblack = "\x1b[40m"; // Fond noir, autres valeurs {40->47} et {100->107}
constexpr auto bgcolor = "\x1b[42m"; // Fond vert, autres valeurs {40->47} et {100->107}

// Pour l'exemple
namespace exemple {
  struct st_exemple {
    int a, b, c;
    st_exemple(): a(0), b(0), c(0){};
  };
};

int main()
{
  // Vecteur a traiter
  std::vector<exemple::st_exemple> vRawValues(100, exemple::st_exemple());

#if __cplusplus > 201703L
  std::cout << "With c++20\n";
  using namespace std::chrono_literals;
  for(size_t i = 0,
	Max = std::distance(vRawValues.begin(), vRawValues.end());
      auto v:vRawValues) {
    const int percent = (++i)*100/Max;
    const double resize = 0.25;
    std::cout << "Traitement en cours " << std::setw(3) << percent << "\% "
#ifdef TEST_ANSI
	      << bgcolor // Ici on applique le style sur la suite
	      << std::string(std::ceil(percent*resize), ' ')
	      << bgblack
	      << std::string(std::floor((100-percent)*resize), ' ')
	      << reset // On revient au style d'origine

#else
	      << '|'	<< std::string(std::ceil(percent*resize), '#')
	      << std::string(std::floor((100-percent)*resize), ' ') << '|'
#endif
	      << "\r" << std::flush;
    // Traitement des valeurs dans le vecteur
    std::this_thread::sleep_for(100ms);
  }
#else // C++ 17 ou moins
  std::cout << "Without c++20\n";
  // Temps pour simuler un traitement long
  const std::chrono::milliseconds ms{100};
  {
    const size_t Max = std::distance(vRawValues.begin(), vRawValues.end());
    // Pour limiter le scope des deux variables utilisée uniquement dans la boucle
    size_t i = 0;
    for(auto v:vRawValues){
      const int percent = (++i)*100/Max;
      const double resize = 0.25;
      std::cout << "Traitement en cours " << std::setw(3) << percent << "\% "
#ifdef TEST_ANSI
  		<< bgcolor // Ici on applique le style sur la suite
		<< std::string(std::ceil(percent*resize), ' ')
		<< bgblack
		<< std::string(std::floor((100-percent)*resize), ' ')
		<< reset // On revient au style d'origine

#else
  		<< '|'	<< std::string(std::ceil(percent*resize), '#')
		<< std::string(std::floor((100-percent)*resize), ' ') << '|'
#endif
		<< "\r" << std::flush;
      // Traitement des valeurs dans le vecteur
      std::this_thread::sleep_for(ms);
    }
  }
#endif
  // std::cout << i; <= Erreur
  std::cout << "\nTraitement terminé.\n";
}
