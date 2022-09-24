# poll()
---
~~~cpp
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int délai);
~~~
- wait for some event on a file descriptor
- une variation de [[select - Webserv]] : il attend que l'un des fd parmi un ensemble soit prêt pour effectuer des entrées-sorties.
---
- L'ensemble des fd à surveiller est indiqué dans l'argument fds qui est un tableau de structures nfds du type :
~~~cpp
struct pollfd {
    int   fd;         /* Descripteur de fichier */
    short events;     /* Événements attendus    */
    short revents;    /* Événements détectés    */
};
~~~
 - Contenu de la struct : 
	- **fd** : contient fd ouvert
	- **events** : paramètre d'entrée -> un masque de bits indiquant les événements qui intéressent l'application. 
	- **revents** : paramètre de sortie -> rempli par le noyau avec les événements qui se sont effectivement produits
		- bits renvoyés dans revents peuvent inclure ceux spécifiés dans events, ou l'une des valeurs POLLERR, POLLHUP ou POLLNVAL. 

- Returns :
	-  Succes : positive représentant le **nombre de structures ayant un champ revents non nul = c'est-à-dire le nombre de structures pour lesquels un événement attendu, ou une erreur, s'est produit** 
	- Retour nul =  un dépassement du délai d'attente et qu'aucun descripteur de fichier n'était prêt. 
	- Error : appels renvoient -1, et errno contient le code d'erreur.  
---
## [[Bits event-revents - poll()]]
---
### [Source](http://manpagesfr.free.fr/man/man2/poll.2.html)
---
## [[Functions - Webserv (CPP)]]
## [[Webserv]]