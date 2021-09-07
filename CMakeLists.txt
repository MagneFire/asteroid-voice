cmake_minimum_required(VERSION 3.6.0)

project(asteroid-audiorecorder
	VERSION 0.0.1
	DESCRIPTION "Default flashlight app for AsteroidOS")

find_package(ECM REQUIRED NO_MODULE)
find_package(AsteroidApp REQUIRED)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ASTEROID_MODULE_PATH})

include(FeatureSummary)
include(GNUInstallDirs)
include(ECMQMLModules)
include(AsteroidCMakeSettings)
include(AsteroidTranslations)

find_package(Qt5 5.12.0 COMPONENTS Multimedia REQUIRED)

add_subdirectory(src)