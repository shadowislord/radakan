--require("functions.lua")

---------------------------------------------------
function setdefines()
  if (windows) then
  
    -- disable VS2005 CRT security warnings
    if (options["target"] == "vs2005") then
        table.insert(package.defines, "_CRT_SECURE_NO_DEPRECATE")
    end
    
    table.insert(package.defines, "WIN32")
    table.insert(package.links, { "user32"})
      --table.insert(package.files, "../..src/resources.rc")
  end
end  
---------------------------------------------------
function setfiles(path) 
  core_files =
  {
      matchfiles(path .. "include/*.h"),
      matchfiles (path .. "src/*.cpp")
  }
  package.files = { core_files }  
  
    --package.excludes = {
    --  "dont_build_this.c"
    --}    
end
---------------------------------------------------
function setODElibpath()
    if (windows) then    
      table.insert(package.libpaths, "../../../ode/lib")
    end
end
---------------------------------------------------
function setDefaultPack()
      if (not options["sdk"] and not options["source"] and not options["cvs"]) then
        options["sdk"] = true
      end    
end
---------------------------------------------------
function setOGRElibpath()
    if (windows) then    
      if (options["sdk"]) then
        table.insert(package.libpaths, "$(OGRE_HOME)")
      elseif (options["source"]) then
        table.insert(package.libpaths, "../../../../../OgreMain/lib")
      elseif (options["cvs"]) then
        table.insert(package.libpaths, "../../../../../../OgreHead/OgreMain/lib")
      end
    end
end
---------------------------------------------------
function setbuildoptions()      
  if (linux) then
    package.buildoptions = { "-Wall" }
 end
 
  package.config["Debug"].buildflags = 
  { 
  
  }
  package.config["Release"].buildflags = 
  { 
      "optimize-speed", 
      "no-symbols", 
      "no-frame-pointer" 
  }	
  package.config["Debug"].objdir = "../obj/Debug"
  package.config["Release"].objdir = "../obj/Release"
end
---------------------------------------------------
function getsubScriptFolder()
  if (options["target"] == "vs6") then
    return "/scripts/VC6"
  end
  if (options["target"] == "vs2002") then
    return "/scripts/VC7"
  end
  if (options["target"] == "vs2003") then
    return "/scripts/VC7.1"
  end
  if (options["target"] == "vs2005") then
    return "/scripts/VC8"
  end 
  if (options["target"] == "vs2005") then
    return "/scripts/VC8"
  end 
  return ""
end
  
  
  