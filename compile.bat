@echo off
echo Building project...
cmake --preset=mingw-release >nul 2>&1
cmake --build --preset=mingw-release >nul 2>&1

echo.
echo Pokrećem testove:
echo ================
cd build
ctest --output-on-failure
cd ..

echo.
echo Build i Test GOTOVI! 
echo Aplikacija se nalazi u /build/bin