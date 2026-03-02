@echo off
set /p OUT_NAME="Unesite ime za ovaj benchmark (npr. baseline ili refactor): "

if "%OUT_NAME%"=="" (
    echo GRESKA: Morate uneti ime za output fajl.
    pause
    exit /b
)

:: Putanja do foldera gde su build fajlovi
set BUILD_DIR=cmake-build-release

echo ========================================
echo [1/3] Build-ovanje gbench targeta...
echo ========================================
cmake --build %BUILD_DIR% --target gbench -j 10

if %ERRORLEVEL% NEQ 0 (
    echo GRESKA: Build nije uspeo. Proveri CMake greske iznad.
    pause
    exit /b
)

echo.
echo ========================================
echo [2/3] Pokretanje benchmarkova (10x repeticija)...
echo ========================================

:: --benchmark_repetitions=10: Svaki test se vrti 10 puta za statisticku tackost
:: --benchmark_display_aggregates_only=true: U konzoli prikazuje samo Mean/StdDev/Median
:: --benchmark_out: U JSON fajl ipak idu SVI podaci (potrebno za U-test)

.\%BUILD_DIR%\benchmark\gbench.exe ^
  --benchmark_repetitions=10 ^
  --benchmark_min_time=2s ^
  --benchmark_display_aggregates_only=true ^
  --benchmark_enable_random_interleaving=true ^
  --benchmark_out=%OUT_NAME%.json ^
  --benchmark_out_format=json

echo.
echo ========================================
echo [3/3] Gotovo!
echo Rezultati sa 10 ponavljanja su u: %OUT_NAME%.json
echo ========================================
pause
