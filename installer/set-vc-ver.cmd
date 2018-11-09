pushd "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Redist"
FOR /F "tokens=*" %%g IN ('dir /b MSVC') do (SET VC_redist_ver=%%g)
popd
