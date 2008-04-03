-- Here are the lists of tests to build. Add/remove new
-- tests here and everything else should just work

  local Demos =
  {
    "GranTurismOgre",
    "LandScape",
    "SimpleScenes"
  }

-- Factory function for test packages
  function makeDemos(index, name)
  
    package = newpackage()    
    --------------------------
    -- Package Build Settings
    package.name = "OgreOde_Demo_" .. name
    package.language = "c++"
    package.kind = "exe"
            
    package.config["Debug"].target = "Demos/" .. name .. "/bin/debug/"
    package.config["Release"].target = "Demos/" .. name .. "/bin/release/"
           
    setdefines()
    setbuildoptions()
  
    --------------------------
    -- Libraries
   --OgreOde_Core && OgreOde_Prefab
    package.libpaths = 
    { 
        "../../../../ode/lib",
        "../../../../lib",
        "../../../../prefab/lib"
    }
    setODElibpath()
    setOGRElibpath()
    
    package.links = {  }
    package.config["Debug"].links = 
    { 
          "OgreOde_Core_d", 
          "OgreOde_Prefab_d", 
          "OgreMain_d"       
    }           
    package.config["Release"].links = 
    { 
          "OgreOde_Core", 
          "OgreOde_Prefab", 
          "OgreMain" 
    }
          
    --------------------------
    -- Includes
    package.includepaths = 
    { 
        "../../include" ,
        "../../../../include",
        "../../../../prefab/include",
        "../../../../ode/include" 
    }
    
    --------------------------
    -- Files
    package.path = "../Demos/" .. name  .. getsubScriptFolder()
    setfiles("../../")
  end

table.foreach(Demos, makeDemos)

