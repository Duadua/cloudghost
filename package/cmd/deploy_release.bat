@echo off

echo copy vs depends
robocopy ../depends/x64/release ../x64/release

echo copy resources
robocopy ../../resources ../x64/release/resources /E

:: Setting up environment for Qt usage...
set PATH=D:\qt\Qt5.12.0\5.12.0\msvc2017_64\bin;%PATH

echo copy qt depends
cd ../x64/Release
windeployqt CloudGhost.exe

pause
