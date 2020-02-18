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
Téléchargez la dernière version disponible de [stb](https://github.com/nothings/stb) depuis github. Vous pouvez supprimez tout les fichiers à l'execption
de stb_image.h.

stb_image sera ammené a être implementer directement dans le projet.