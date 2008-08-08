require("functions.lua")
  
  project.name = "OgreOde"

  project.bindir = "../"  
  project.libdir = "../"  
  
-- Project options
  addoption("sdk",    "Use SDK folder hierarchy")
  addoption("cvs",    "Use CVS  folderhierarchy")
  addoption("Source", "Use Source zip  folder hierarchy")
  -- make sure at least one is selected.
  setDefaultPack()
  
-- Set the output directories
  project.path = "../"
  

-- Build packages
  dopackage("OgreOde_Core.lua")
  dopackage("OgreOde_Prefab.lua")
  dopackage("OgreOde_Demos.lua")

-- Remove all intermediate files
  function doclean(cmd, arg)  
    docommand(cmd, arg)    
    os.rmdir("../lib/")
    os.rmdir("../obj")
  end


