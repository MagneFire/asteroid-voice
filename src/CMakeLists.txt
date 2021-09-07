
set(SRC
main.cpp
audiorecorder.cpp
audioiodevice.cpp
)

set(HDR
audiorecorder.h
audioiodevice.h
)

add_executable(asteroid-audiorecorder ${SRC} ${HDR} resources.qrc)




target_link_libraries(asteroid-audiorecorder
PRIVATE
Qt5::Qml
Qt5::Quick
Qt5::Multimedia
AsteroidApp)