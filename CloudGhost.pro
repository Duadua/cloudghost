TEMPLATE = app
TARGET = CloudGhost
QT += core opengl gui widgets
CONFIG += debug_and_release
DEFINES += _UNICODE _ENABLE_EXTENDED_ALIGNED_STORAGE WIN64 QT_DLL QT_OPENGL_LIB QT_WIDGETS_LIB

CONFIG(debug, debug|release){
    DESTDIR = ./x64/Debug
    INCLUDEPATH += ./generated \
        . \
        ./generated/Debug \
        ./tools \
        ./src/include \
        ./math/include \
        ./third_part/glfw/include \
        ./third_part/glew/include \
        ./third_part/assimp/include \
        ./tools \
        $$(VC_IncludePath) \
        $$(WindowsSDK_IncludePath)

    DEPENDPATH += . \
        ./third_part/glfw/libs \
        ./third_part/glew/libs \
        ./third_part/assimp/libs \
        $(VC_LibraryPath_x64) \
        $(WindowsSDK_LibraryPath_x64) \
        $(NETFXKitsDir)Lib\um\x64 \
        $$(QTDIR)/lib \
        $$(QTDIR)/bin

    LIBS += -lqtmaind \
        -lQt5Cored \
        -lQt5Guid \
        -lQt5OpenGLd \
        -lopengl32 \
        -lglu32 \
        -lQt5Widgetsd \
        -luser32 \
        -LD:/cpbr/cloudghost/third_part/glfw/libs/ -lglfw3d \
        -LD:/cpbr/cloudghost/third_part/glew/libs/ -llibglew32d \
        -LD:/cpbr/cloudghost/third_part/assimp/libs/ -lassimp-vc140-mt \

    MOC_DIR += ./generated/Debug
    OBJECTS_DIR += release
    UI_DIR += ./generated
    RCC_DIR += ./generated

} # debug
else {
    DESTDIR = ./x64/Release
    INCLUDEPATH += ./generated \
        . \
        ./generated/Release \
        ./tools \
        ./src/include \
        ./math/include \
        ./third_part/glfw/include \
        ./third_part/glew/include \
        ./third_part/assimp/include \
        ./tools \
        $$(VC_IncludePath) \
        $$(WindowsSDK_IncludePath)

    DEPENDPATH += . \
        ./third_part/glfw/libs \
        ./third_part/glew/libs \
        ./third_part/assimp/libs \
        $(VC_LibraryPath_x64) \
        $(WindowsSDK_LibraryPath_x64) \
        $(NETFXKitsDir)Lib\um\x64 \
        $$(QTDIR)/lib \
        $$(QTDIR)/bin

    LIBS += -lqtmain \
        -lQt5Core \
        -lQt5Gui \
        -lQt5OpenGL \
        -lopengl32 \
        -lglu32 \
        -lQt5Widgets \
        -luser32 \
        -LD:/cpbr/cloudghost/third_part/glfw/libs/ -lglfw3 \
        -LD:/cpbr/cloudghost/third_part/glew/libs/ -llibglew32 \
        -LD:/cpbr/cloudghost/third_part/assimp/libs/ -lassimp-vc140-mt

    MOC_DIR += ./generated/Release
    OBJECTS_DIR += release
    UI_DIR += ./generated
    RCC_DIR += ./generated

} # release

include(CloudGhost.pri)

RC_ICONS = ./resources/ui/icon.ico
