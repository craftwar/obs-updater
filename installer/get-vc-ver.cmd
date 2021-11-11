SET VC_redist_path="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Redist"
pushd "%VC_redist_path%"
FOR /F "tokens=*" %%g IN ('dir /b MSVC') do (SET VC_redist_ver=%%g)
popd
