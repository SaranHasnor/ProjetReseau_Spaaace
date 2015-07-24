# ProjetReseau_Spaaace

Améliorations depuis la présentation:
- Plus de crash lié à OpenGL
- Un vaisseau un peu moins horrible avec des textures
- Ajout d'une interface de connexion et d'un bouton de déconnexion
- Résolution de plusieurs problèmes liés au gameplay et au réseau

Problèmes connus:
- Lorsque trop de clients sont connectés, les messages envoyés peuvent s'empiler et causer un crash à la déserialization (solution: utiliser UDP)
- Si une tentative de connexion échoue, il semble être nécessaire de relancer le programme pour pouvoir tenter de se reconnecter (solution: vérifier _checkSocketWritable dans network_tcp.c)
- Une désynchronisation peut s'effectuer entre ce que le client sait de lui-même et les informations que possèdent le serveur. Le client n'écrase ses propres informations au profit de celles du serveur que si celles-ci diffèrent grandement, c'est à dire une différence de position importante ou des points de vie différents (solution: donner un peu plus de liberté au client en lui permettant d'envoyer au serveur sa position plutôt que seulement son input, et réduire le serveur au rôle de vérification)
- Lors d'une déconnexion d'un client par timeout, le serveur ne supprime pas toujours les informations du joueur, et ne communique pas de cette déconnexion aux autres clients (solution: changer quelques points de la structure de la partie réseau pour permettre de remonter plus d'informations à la partie gameplay)
- Si vous vous rapprochez des étoiles elles disparaissent, et c'est très grave