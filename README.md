# R3D Engine

R3D Engine est un moteur de rendu graphique qui s'appuie sur Vulkan. L'objectif est d'en apprendre
plus sur les moteurs de rendu et leur fonctionnement. C'est également l'occasion de découvrir Vulkan.

## Installation

### Dépendences

#### Vulkan SDK
Téléchargez le version 1.2.131.2 de [Vulkan SDK](https://vulkan.lunarg.com/sdk/home) et installez la.

Rien n'empêche d'utiliser une version supérieur ou ulterieur, mais ce n'est pas garranti que le projet compilera.

#### GLFW3
Téléchargez le version 3.3.2 de [GLFW version 3](https://www.glfw.org/) et décompressez le fichier.

#### GLM
Téléchargez la dernière version disponible de [GLM](https://glm.g-truc.net/0.9.9/index.html) et  décompressez le fichier.

#### stb_image
Téléchargez la dernière version disponible de [stb](https://github.com/nothings/stb) depuis github. Vous pouvez supprimez tout les fichiers à l'exception
de stb_image.h.

stb_image sera ammené a être implementer directement dans le projet.

### Configuration du projet

Le projet a été développé et testé par Visual Studio uniquement. Il est donc recommander d'utiliser cet IDE.
Pour commencer à configurer le projet, fait un clique droit dans l'explorateur de solution sur R3D_Vulkan_Engine, 
puis allez dans propriétés.

Dans C/C++ -> Général -> Autre répertoire, ajoutez les liens vers le dossier include de Vulkan, et GLFW. Aujoutez également tout le dossier
de GLM ainsi que sbt_image.h.

Dans Editeur de liens -> Général -> Répertoire de bibliothèques supplémentaires,  ajoutez le dossier Lib32 de Vulkan et lib-vc2015 de GLFW.

Dans Editeur de liens -> entrée -> Dépendences supplémentaires, entrez vulkan-1.lib, glfw3.lib