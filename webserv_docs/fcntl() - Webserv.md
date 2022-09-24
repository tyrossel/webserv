# fcntl()
---
~~~cpp
#include <unistd.h>
#include <fcntl.h>

int fcntl(int fd, int cmd, ...);
~~~
- permet de se livrer à l'une des opérations décrites plus loin sur le descripteur de fichier **fd**
- L'opération est déterminée par la valeur de **cmd**
- Liste des valeurs possibles de **cmd** (définit dans **<fcntl.h>**) :
	- **DUPLIQUER FD** : 
		- **F_DUPFD** : Trouver le plus petit numéro de descripteur libre supérieur ou égal à arg et le transformer en copie de fd
		- **F_DUPFD_CLOEXEC** : Comme pour un F_DUPFD, mais positionne en plus l'attribut « close-on-exec » pour le descripteur dupliqué (permet d'éviter une opération F_SETFD de fcntl() supplémentaire pour positionner l'attribut FD_CLOEXEC)
	- **ATTRIBUTS DU FD** :
		- **F_GETFD** : Lire les attributs du fd
		- **F_SETFD** : Positionner les attributs du fd avec la valeur spécifiée par arg
	- **ATTRIBUT D'ETAT DU FICHIER** :
		- **F_GETFL** : Lire les attributs d'état du fichier
		- **F_SETFL** : Positionner les nouveaux attributs d'état pour le fichier à la valeur indiquée par **arg**. 
			- Les modes d'accès au fichier (O_RDONLY, O_WRONLY, O_RDWR) et les attributs de création du fichier (c'est-à-dire O_CREAT, O_EXCL, O_NOCTTY, O_TRUNC) dans arg sont ignorés.
	- **VERROUILLAGE** : [docs ici](http://manpagesfr.free.fr/man/man2/fcntl.2.html)
	- **GESTION SIGNAUX**
		- **F_GETOWN** : Obtenir le PID ou l'ID du groupe de processus qui reçoit les signaux pour les événements concernant le **fd**
		- **F_SETOWN** : Définir le PID ou l'ID du groupe de processus qui recevront les signaux pour les événements concernant le **fd**
		- **F_GETSIG** : Renvoyer le numéro du signal émis lorsque l'entrée ou la sortie deviennent possibles
		- **F_SETSIG** : Indiquer le signal à émettre lorsque l'entrée ou la sortie deviennent possibles

- Return :
	- La valeur renvoyée par fcntl() varie suivant le type d'opération :
		- **F_DUPFD** : Le nouveau descripteur.
		- **F_GETFD** : La valeur des attributs.
		- **F_GETFL** : La valeur des attributs.
		- **F_GETLEASE** : Type de la tenue de bail sur le fd
		- **F_GETOWN** : Le propriétaire du fd
		- **F_GETSIG** : La valeur du signal envoyé lorsque la lecture ou l'écriture deviennent possibles, ou zéro pour le comportement SIGIO traditionnel.
		- **Toutes les autres commandes renvoient zéro.**
	- En cas d'échec -1 est renvoyé, auquel cas errno est renseigné en conséquence.  
---
### [Source](https://man7.org/linux/man-pages/man2/fcntl.2.html)
---
## [[Functions - Webserv (CPP)]]
## [[Webserv]]