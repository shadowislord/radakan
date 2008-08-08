premake --sdk --clean --target vs2005

exit

premake --sdk --clean --target vs6
premake --cvs --clean --target vs6
premake --source  --clean --target vs6

premake --sdk --clean --target vs2002
premake --cvs --clean --target vs2002
premake --source  --clean --target vs2002

premake --sdk --clean --target vs2003  
premake --cvs --clean --target vs2003
premake --source  --clean --target vs2003

premake --sdk --clean --target vs2005 
premake --cvs --clean --target vs2005
premake --source  --clean --target vs2005  

premake --cvs --clean --target gnu

