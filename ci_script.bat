@ECHO OFF

@REM Requirement for cmake:
@REM Windows system has a compiler for the qt framework,
@REM which is known in environment variables.

SET cmake_generator=%1

SET pwd=%cd%
SET build_dir=%pwd%\build

if "%~1"=="" (
    SET cmake_generator="MinGW Makefiles"
)
ECHO Using cmake generator: %cmake_generator%

if exist build (
    ECHO Remove build direcory
    rd /s /q build
)

mkdir %build_dir%
cd %build_dir%

cmake -G %cmake_generator% ..

if %ERRORLEVEL% equ 0 (
    echo Cmake configure success
) else (
    CALL :ExitWithCode "Error cmake configure with code" %ERRORLEVEL%
    GOTO :EOF
)

cmake --build . -j

if %ERRORLEVEL% equ 0 (
    echo Cmake build success
) else (
    CALL :ExitWithCode "Error build with code" %ERRORLEVEL%
    GOTO :EOF
)

ctest --rerun-failed --output-on-failure

if %ERRORLEVEL% equ 0 (
    echo Tests running success
) else (
    CALL :ExitWithCode "Running tests end with code" %ERRORLEVEL%
    GOTO :EOF
)

CALL :ExitWithCode "ci_script procedure success code" %ERRORLEVEL%
EXIT /B %ERRORLEVEL%

:ExitWithCode
    ECHO %~1: %2
    cd %pwd%
    EXIT /B %2
