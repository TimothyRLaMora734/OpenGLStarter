@echo off
cd %~dp0
REM font2bitmap -i %1 -o "%~d1%~p1%~n1" -m s -s 60 -c charset.utf8 -p 5
font2bitmap -i %1 -o "%~d1%~p1%~n1" -m opengl -s 60 -c charset.utf8 -p 5
REM font2bitmap -i %1 -o "%~d1%~p1%~n1" -m mixed -s 60 -c charset.utf8 -p 5
pause