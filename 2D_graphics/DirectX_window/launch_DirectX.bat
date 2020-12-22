echo off
taskkill /FI "IMAGENAME eq DirectX_window.exe"
timeout /t 0 /nobreak > NUL
cd 2D_graphics\DirectX_window
start DirectX_window.exe
start showDirectX.exe