package.name = "OgreOde_Prefab"

--------------------------
-- Package Build Settings
  package.language = "c++"
  package.kind = "dll"    
    
    package.config["Debug"].target = "prefab/lib/debug/"
    package.config["Release"].target = "prefab/lib/release/"
  
  setdefines()
  setbuildoptions()
  
--------------------------
-- Libraries
   --OgreOde_Core
    package.libpaths = 
    { 
        "../../../../lib" 
    }
    setODElibpath()
    setOGRElibpath()
    
  package.links = {  }
  package.config["Debug"].links = 
  { 
      "OgreOde_Core_d", 
      "oded", 
      "OgreMain" 
  }
  package.config["Release"].links = 
  { 
      "OgreOde_Core", 
      "ode", 
      "OgreMain" 
  }
  
--------------------------
-- Includes
  package.includepaths =
  {
    "../../../ode/include",
    "../../../include",
    "../../include"
  }
  
--------------------------
-- Files
  package.path = "../prefab/" .. getsubScriptFolder()
  setfiles("../../")
