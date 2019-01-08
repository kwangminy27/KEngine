cd %~dp0

xcopy Engine\Inc\*.h _Engine\Inc\ /d /s /y /exclude:ignore.txt
xcopy Engine\Inc\*.inl _Engine\Inc\ /d /s /y /exclude:ignore.txt
xcopy Engine\Bin\*.* Client\Bin\ /d /s /y /exclude:ignore.txt
