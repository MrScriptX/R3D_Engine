# R3D Engine

R3D Engine est un moteur de rendu graphique qui s'appuie sur Vulkan. L'objectif est d'en apprendre
plus sur les moteurs de rendu et leur fonctionnement. C'est également l'occasion de découvrir Vulkan.

## Installation

### Dépendences

#### Vulkan SDK
Téléchargez le version 1.2.131.2 de (https://vulkan.lunarg.com/sdk/home)[Vulkan SDK] et installez la.

Rien n'empêche d'utiliser une version supérieur ou ulterieur, mais ce n'est pas garranti que le projet compilera.

#### GLFW3
Téléchargez le version 3.3.2 de (https://www.glfw.org/)[GLFW version 3] et décompressez le fichier.

#### GLM
Téléchargez la dernière version disponible de (https://glm.g-truc.net/0.9.9/index.html)[GLM] et  décompressez le fichier.

#### stb_image
Téléchargez la dernière version disponible de (https://github.com/nothings/stb)[stb] depuis github. Vous pouvez supprimez tout les fichiers à l'execption
de stb_image.h.

stb_image sera ammené a être implementer directement dans le projet.