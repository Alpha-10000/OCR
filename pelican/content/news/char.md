Title: Détection de caractères 
Date: 2014-10-14
Category: News
Tags: caractères
Author: Nicolas
Slug: chars

Une fois les lignes reconnues, il faut isoler les caractères sur chacune 
d'entre elles.

### Histogramme

Comme pour la détection de lignes, nous utilisons un histogramme cette fois-ci 
horizontal. Nous parcourons la ligne de gauche à droite, et nous récupérons la 
présence ou non d'un pixel noir sur la colonne au travers d'un tableau binaire.

### Détection des caractères
De même, le passage d'une colonne blanche à une colonne ayant un pixel noir 
indique le début d'un caractère, et inversement.

### Mémorisation des lignes et de ses caractères

Pour faciliter le stockage des différentes données, nous avons choisi de 
définir une structure Block contenant le SDL_Rect représentant la ligne, le 
nombre de caractères de celle-ci ainsi qu'un tableau de SDL_Rect représentant 
ces derniers.
