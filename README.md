# Projet final C — Space Invaders (SDL2) - Règles du jeu

Le but du jeu est de détruire les ennemis qui descendent sur l'écran en leur tirant dessus avant que ceux-ci n'atteignent le bas de la page.

## Types d'ennemis

- La "horde", constituée d'ennemis bleu foncé disposés en ligne. Ces ennemis se déplacent sur le côté et descendent progressivement. Ces ennemis ont la possibilité de tirer et le joueur doit éviter leurs projectiles. La vitesse de la horde accélère au cours de la partie. Certains ennemis de la horde sont plus résistants et nécessitent d'être touchés deux fois pour être éliminés (cela représente une ligne d'ennemis sur cinq). Si la horde atteint le joueur, celui-ci a perdu. La horde est constituée de 20 lignes de 8 ennemis.

- Les "ennemis rapides" qui se déplacent vers le bas et qui comme leur nom le sous-entend se déplacent plus rapidement que la horde. Ils apparaissent aléatoirement en haut de la grille et descendent vers le joueur. Si un ennemi rapide atteint le bas de l'écran, le joueur perd une vie.

## Vies

Le joueur commence la partie avec 3 vies symbolisées par des cœurs. Il perd une vie s'il est touché par un projectile de la horde ou si un ennemi rapide atteint le bas de l'écran. Si le nombre de vies atteint 0, le joueur a perdu. Cependant des cœurs peuvent apparaître en haut de l'écran et descendre vers le joueur. Si ce dernier en attrape un, il gagne une vie supplémentaire. Notez que des cœurs peuvent apparaître uniquement si le joueur a strictement moins de 5 vies.

Le joueur une perd vie si : 
- le joueur est touché par un projectile de la horde
- un ennemi rapide atteint le bas de l'écran

## Fin de la partie

La partie s'arrête dès que le joueur a gagné ou perdu, un message s'affiche alors à l'écran.

Pour gagner, le joueur doit éliminer tous les ennemis de la horde.

Deux conditions peuvent entraîner la défaite du joueur : 
- Le nombre de vies tombe à zéro
- Un ennemi de la horde atteint le bas de l'écran
