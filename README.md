# R3D Engine

R3D Engine est un moteur de rendu graphique qui s'appuie sur Vulkan. L'objectif est d'en apprendre
plus sur les moteurs de rendu et leur fonctionnement. C'est �galement l'occasion de d�couvrir Vulkan.

## Installation

### D�pendences

#### Vulkan SDK
T�l�chargez le version 1.2.131.2 de [Vulkan SDK](https://vulkan.lunarg.com/sdk/home) et installez la.

Rien n'emp�che d'utiliser une version sup�rieur ou ulterieur, mais ce n'est pas garranti que le projet compilera.

#### GLFW3
T�l�chargez le version 3.3.2 de [GLFW version 3](https://www.glfw.org/) et d�compressez le fichier.

#### GLM
T�l�chargez la derni�re version disponible de [GLM](https://glm.g-truc.net/0.9.9/index.html) et  d�compressez le fichier.

#### stb_image
T�l�chargez la derni�re version disponible de [stb](https://github.com/nothings/stb) depuis github. Vous pouvez supprimez tout les fichiers � l'exception
de stb_image.h.

stb_image sera ammen� a �tre implementer directement dans le projet.

#### tiny object loader
T�l�chargez la derni�re version disponible de [tiny object loader](https://github.com/syoyo/tinyobjloader) depuis github.

tiny object loader sera ammen� a �tre remplac�.

### Configuration du projet

Le projet a �t� d�velopp� et test� par Visual Studio uniquement. Il est donc recommander d'utiliser cet IDE.
Dans le dossier script/, lancez setup.bat � l'aide d'un terminal.

Trois nouvelles variables environnements que vous devrez modifi�.

GLM_PATH	: chemin vers votre installation de GLM
GLFW_PATH	: chemin vers votre installation de GLFW
R3DLIB_PATH	: chemin vers votre installation de vos include suppl�mentaire
ASSIMP_PATH	: chemin vers votre installation de ASSIMP