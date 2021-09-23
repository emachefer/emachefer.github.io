---
layout: post
title: Importance du choix d'une implémentation
tags: C++ performance
---
En *C++* un problème peut avoir plusieurs solutions possible, mais se valent-elles toutes ?

Pour répondre à cette question, je vais implémenter différentes fonctions remplissant de *0* un tableau de dimension 2.

### Libraires et système
Pour faire ce test, je n'utilise que des librairies standards `chrono`, `iomanip`, `iostream`, `numeric` et `vector`, et comme compilateur `gcc` version `9.3.0`.

Tout d'abord, définissons la taille des échantillons. 
Afin de voir une différence significative, j'utilise une taille globale `constexpr int size = 1000;`.

### Première version
C'est une version standard, qui ressemble à ce qui pourrait être fait en *C*.
Le tableau est rempli en commençant par le second indice.
{% highlight cpp %}
double first_version()
{
	auto start = std::chrono::high_resolution_clock::now();
	int tb_test[size][size];
	for (int i=0; i<size; ++i) {
		for (int j = 0; j<size; ++j) {
			tb_test[ i ][ j ] = 0;
		}
	}
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff = end-start;
	return diff.count();
}
{% endhighlight %}

### Deuxième version
La seconde version ressemble à la première, mais rempli le premier indice avant de remplir le second.
{% highlight cpp %}
double second_version()
{
	auto start = std::chrono::high_resolution_clock::now();
	int tb_test[size][size];
	for (int i=0; i<size; ++i) {
		for (int j = 0; j<size; ++j) {
		// Différence ici
		tb_test[ j ][ i ] = 0;
		}
	}
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff = end-start;
	return diff.count();
}
{% endhighlight %}

### Troisième version
Cette version utilise les boucles du type *C++* avec les fonctions standards `std::begin` et `std::end`.
{% highlight cpp %}
double third_version()
{
	auto start = std::chrono::high_resolution_clock::now();
	int tb_test[size][size];
	auto tb_end = std::end(tb_test);
	auto tb_begin = std::begin(tb_test);
	for (auto pos = tb_begin; pos != tb_end; ++pos) {
		for (auto value: *pos){
			value = 0;
		}
	}
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff = end-start;
	return diff.count();
}
{% endhighlight %}

### Quatrième version
Dans cette version, plutôt que d'utiliser une boucle standard, j'utilise la fonction `std::fill`.
{% highlight cpp %}
double fourth_version()
{
	auto start = std::chrono::high_resolution_clock::now();
	int tb_test[size][size];
	auto tb_end = std::end(tb_test);
	auto tb_begin = std::begin(tb_test);
	for (auto pos = tb_begin; pos != tb_end; ++pos) {
		std::fill(std::begin(*pos), std::end(*pos), 0);
	}
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff = end-start;
	return diff.count();
}
{% endhighlight %}

### Implémentation
Pour tester les différentes versions, voici le programme.

{% highlight cpp %}
int main()
{
  std::stringstream logstream;
  logstream << std::scientific
	    << std::setprecision(3)
	    << "n\t" << "version1\t"<<"version2\t"<<"version3\t"<<"version4\n";
  std::cout << "Calculs en cours...\n";

  std::vector<double> v1, v2, v3, v4;
  const int nb_loops = 10*size;
  for(int it = 0; it < nb_loops; ++it){
    std::cout << "Version " << it+1 << '/' << nb_loops << '\r' << std::flush;
    double d1 = first_version();
    double d2 = second_version();
    double d3 = third_version();
    double d4 = fourth_version();
    v1.push_back(d1);
    v2.push_back(d2);
    v3.push_back(d3);
    v4.push_back(d4);
    logstream << it << '\t'
	      << d1 << '\t'
	      << d2 << '\t'
	      << d3 << '\t'
	      << d4 << '\n';
  }
  std::cout << std::endl;
  if (v1.empty() || v2.empty() || v3.empty()) {
    std::cout << "[ERROR] Les vecteurs sont vide...\n";
    return 0;
  }
  
  double s1 = std::accumulate(v1.begin(), v1.end(), 0.);
  double s2 = std::accumulate(v2.begin(), v2.end(), 0.);
  double s3 = std::accumulate(v3.begin(), v3.end(), 0.);
  double s4 = std::accumulate(v4.begin(), v4.end(), 0.);
  std::cout << "Temps moyens :\n"
	    << "\t- [v1] version 1 : " << s1/nb_loops << " secondes\n"
	    << "\t- [v2] version 2 : " << s2/nb_loops << " secondes\n"
	    << "\t- [v3] version 3 : " << s3/nb_loops << " secondes\n"
	    << "\t- [v4] version 4 : " << s4/nb_loops << " secondes\n";

  std::fstream ofile("./results.csv", std::ios::out);
  ofile << logstream.str();
  ofile.close();
  
  std::cout << std::fixed << std::setprecision(0) <<"Rapports :\n";
  std::cout << "\t v1 est " << s2/s1*100 << "% plus "<< (s1<s2?"rapide":"lente") << " que v2\n";
  std::cout << "\t v3 est " << s2/s3*100 << "% plus "<< (s3<s2?"rapide":"lente") << " que v2\n";
  std::cout << "\t v3 est " << s1/s3*100 << "% plus "<< (s3<s1?"rapide":"lente") << " que v1\n";
  std::cout << "\t v4 est " << s2/s4*100 << "% plus "<< (s4<s2?"rapide":"lente") << " que v2\n";
  std::cout << "\t v4 est " << s1/s4*100 << "% plus "<< (s4<s1?"rapide":"lente") << " que v1\n";
  std::cout << "\t v4 est " << s3/s4*100 << "% plus "<< (s4<s3?"rapide":"lente") << " que v3\n";
  std::cout << std::endl;
    
  return 0;
}
{% endhighlight %}

### Les résultats
Sur 10 000 itérations, les résultats sont les suivants :

![Comparaison sur 10000 itérations]({{site.url}}/examples/CPP/2021-06_fill_comparison/comp_10000.png "Premier test")

Vous pouvez faire le calcul en réutilisant le [code]({{site.git_origin}}/tree/main/examples/CPP/2021-06_fill_comparison/) ou à partir du [fichier csv]({{site.git_origin}}/exemples/CPP/2021-06_fill_comparison/results.csv).

### Pourquoi ces différences ?
* Les deux premières versions semblent similaires, mais la seconde version n'est pas optimisée, la boucle ne vas pas à deux adresses consécutives, mais doit faire un saut à chaque itération.
* La troisième utilise des pointeurs et passe d'adresse en adresse directement.
* La quatrième ne crée pas de copie pour les itérations.

### Conclusion

La librairie standard permet d'être plus explicite sur ces intentions dans le code et permet généralement d'obtenir un temps d'exécution plus rapide qu'une implémentation personnelle.

Une réécriture d'une ancienne librairie en utilisant la librairie standard peut permettre un gain en performance significatif des méthodes utilisées.

Vous pouvez trouver le code sur la page [github du site]({{site.git_origin}}/tree/main/examples/CPP/2021-06_fill_comparison/)
