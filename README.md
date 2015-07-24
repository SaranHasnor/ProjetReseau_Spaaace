# ProjetReseau_Spaaace

Am�liorations depuis la pr�sentation:
- Plus de crash li� � OpenGL
- Un vaisseau un peu moins horrible avec des textures
- Ajout d'une interface de connexion et d'un bouton de d�connexion
- R�solution de plusieurs probl�mes li�s au gameplay et au r�seau

Probl�mes connus:
- Lorsque trop de clients sont connect�s, les messages envoy�s peuvent s'empiler et causer un crash � la d�serialization (solution: utiliser UDP)
- Si une tentative de connexion �choue, il semble �tre n�cessaire de relancer le programme pour pouvoir tenter de se reconnecter (solution: v�rifier _checkSocketWritable dans network_tcp.c)
- Une d�synchronisation peut s'effectuer entre ce que le client sait de lui-m�me et les informations que poss�dent le serveur. Le client n'�crase ses propres informations au profit de celles du serveur que si celles-ci diff�rent grandement, c'est � dire une diff�rence de position importante ou des points de vie diff�rents (solution: donner un peu plus de libert� au client en lui permettant d'envoyer au serveur sa position plut�t que seulement son input, et r�duire le serveur au r�le de v�rification)
- Lors d'une d�connexion d'un client par timeout, le serveur ne supprime pas toujours les informations du joueur, et ne communique pas de cette d�connexion aux autres clients (solution: changer quelques points de la structure de la partie r�seau pour permettre de remonter plus d'informations � la partie gameplay)
- Si vous vous rapprochez des �toiles elles disparaissent, et c'est tr�s grave