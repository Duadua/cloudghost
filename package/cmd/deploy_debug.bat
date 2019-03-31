@echo off

echo copy vs depends
robocopy ../depends/x64/debug ../x64/debug

echo copy resources
robocopy ../../resources ../x64/debug/resources /E

:: Setting up environment for Qt usage...
set PATH=D:\qt\Qt5.12.0\5.12.0\msvc2017_64\bin;%PATH

echo copy qt depends
cd ../x64/Debug
windeployqt CloudGhost.exe

pause

