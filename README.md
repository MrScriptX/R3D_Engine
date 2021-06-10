![Linter](https://github.com/MrScriptX/R3D_Engine/actions/workflows/linter.yml/badge.svg) ![GCC Build](https://github.com/MrScriptX/R3D_Engine/actions/workflows/gcc-build.yml/badge.svg) [![Codacy Badge](https://app.codacy.com/project/badge/Grade/79a0d84088bb4a20b72eeb81d4920f04)](https://www.codacy.com/gh/MrScriptX/R3D_Engine/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=MrScriptX/R3D_Engine&amp;utm_campaign=Badge_Grade)

# R3D Engine

R3D Engine est un moteur de rendu graphique qui s'appuie sur Vulkan. L'objectif est d'en apprendre plus sur les moteurs de rendu et leur fonctionnement et leur architecture. C'est également l'occasion de découvrir l'API [Vulkan](https://www.vulkan.org/).

R3D Engine est un moteur très primitif. En aucun cas il n'est utilisable en production. C'est un moteur bac à sable pour les débutants.

Vous pouvez suivre l'avancement du projet sur mon [blog](https://mrscriptx.github.io/).

## Installation

### Dépendances

#### Vulkan SDK
Téléchargez la version 1.2.131.2 de [Vulkan SDK](https://vulkan.lunarg.com/sdk/home) et installez-la.

### Autres
Toutes les dépendances sont disponibles dans le dossier dependencies. Vous pourrez y trouver :
- Assimp 5.0.1
- GLEW 2.1.0
- GLFW 3.3.4
- GLM
- STB IMAGE

Il faut les inclure à votre projet. Rajouter également le fichier assimp-vc142-mtd.dll à la racine de votre projet.

### Compilation
Compilez le projet en utilisant Visual Studio 2019. Vous obtiendrez une bibliothèque statique Engine.lib dans le dossier x64/Debug. Tous les fichiers d'en-tête se trouvent dans le dossier Engine/Includes.

### Assets
Le dossier assets contient les fichiers de shaders et est obligatoire. Ajoutez le dossier à la racine de votre projet.