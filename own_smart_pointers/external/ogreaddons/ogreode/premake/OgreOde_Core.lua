package.name = "OgreOde_Core"

--------------------------
-- Package Build Settings
  package.language = "c++"
  package.kind = "dll"    
  
    package.config["Debug"].target = "lib/debug/"
    package.config["Release"].target = "lib/release/"
    
    
  setdefines()
  setbuildoptions()
    
--------------------------
-- Libraries
  package.libpaths = {  }
  setODElibpath()
  setOGRElibpath()
    
    
  package.links = {  }
  package.config["Debug"].links = 
  { 
      "oded", 
      "OgreMain" 
      
  }
  package.config["Release"].links = 
  { 
      "ode", 
      "OgreMain" 
  }
--------------------------
-- Includes
  package.includepaths =
  {
    "../../ode/include",
    "../../include"
  }
    
--------------------------
-- Files

  --
  
  
  package.path = "../" .. getsubScriptFolder()
  setfiles("../../")