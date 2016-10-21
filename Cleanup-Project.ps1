Get-ChildItem -Recurse -Filter *.vcxproj.user | Remove-Item -Force -Recurse
Get-ChildItem -Recurse -Filter *.vcxproj | Remove-Item -Force -Recurse
Get-ChildItem -Recurse -Filter *.vcxproj.filters | Remove-Item -Force -Recurse
Get-ChildItem -Recurse -Filter CMakeCache.txt | Remove-Item -Force -Recurse
Get-ChildItem -Recurse -Directory -Filter CMakeFiles | Remove-Item -Force -Recurse
Get-ChildItem -Recurse -Filter cmake_install.cmake | Remove-Item -Force -Recurse
Get-ChildItem -Recurse -Filter *.sln | Remove-Item -Force -Recurse
Get-ChildItem -Recurse -Directory -Filter bin | Remove-Item -Force -Recurse
Get-ChildItem -Recurse -Filter *.VC.db | Remove-Item -Force -Recurse
Get-ChildItem -Recurse -Filter *.VC.opendb | Remove-Item -Force -Recurse
Get-ChildItem -Recurse -Directory -Filter x64 | Remove-Item -Force -Recurse
Get-ChildItem -Recurse -Directory *.dir | Remove-Item -Force -Recurse
Get-ChildItem -Recurse -Directory .vs | Remove-Item -Force -Recurse