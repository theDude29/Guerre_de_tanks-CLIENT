#Si vous rencontrez des problèmes de avec ce fichier CMakeLists.txt
#merci de me le faire savoir : remi.perenne@free.fr

cmake_minimum_required(VERSION 2.6)
 
project(GuerreDeTank_Client)
 
# Trouver un package avec pkg-config 
find_package(PkgConfig)

pkg_check_modules(Irrlicht REQUIRED)
include_directories(${Irrlicht_INCLUDE_DIRS})
link_directories(${Irrlicht_LIBRARIES})

pkg_check_modules(sfml-network REQUIRED)
include_directories(${sfml-network_INCLUDE_DIRS})
link_directories(${sfml-network_LIBRARIES})

pkg_check_modules(sfml-audio REQUIRED)
include_directories(${sfml-audio_INCLUDE_DIRS})
link_directories(${sfml-audio_LIBRARIES})

pkg_check_modules(sfml-system REQUIRED)
include_directories(${sfml-system_INCLUDE_DIRS})
link_directories(${sfml-system_LIBRARIES})

 
add_executable(
    GuerreDeTank_Client
    BoutonManette.cpp
    client.cpp
    decor.cpp
    Explosion.cpp
    GestionBruits.cpp
    GestionExplosion.cpp
    infoPartie.cpp
    infoPositionRotationJoueur.cpp
    Joueur.cpp
    main.cpp
    MyCamera.cpp
    MyEventReceiver1.cpp
    MyEventReceiver2.cpp
    MyEventReceiver.cpp
    MyGui.cpp
    Shoot.cpp
    TankNormal.cpp
    vector3d_operateur.cpp
)
 
target_link_libraries(
    GuerreDeTank_Client
    Irrlicht
    GL
    sfml-graphics
    sfml-window
    sfml-audio
    sfml-network
    sfml-system
)
