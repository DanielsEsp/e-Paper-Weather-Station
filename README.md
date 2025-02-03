# e-Paper-Weather-Station

Nouvelle version basée sur la base Open Metéo Cast déja publiée pour écran Tft 480 x 320.
Ici c'est un écran e-Paper 792 x 272 avec la possibilité de rafraîchissement partiel.

![20250202_080831](https://github.com/user-attachments/assets/2d5db50a-844e-4ad5-a21f-0a30ed620f86)


https://github.com/user-attachments/assets/95395337-b37f-4af3-a58e-0604fe9d19b4


Matériel utilisé

e-Paper 5,79 pouces avec une résolution de 792 x 272 et Wemos LOLIN 32
Un grand merçi au site 
https://www.makerguides.com/lighting-displays/
Pour la publication d'articles relatifs à la mise en oeuvre d'écrans e-Paper.

La récupération des données météo sur le site open-meteo qui publie les datas accessible au format Json et sans API-KEY
Pour les alertes le site weatherapi accessible après enregistrement et delivrance d'une API-KEY gratuite pour un nombre limité de call par jour, mais amplement suffisant pour une utilsation privée.

Initialisation de l'horloge par serveur NTP au démarrage ou reboot puis utilisation de l'horloge RTC de l'ESP.
La grille écran se fait en rafraîchissement total et à chaque changement de date.
Mise à jour de l'heure par réveil du mode Light Sleep toutes les 55 secondes.
Accès au seveur Open-Météo toute les 5 minutes pour relevé des datas météo. 
Et mise à jour uniquement des datas ayant changés. En mode rafraîchissement partiel.
Une requête chaque heure pour mise en mémoire de la pression h-1 et affichage de la pression actuelle.
A chaque heure accès à l'API weatherapi pour interrogation champs "alert", si alerte en cours, affichage en zone datas vent.



