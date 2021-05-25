<#
        Build script
#>
if(!(Test-Path(".\build\")))
{
        New-Item -Itemtype Directory "build"
}

$source_name = "..\code\*.cpp"
$executable_name = "game.exe"

$lib_path = "..\libs\"
$include_path1 = "..\code\headers\"
$include_path2 =  "..\code\extern\"

$compiler_flags = "/nologo", "/EHsc", "/Zi", "/FC"
# $linker_flags =

$libraries = "SDL2.lib", "SDL2main.lib", "SDL2_image.lib", "shell32.lib"

Push-Location .\build

#cl /MD $source_name /Fe$executable_name $compiler_flags /I$include_path /link /LIBPATH:$lib_path $libraries $linker_flags /SUBSYSTEM:console

cl $source_name /Fe$executable_name /I$include_path1 /I$include_path2  $compiler_flags /link /LIBPATH:$lib_path $libraries $linker_flags /SUBSYSTEM:console

Pop-Location

