Title: Détection de lignes 
Date: 2014-10-06
Category: News
Tags: lignes
Author: Benzaiten
Slug: lignes

Pour que notre O.C.R puisse reconnaître du texte, il faut d'abord situer ce 
dernier. Dans cette partie, nous travaillons sur une image dite "parfaite", 
issue du processus pré-traitement. Dans un premier temps, nous devons trouver 
l'emplacement des différentes lignes du texte.

### Histogramme

Afin de trouver nos lignes, nous utilisons ce qu'on appelle un histogramme 
vertical de l'image. En effet, nous parcourons l'image de haut en bas et pour 
chaque ligne, nous comptons le nombre de pixels noirs sur cette dernière. 
Nous obtenons ainsi la répartition du texte de l'image selon l'axe y.

### Détection des lignes

Grâce à cet histogramme, nous pouvons aisément distinguer les lignes de texte. 
Chaque ligne de texte est, en effet, séparée de la suivante par au moins une 
ligne blanche, c'est à dire sans pixel noir.

### Optimisation

Tout d'abord, nous avons commencé par travailler avec un seuil de nombre de 
pixels noirs, à partir duquel nous considérions une ligne comme faisant 
partie d'une ligne de texte. Or, notre image étant parfaite, la présence d'un 
unique pixel noir indique une ligne de texte. Ainsi, nous avons optimisé 
l'algorithme de l'histogramme afin de passer à la ligne suivant si un pixel 
noir était détecté.

### Mémorisation des composantes d'une ligne

Afin d'avoir toutes les composantes d'une ligne de texte, c'est-à-dire ses 
coordonnée x et y ainsi que sa largeur et sa hauteur, nous utilisons la 
structure SDL_Rect de la bibliothèque SDL qui répond parfaitement à nos 
attentes.
