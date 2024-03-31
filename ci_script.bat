@ECHO OFF

@REM Requirement for cmake:
@REM Windows system has a compiler for the qt framework,
@REM which is known in environment variables.
@REM gcovr: https://gcovr.com/en/stable/

SETLOCAL enabledelayedexpansion

SET cmake_generator=%1

SET pwd=%cd%
SET path_application=%pwd%\App
SET build_debug_dir=build_debug
SET build_release_dir=build_release

if "%~1"=="" (
    SET cmake_generator="MinGW Makefiles"
)
ECHO Using cmake generator: %cmake_generator%

SET coverage_threshold=80
SET coverage_report_path=%pwd%\%build_debug_dir%\Testing\coverage
SET coverage_array_idx=0
SET dir_coverage_array=
SET exclude_eoverage_array[0]=3rdparty
SET exclude_eoverage_array[1]=QtWrappers

SET exclude_array_len=0
:LoopSizeArr
if DEFINED exclude_eoverage_array[%exclude_array_len%] (
    SET /a exclude_array_len+=1
    GOTO :LoopSizeArr
)

ECHO Configure coverage path
for /d %%i in ("%path_application%\*") do (
    SET is_dir_exclude=false
    SET /a end_search_loop=%exclude_array_len%-1
    for /l %%j in (0,1,!end_search_loop!) do (
        if "%%~nxi" equ "!exclude_eoverage_array[%%j]!" (
            ECHO Exclude coverage directory: %path_application%\%%~nxi
            SET is_dir_exclude=true
        )
    )
    
    if !is_dir_exclude! equ false (
        ECHO Add coverage directory: %path_application%\%%~nxi
        SET dir_coverage_array[!coverage_array_idx!]=%path_application%\%%~nxi
        SET /a coverage_array_idx+=1    
    )
)

SET filter_coverage_command=
SET /a coverage_array_idx-=1
for /l %%i in (0,1,%coverage_array_idx%) do (
    SET dir_coverage_array[%%i]=!dir_coverage_array[%%i]:\=/!
    SET filter_coverage_command=!filter_coverage_command! --filter=!dir_coverage_array[%%i]!
)

if exist %build_debug_dir% (
    ECHO Remove build debug direcory
    rd /s /q %build_debug_dir%
)
if exist %build_release_dir% (
    ECHO Remove build release direcory
    rd /s /q %build_release_dir%
)

mkdir %build_debug_dir%
cd %build_debug_dir%

cmake -G %cmake_generator% -DCMAKE_BUILD_TYPE:STRING=Debug ..

if %ERRORLEVEL% equ 0 (
    ECHO Cmake configure success
) else (
    CALL :ExitWithCode "Error cmake configure with code" %ERRORLEVEL%
    GOTO :EOF
)

cmake --build . -j8

if %ERRORLEVEL% equ 0 (
    ECHO Cmake build success
) else (
    CALL :ExitWithCode "Error build with code" %ERRORLEVEL%
    GOTO :EOF
)

ctest --rerun-failed --output-on-failure

if %ERRORLEVEL% equ 0 (
    ECHO Tests running success
) else (
    CALL :ExitWithCode "Running tests end with code" %ERRORLEVEL%
    GOTO :EOF
)


SET coverage_report_txt=coverage_report.txt
mkdir %coverage_report_path%
gcovr -r .. ^
--txt-metric branch ^
--exclude-throw-branches ^
%filter_coverage_command% > %coverage_report_path%\%coverage_report_txt%
type %coverage_report_path%\%coverage_report_txt%

for /f "tokens=4" %%a in ('type %coverage_report_path%\%coverage_report_txt%') do (
    SET "actual_coverage=%%a"
)
SET actual_coverage=%actual_coverage:~0,-1%

if %actual_coverage% geq %coverage_threshold% (
    ECHO Coverage gate is satisfied
) else (
    CALL :ExitWithCode "Coverage gate is unfulfilled" -1
    GOTO :EOF
)

ECHO Generate coverage to html report
gcovr -r .. ^
--exclude-throw-branches ^
%filter_coverage_command% ^
--html-nested %coverage_report_path%\index.html

cd %pwd%
mkdir %build_release_dir%
cd %build_release_dir%

cmake -G %cmake_generator% -DCMAKE_BUILD_TYPE:STRING=Release ..

if %ERRORLEVEL% equ 0 (
    ECHO Cmake configure success
) else (
    CALL :ExitWithCode "Error cmake configure with code" %ERRORLEVEL%
    GOTO :EOF
)

cmake --build . -j8

if %ERRORLEVEL% equ 0 (
    ECHO Cmake build success
) else (
    CALL :ExitWithCode "Error build with code" %ERRORLEVEL%
    GOTO :EOF
)

CALL :ExitWithCode "ci_script procedure success code" %ERRORLEVEL%
EXIT /B %ERRORLEVEL%

:ExitWithCode
    ECHO %~1: %2
    cd %pwd%
    EXIT /B %2
