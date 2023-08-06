@echo off

Echo Running refresh
call vendor\bin\premake\premake5.exe vs2019
Echo Complete!

timeout 5