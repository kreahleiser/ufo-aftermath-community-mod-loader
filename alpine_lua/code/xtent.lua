-- Aftermath Lua Plugin Engine ("ALPine")
-- File handling library extention
-- by Alchemist
-- Designed for ALPine Release 2.8.x
-- 20th December 2003

-- Functions in this library:
-- filename	findcfgvfs	(path)  
-- path         getcwd          ()




-- Returns filename of custom *cfg.vfs file at specified path
-- If no files found default value "cfg.vfs" is returned to prevent
-- possible nil or empty string occurence in the command lines
if findcfgvfs == nil then 
	function findcfgvfs(path)
	        local filename
	        local list

                local listname = os.getenv("TEMP")
                if listname == nil then listname = os.getenv("TEMP") end
                if listname == nil then listname = ".\\" end
		listname = listname .. os.tmpname() .. "~"

                -- Alchemist: list of all *cfg.vfs in the specified directory
                -- if cfg.vfs exists it will be placed first
		cmd = "dir /B \"" .. path .. "\\cfg.vfs\" > \"" .. listname .. "\""
		os.execute(cmd)
		cmd = "dir /B \"" .. path .. "\\mod.vfs\" >> \"" .. listname .. "\""
         	os.execute(cmd)
		cmd = "dir /B \"" .. path .. "\\*cfg.vfs\" >> \"" .. listname .. "\""
         	os.execute(cmd)
		cmd = "dir /B \"" .. path .. "\\mod*.vfs\" >> \"" .. listname .. "\""
         	os.execute(cmd)

		-- Alchemist: first filename from the list will be used
		-- this criteria should be enough for now
         	list = io.open(listname, "r")
         	filename = list:read("*l")
                list:close()
                os.remove(listname)

         	-- Alchemist: Let's cause "File not found" error
         	-- instead of nil string concatenation attempt
         	if filename == nil or filename == "" then
         		filename = "cfg.vfs"
         	end

                return filename
	end
end


-- Get current working directory
-- On success returns absolute path including drive letter,
-- on error returns nil
if getcwd == nil then 
	function getcwd()
	        local list
	        local line
	        local pos

                local tempname = os.getenv("TEMP")
		if tempname == nil then tempname = os.getenv("TMP") end
                if tempname == nil then tempname = ".\\" end
		tempname = tempname .. os.tmpname() .. "~"

		-- Alchemist: trick to current path
		-- Works fine in most cases
		os.execute("cd > \"" .. tempname .. "\"")

         	list = io.open(tempname, "r")
                -- Alchemist: added to prevent possible indexing of nil value
         	if list ~= nul then
	         	line = list:read("*l")
 	                list:close()
        	        os.remove(tempname)
        	end

                return line
        end
end
