#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

WinWait,  DirectX window,, 10,
WinActivate, DirectX window ; Launches DirectX Window.
;WinMove, DirectX window,, 2020, 100, DEFAULT, DEFAULT
