@echo off
setlocal enabledelayedexpansion

set "FILE1=%~1"
set "FILE2=%~2"

:: Provjera jesu li unesena dva imena
if "%FILE2%"=="" (
    echo [GRESKA] Upotreba: compare_bench.bat ime1 ime2
    echo Primer: compare_bench.bat baseline refactor
    pause
    exit /b
)

set "BUILD_BENCH_DIR=cmake-build-release\benchmark"
set "PY_SCRIPT=benchmark\lib\tools\compare.py"

echo ========================================
echo Poredim: %FILE1%.json VS %FILE2%.json
echo Lokacija: %BUILD_BENCH_DIR%
echo ========================================

:: Provjera postojanja JSON fajlova - koristimo goto umjesto zagrada za if blokove radi sigurnosti
if not exist "%BUILD_BENCH_DIR%\%FILE1%.json" goto :err_file1
if not exist "%BUILD_BENCH_DIR%\%FILE2%.json" goto :err_file2
if not exist "%PY_SCRIPT%" goto :err_script

echo Pokretanje poredjenja...
python "%PY_SCRIPT%" benchmarks "%BUILD_BENCH_DIR%\%FILE1%.json" "%BUILD_BENCH_DIR%\%FILE2%.json" --utest
:: Provjera error levela bez koristenja zagrada (da se ne bi desio isti bug)
if %ERRORLEVEL% EQU 0 goto :success
    echo.
    echo [INFO] Skripta je vratila gresku %ERRORLEVEL%
    echo Ako nema tabele, proveri: pip install pandas scipy
    goto :end

:err_file1
    echo [GRESKA] Ne vidim %FILE1%.json u %BUILD_BENCH_DIR%
    goto :end

:err_file2
    echo [GRESKA] Ne vidim %FILE2%.json u %BUILD_BENCH_DIR%
    goto :end

:err_script
    echo [GRESKA] Ne vidim compare.py na %PY_SCRIPT%
    goto :end

:success
    echo.
    echo ========================================
    echo Poredjenje uspesno zavrseno.
    echo ========================================

:end
pause
