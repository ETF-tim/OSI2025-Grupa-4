@echo off
echo Building project...
cmake --preset=mingw-release >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration FAILED!
    exit /b 1
)

cmake --build --preset=mingw-release >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Build FAILED!
    exit /b 1
)

echo.
echo Pokrećem testove:
echo ================
cd build
ctest --output-on-failure
set TEST_RESULT=%ERRORLEVEL%
cd ..

if %TEST_RESULT% NEQ 0 (
    echo.
    echo =============================
    echo TESTOVI SU PALI! Build failed.
    echo =============================
    exit /b %TEST_RESULT%
)

echo.
echo ========================================
echo Build i Test GOTOVI! Sve testove PROŠLI
echo ========================================
echo Aplikacija se nalazi u /build/bin