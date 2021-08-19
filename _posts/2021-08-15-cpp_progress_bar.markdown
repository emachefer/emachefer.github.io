---
layout: post
title: Implémentation d'une barre de progression dans un terminal
tags: C++ ANSI TUI progress-bar tutoriel
---
Il peut arriver régulièrement que le traitement des données dans une boucle prenne beaucoup de temps. Ne rien afficher peut donner l'impression que le traitement est bloqué, mais doit-on pour autant afficher tout le traitement en cours à l'utilisateur ?

Cet article propose une solution pour implémenter une barre de progression pour indiquer le traitement en cours des données.

### Libraires et système
Pour fonctionner, seules les librairies standard `iostream`, `iomanip` et `cmath` sont nécessaires.
La dernière partie fonctionne dans un terminal compatible avec les séquences ANSI.

### La boucle de traitement en question
Dans cet exemple, je vais utiliser une boucle *range for*, mais afin d'avoir un indice incrémenté, je vais encapsuler la boucle (si *C++20* n'est pas disponible) pour que les informations liées uniquement à la boucle ne soient pas disponibles après.

{% highlight cpp %}
// Simplifié avec c++20
// for(size_t index = 0, max_value = std::distance(v.begin(), v.end()); auto value:v) 
{
	size_t index = 0;
	size_t max_value = std::distance(v.begin(), v.end());
	for(auto value:v) {
		const int pourcent = (++index)*100/max_value;
		std::cout << "Traitement en cours " << pourcent << "\%. \n";// (1)
		// Long traitement des valeurs
	}
}
// std::cout << index; <= Erreur
{% endhighlight %}

Améliorons la ligne `(1)`.

### Implémentation d'une barre de progression

Une première amélioration simple est de ne pas changer de ligne à chaque fois, mais de réécrire sur la ligne actuelle (`\r`) et de faire un `flush` pour éviter d'avoir des clignotements à l'écran (visible en `(4)`).

Afin d'avoir une information plus visuelle, créons une barre de progression.

{% highlight cpp %}
	const double resize = 0.25; // La barre 0.25 * 100 = 25 caractères
	std::cout << "Traitement en cours " << pourcent << "\%. |"
		<< std::string(std::ceil(percent*resize), '#') // (2)
		<< std::string(std::floor((100-percent)*resize), ' ') // (3)
		<< "| \r" << std::flush; // (4)
{% endhighlight %}

Pour avoir une taille bien définie, on peut utiliser deux chaînes de caractères dont la longueur totale sera un pourcentage la taille du vecteur à traiter.

En `(2)` on créer une barre indiquant la progression avec le constructeur de `std::string` permettant de définir une taille et le caractère répété.

En `(3)` on créer une autre chaîne dont la taille correspond au maximum de la barre moins la progression en cours.

Le résultat est le suivant
![Barre de progression]({{site.url}}/examples/CPP/2021-08_progress_bar/progress_bar.png "Barre de progression")

Il est possible d'apporter de la couleur grâce aux séquences ANSI.

### Les séquences ANSI
[Les séquences de contrôle ANSI](https://en.wikipedia.org/wiki/ANSI_escape_code) permettent de gérer non seulement la couleur des caractères mais également de déplacer le curseur dans le terminal.

Le saut de ligne (`\n`) ou la tabulation (`\t`) font parti de ces codes mais il en existe beaucoup d'autres que je ne détaillerai pas ici car seuls la gestion de la couleur m'intéresse par la suite.
Il est possible de tester les 8 couleurs compatibles avec la plupart des terminaux avec un shell *bash*.

{% highlight bash %}
for sdr in "\x1b[40m" "\x1b[41m" "\x1b[42m" "\x1b[43m" "\x1b[44m" "\x1b[45m" "\x1b[46m" "\x1b[47m]"; do echo -e $sdr"\tHello world\t\x1b[0m"; done
{% endhighlight %}

Ici, la chaîne `\x1b[` permet au terminal de savoir que ce qui se trouve avant `m` ne doit pas être affiché mais converti en une couleur.  Ce qui devrait donner le résultat suivant

![Test couleurs avec bash]({{site.url}}/examples/CPP/2021-08_progress_bar/bash_test_bgcolor.png "Test couleurs avec bash")

### Ajoutons de la couleur
Connaissant les séquences précédentes, on peut rapidement améliorer la boucle.

{% highlight cpp %}
	// Avant la boucle...
	const double resize = 0.25;
	constexpr auto bg_green = "\x1b[42m";
	constexpr auto bg_black = "\x1b[40m";
	constexpr auto reset = "\x1b[0m";
	
	// Dans la boucle...
	std::cout << "Traitement en cours " << pourcent << "\%. "
		<< bg_green // (5)
		<< std::string(std::ceil(percent*resize), ' ')
		<< bg_black // (6)
		<< std::string(std::floor((100-percent)*resize), ' ')
		<< reset // (7)
		<< "\r" << std::flush;
{% endhighlight %}

En `(5)` on change la couleur du fond du terminal, les chaînes suivantes auront donc un arrière plan vert.
En `(6)` on change la couleur en noir, puis on réinitialise en `(7)` afin de reprendre le défaut du terminal.

Le résultat est le suivant.
![ANSI progress bar]({{site.url}}/examples/CPP/2021-08_progress_bar/ANSI_progress_bar.gif "Barre de progression en couleur")

### Conclusion
Vous pouvez essayer par vous même le code disponible sur la page [github du site]({{site.git_origin}}/tree/main/examples/CPP/2021-08_progress_bar/) où plusieurs implémentations sont possible : avec ou sans *C++20* et avec ou sans *ANSI*.

Vous voulez réagir sur l'article ? Ouvrez une [issue sur github]({{site.git_origin}}/issues).
