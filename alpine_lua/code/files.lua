-- Aftermath Lua Plugin Engine ("ALPine")
-- File handling library
-- Written by Andrew 'Fulby' Campbell
-- Release 2.8
-- 17th December 2003

-- Functions in this library:
-- 				needs			(filename)
-- items		loaditems 		(filetype, name)
-- 				saveitems		(filetype, name, items)
-- 				needsl			(filename)
-- items		loaditemsl	 	(filetype, name)
-- 				saveitemsl		(filetype, name, items)
-- items		loadsub			(filetype, name)
-- 				savesub			(filetype, name, items)
-- items		loadsube		(filetype, name)
-- langindex	loadlangindex	()
-- 				savelangindex	(langindex)
-- items		loaditemse		(filetype, name)
-- list			loadtech		(filename)
-- 				savetech		(filename, list)
-- list			loadteche		(filename)
-- list			loadequip		(filename)
-- 				saveequip		(filename, list)
-- list			loadenemies		(filename)
-- 				saveenemies		(filename, list)

--				writesubinv		(outfile, indent, name, subinv)
-- 				changeline		(file, search, replace)
-- 				copyfile		(sourcename, dest, vfs)
-- 				createdirectory	(dest, vfs)



-- Check filename exists, if not extract it from gamedata.vfs
-- if file does not exist and cannot be extracted, exit plugin script
-- filename	file to check, path is relative to root of VFS
function needs (filename)
	-- convert to absolute path
	local abspath = PATH .. "\\" .. filename
	GDFileAdded = true -- Gamedata file added so create cfg.vfs

	-- check if file exists in TDT
	local f = io.open(abspath)
	if f ~= nil then
		f:close()
		return
	end

	-- if not, create directories and extract from gamedata.vfs
	local lastslash = 0
	for i = 1, string.len(filename) do
		if string.byte(filename, i) == 92 then lastslash = i end
	end
	local filepath = string.sub(filename, 1, lastslash)
	createdirectory(filepath)

	local params = "se \"" .. GAMEPATH .. "\\gamedata.vfs\" \""
	.. filename .. "\" \"" .. PATH .. "\\" .. string.sub(filepath, 1, -2) .. "\""
	if vfs ~= nil then vfs(params) end

	-- Alchemist: Win98 console problem workaround
	-- Let's make command string shorter and produce
	-- external batch file which will do all the rest
	-- f should now exist in TDT
	f = io.open(abspath)
	if f == nil then
		local batname = "extract.bat"
		local bat = io.open(batname, "w+")
		local exec = "VFStool se ..\\gamedata.vfs \"".. filename .. "\" \""
		.. PATH .. "\\" .. string.sub(filepath, 1, -2) .. "\"" 
		if bat ~= nil then
		        bat:write("@echo off \n")
			bat:write(exec, "\n")
			bat:flush()
			bat:close()
			os.execute(batname)
			os.remove(batname)
		end
	else
		f:close()
	end

	-- f should now exist in TDT
	f = io.open(abspath)
	if f == nil then
		io.write("ERROR: Could not extract " .. filename .. " from gamedata.vfs\n")
		error("needs() could not find file " .. filename)
	end
	f:close()
	return
end


-- Load "listof*.txt" from gamedata.vfs into table
-- filetype is a case sensitive text string, one of: AMMO, ARMOUR, MAGAZINE or WEAPON
-- Returns table
function loaditems (filetype, name)
	print("Parsing file from Gamedata.vfs")
	local infile = io.lines(PATH .. "\\" .. name)

	local list = {}		-- temp list of items
	for line in infile do
		if string.byte(line) ~= 59 then
		-- Start of Item
		if string.find(line, "%s*" .. filetype) and
		not string.find(line, "END_OF_") and
		not string.find(line, "LIST_OF_") then
			local item = parse_item(infile, filetype)
			table.insert(list, item)
		end
		end
	end
	return list
end


-- Save a list of items to cfg.vfs
-- filetype is a case sensitive text string, one of: AMMO, ARMOUR, MAGAZINE or WEAPON
function saveitems (filetype, name, list)
	if list == nil then
		error("No list of items given to saveitems function")
	end
	
	local outfile = io.open(PATH .. "\\" .. name, "w")

	outfile:write("LIST_OF_" .. filetype .. "\n")

	-- Write Item
	for item, stats in ipairs(list) do
		outfile:write("  " .. filetype .. "\n")

		-- Write stats for Item
		for i, stat in pairs(stats) do
			outfile:write("    " .. stat[KEY] .. " " .. stat[VALUE] .. "\n")
		end
		
		outfile:write("  END_OF_" .. filetype .. "\n")
	end

	outfile:write("END_OF_LIST_OF_" .. filetype .. "\n")
	outfile:close()
end


-- Check filename exists, if not extract it from localizationpack.vfs
-- if file does not exist and cannot be extracted, return false
-- filename	file to check, path is relative to root of VFS
function needsl (filename)
	-- convert to absolute path
	local abspath = LOCPATH .. "\\" .. filename
	LocFileAdded = true

	-- check if file exists in LTDT
	local f = io.open(abspath)
	if f ~= nil then
		f:close()
		return true
	end

	-- if not, extract from localizationpack.vfs
	local lastslash = 0
	for i = 1, string.len(filename) do
		if string.byte(filename, i) == 92 then lastslash = i end
	end
	local filepath = string.sub(filename, 1, lastslash)
	if string.len(filepath) > 0 then createdirectory(filepath, "l") end

	local params = "se \"" .. GAMEPATH .. "\\Localization\\LocalizationPack.vfs\" \""
	.. filename .. "\" \"" .. LOCPATH .. "\\" .. string.sub(filepath, 1, -2) .. "\""
	if string.len(filepath) == 0 then params = string.sub(params, 1, -2) end
	if vfs ~= nil then vfs(params) end

	-- Alchemist: Win98 console problem workaround
	-- Let's make command string shorter and produce
	-- external batch file which will do all the rest
	f = io.open(abspath)
	if f == nil then
		local batname = "extract.bat"
		local bat = io.open(batname, "w+")
		local exec = "VFStool se ..\\Localization\\LocalizationPack.vfs \"".. filename .. "\" \""
		.. LOCPATH .. "\\" .. string.sub(filepath, 1, -2) .. "\"" 
		if string.len(filepath) == 0 then exec = string.sub(exec, 1, -2) end
		if bat ~= nil then
		        bat:write("@echo off \n")
			bat:write(exec, "\n")
			bat:flush()
			bat:close()
			os.execute(batname)
			os.remove(batname)
		end
	end

	-- f should now exist in LTDT
	f = io.open(abspath)
	if f == nil then
		io.write("\n\nERROR: Could not extract " .. filename .. " from Localization\\LocalizationPack.vfs\n\n")
		return false
	end
	f:close()
	return true
end


-- Load text file localizationpack.vfs into table
-- filetype is a case sensitive text string, normally STR_RES
-- Returns table
function loaditemsl (filetype, name)
	print("Parsing file from LocalizationPack.vfs")
	local infile = io.open(LOCPATH .. "\\" .. name, "r")

	infile:read(2) -- eat the leading 0xFFFE
	local uni = infile:read("*a")
	infile:close()

	local ascii = toANSI(uni)

	-- ascii now contains ASCII version of file
	local list = {}		-- temp list of items
	local item = {}
	local state = "Out"	-- Is parser currently in a record or not

	for line in string.gfind(ascii, "[^\n]+") do
		if string.byte(line) ~= 59 then
		-- Start of Item
		if string.find(line, "%s*" .. filetype) and
		not string.find(line, "END_OF_") then
			state = "In"
		end
		-- End of Item
		if string.find(line, "END_OF_" .. filetype) then
			state = "Out"
			table.insert(list, item)	-- add item to list
			item = {}	-- create new table for next item
		end

		-- Add stats to Item list
		if state == "In" then
			for k, v in string.gfind(line, "%s+([%w%p%-]+)%s+(.+)") do
				pair = {k,v}
				table.insert(item, pair)
			end
		end
		end
	end
	return list
end


-- Save a list of items to localizationpack.vfs
-- filetype is a case sensitive text string, normally STR_RES
function saveitemsl (filetype, name, list)
	if list == nil then
		error("No list of items given to saveitemsl function")
	end

	-- create ascii string of file
	local ascii = "LIST_OF_" .. filetype .. "\r\n"
	for itemi, item in ipairs(list) do
		ascii = ascii .. "  " .. filetype .. "\r\n"
		for stati, stat in ipairs(item) do
			ascii = ascii .. "    " .. stat[1] .. " " .. stat[2] .. "\r\n"
		end
		ascii = ascii .. "  END_OF_" .. filetype .. "\r\n"
	end
	ascii = ascii .. "END_OF_LIST_OF_" .. filetype .. "\r\n"

	-- convert to unicode and save
	local outfile = io.open(LOCPATH .. "\\" .. name, "wb")
	outfile:write(string.char(255,254))
	outfile:write(toUnicode(ascii))
	outfile:close()
end


-- load list of item X where each item contains sub items SUB_X
function loadsub (filetype, name)
	print("Parsing file from Gamedata.vfs")
	local infile = io.lines(PATH .. "\\" .. name)

	local list = {}		-- temp list of items
	for line in infile do
		if string.byte(line) ~= 59 then
		-- Start of Item
		if string.find(line, filetype) ~= nil
		and string.find(line, "LIST_OF_") == nil
		and string.find(line, "END_OF_") == nil 
		and string.find(line, "SUB_") == nil
		and string.find(line, "AIDEFAULT") == nil then
			state = "In Item"
			local item = parse_item(infile, filetype)
			table.insert(list, item)	-- add item to list
		end
		end
	end
	return list
end


-- Save a list of items with sub items to go in cfg.vfs
-- filetype is a case sensitive text string, normally GLOSSARY
function savesub (filetype, name, list)
	local outfile = io.open(PATH .. "\\" .. name, "w")

	outfile:write("LIST_OF_" .. filetype .. "\n")

	-- Write Item
	for item, stats in ipairs(list) do
		outfile:write("  " .. filetype .. "\n")

		-- Write stats for Item
		for i, t in pairs(stats) do -- index, table
			-- if key, value pair then write
			if table.getn(t) == 2 then
				outfile:write("    " .. t[KEY] .. " " .. t[VALUE] .. "\n")
			else
			-- else write sub item
				outfile:write("    SUB_" .. filetype .. "\n") 
				for si, kv in pairs(t) do
					outfile:write("      " .. kv[KEY] .. " " .. kv[VALUE] .. "\n");
				end
				outfile:write("    END_OF_SUB_" .. filetype .. "\n")
			end
		end
		
		outfile:write("  END_OF_" .. filetype .. "\n")
	end

	outfile:write("END_OF_LIST_OF_" .. filetype .. "\n")
	outfile:close()
end


-- load list of item X where each item contains sub items SUB_X from an external file
function loadsube (filetype, name)
	print("Parsing file from plugin directory")
	local infile = io.lines("plugins\\" .. name)

	local list = {}		-- temp list of items
	for line in infile do
		if string.byte(line) ~= 59 then
		-- Start of Item
		if string.find(line, filetype) ~= nil
		and string.find(line, "LIST_OF_") == nil
		and string.find(line, "END_OF_") == nil 
		and string.find(line, "SUB_") == nil
		and string.find(line, "AIDEFAULT") == nil then
			local item = parse_item(infile, filetype)
			table.insert(list, item)	-- add item to list
		end
		end
	end
	return list
end


-- Load language index from localizationpack.vfs into table
-- Returns table
function loadlangindex ()
	needsl("languageindex.txt")
	local infile = io.lines(LOCPATH .. "\\languageindex.txt")

	local index = {}
	local state = "Out"	-- Is parser currently in a record or not
	for line in infile do
		if string.byte(line) ~= 59 then
		-- Start of index
		if string.find(line, "LOCALIZATION_PACK") == nil then
			state = "In"
		end

		-- End of index
		if string.find(line, "END_OF_LOCALIZATION_PACK") ~= nil
		then
			state = "Out"
		end

		-- Add properties to index
		if state == "In" then
			for k, v in string.gfind(line, "%s+([%w%p%-]+)%s+(.+)") do
				pair = {k,v}
				table.insert(index, pair)
			end
		end
		end
	end
	return index
end


-- Save language index to localizationpack.vfs
function savelangindex (index)
	local outfile = io.open(LOCPATH .. "\\languageindex.txt", "w")

	outfile:write("LOCALIZATION_PACK\n")

	-- Write index
	for item, stat in ipairs(index) do
		if stat[1] ~= "LANGUAGE" then outfile:write("  ") end
		outfile:write("  " .. stat[1] .. " " .. stat[2] .. "\n")
	end

	outfile:write("  END_OF_LANGUAGE\nEND_OF_LOCALIZATION_PACK")
	outfile:close()
end


-- Load "listof*.txt" format file into table.  Path is relative to plugins directory
-- filetype is a case sensitive text string, one of: AMMO, ARMOUR, MAGAZINE or WEAPON
-- Returns table
function loaditemse (filetype, name)
	print("Parsing items file from plugins directory")
	local infile = io.lines("plugins\\" .. name)

	local list = {}		-- temp list of items
	local item
	for line in infile do
		if string.byte(line) ~= 59 then
		-- Start of Item
		if string.find(line, "%s*" .. filetype) and
		not string.find(line, "END_OF_") and
		not string.find(line, "LIST_OF_") then
			item = parse_item(infile, filetype)
			table.insert(list, item)
		end
		end
	end
	return list
end


function loadtech (name)
	print("Parsing tech file from Gamedata.vfs")
	local infile = io.lines(PATH .. "\\" .. name)

	local list = {}		-- temp list of items
	local item
	for line in infile do
		if string.byte(line) ~= 59 then
		
		-- Start of Item
		if string.find(line, "%s*TECHNO") and
		not string.find(line, "_TECHNO") then
			item = parse_item(infile, "TECHNO")
			table.insert(list, {"TECHNO", item})	-- add item to list
		end

		if string.find(line, "%s*OBJECTS") and
		not string.find(line, "_OBJECTS") then
			item = parse_item(infile, "OBJECTS")
			table.insert(list, {"OBJECTS", item})
		end

		end
	end
	return list
end


function savetech (name, list)
	local outfile = io.open(PATH .. "\\" .. name, "w")

	outfile:write("LIST_OF_TECHNO\n")

	-- Write Item
	for i, item in ipairs(list) do
		if item[1] == "TECHNO" then
			outfile:write("  TECHNO\n")
		else
			outfile:write("  OBJECTS\n")
		end

		-- Write stats for Item
		for i, stat in pairs(item[2]) do
			if type(stat[KEY]) == "string" then
				outfile:write("    " .. stat[KEY] .. " " .. stat[VALUE] .. "\n")
			else
				-- write sub item
				outfile:write("    SUB_TECHNO\n")
				for j, substat in pairs(stat) do
					outfile:write("      " .. substat[KEY] .. " " .. substat[VALUE] .. "\n")
				end
				outfile:write("    END_OF_SUB_TECHNO\n")
			end
		end
		
		if item[1] == "TECHNO" then
			outfile:write("  END_OF_TECHNO\n")
		else
			outfile:write("  END_OF_OBJECTS\n")
		end
	end

	outfile:write("END_OF_LIST_OF_TECHNO\n")
	outfile:close()
end

-- as loadtech but with external file
function loadteche (name)
	print("Parsing tech file from plugins directory")
	local infile = io.lines("plugins\\" .. name)

	local list = {}		-- temp list of items
	local item
	for line in infile do
		if string.byte(line) ~= 59 then
		
		-- Start of Item
		if string.find(line, "%s*TECHNO") and
		not string.find(line, "_TECHNO") then
			item = parse_item(infile, "TECHNO")
			table.insert(list, {"TECHNO", item})	-- add item to list
		end

		if string.find(line, "%s*OBJECTS") and
		not string.find(line, "_OBJECTS") then
			item = parse_item(infile, "OBJECTS")
			table.insert(list, {"OBJECTS", item})
		end

		end
	end
	return list
end


-- load equipset file
-- returns list of inventory tables
-- inventory tables have subinventories "RIGHT_HAND", "LEFT_HAND", "BELT" and "BACKPACK"
-- subinventory is either Weapon or Mag
-- both subinventories have elements "POS_X", "POS_Y", "TEMPLATE"
-- Weapons have "MAG" and "HANDLING"
-- Mags have "CAPACITY"
-- EQUIP_SET
--   INVENTORY+
function loadequip (filename)
	print("Parsing inventory file from Gamedata.vfs")
	local infile = io.lines(PATH .. "\\" .. filename)

	local list = {}		-- temp list of items

	for line in infile do
		if string.byte(line) ~= 59 then
			local skip = false	-- set skip to true to skip to next line

			local t1, t2, t3 = line_to_args3(line)

			if t1 ~= nil then
				-- add name and inv count to returned list
				if skip == false
				and t1 == "EQUIP_SET" then
					list["NAME"] = t2
					list["COUNT"] = t3
					skip = true
				end

				-- Start of inventory
				if t1 == "INVENTORY"
				and skip == false then
					local inv = parse_inv(infile)
					table.insert(list, inv)	-- add inv to list
					skip = true
				end
			end
		end
	end
	return list
end


-- save equipset file
-- if a test parameter of 'true' is given, save path is absolute
-- otherwise, save path is relative to TDT root
function saveequip (filename, list, test)
	local outfile
	if test == nil then
		outfile = io.open(PATH .. "\\" .. filename, "w")
	else
		outfile = io.open(filename, "w")
	end

	outfile:write("EQUIP_SET " .. list.NAME .. " " .. table.getn(list) .. "\n")

	-- Write Item
	for i, inv in ipairs(list) do
		outfile:write("  INVENTORY\n")

		-- Write each subinventory for this inventory
		writesubinv(outfile, 2, "RIGHT_HAND", inv.RIGHT_HAND);
		writesubinv(outfile, 2, "LEFT_HAND", inv.LEFT_HAND);
		writesubinv(outfile, 2, "BELT", inv.BELT);
		writesubinv(outfile, 2, "BACKPACK", inv.BACKPACK);

		outfile:write("  END_OF_INVENTORY\n")
	end

	outfile:write("END_OF_EQUIP_SET\n")
	outfile:close()
end




-- load enemies*.txt file
-- NOT listofenemy.txt, use loadsub for that file
-- returns list of enemy tables
-- structure:
-- ENEMIES
--   SOLDIER+
--     UNID_ID #
--     NAME string
--     INVENTORY (table)
--     CHARACTER (table)
--     armour
function loadenemies (filename)
	print("Parsing enemies file from Gamedata.vfs")
	local infile = io.lines(PATH .. "\\" .. filename)

	local list = {}		-- temp list of items

	for line in infile do
		if string.byte(line) ~= 59 then
			local skip = false	-- set skip to true to skip to next line

			local t1, t2, t3 = line_to_args3(line)

			if t1 ~= nil then
				-- Start of soldier
				if skip == false
				and t1 == "SOLDIER" then
					local soldier = parse_soldier(infile)
					table.insert(list, soldier)	-- add soldier to list
					skip = true
				end
			end
		end
	end
	return list
end


-- save enemies*.txt file
-- NOT listofenemy.txt, use savesub for that file
-- if a test parameter of 'true' is given, save path is absolute
-- otherwise, save path is relative to TDT root
function saveenemies (filename, list, test)
	local outfile
	if test == nil then
		outfile = io.open(PATH .. "\\" .. filename, "w")
	else
		outfile = io.open(filename, "w")
	end

	outfile:write("ENEMIES\n")

	-- Write Item
	for sol_i, soldier in ipairs(list) do
		outfile:write("  SOLDIER\n")

		-- Write data for this soldier
		outfile:write("    UNIT_ID " .. soldier.UNIT_ID .. "\n")
		outfile:write("    NAME " .. soldier.NAME .. "\n")

		-- write inventory
		outfile:write("    INVENTORY\n")
		writesubinv(outfile, 4, "BACKPACK", soldier.INVENTORY.BACKPACK);
		writesubinv(outfile, 4, "BELT", soldier.INVENTORY.BELT);
		writesubinv(outfile, 4, "RIGHT_HAND", soldier.INVENTORY.RIGHT_HAND);
		writesubinv(outfile, 4, "LEFT_HAND", soldier.INVENTORY.LEFT_HAND);
		outfile:write("    END_OF_INVENTORY\n")

		-- write character
		outfile:write("    CHARACTER\n")
		for i, kv in ipairs(soldier.CHARACTER) do
			outfile:write("      " .. kv[KEY] .. " " .. kv[VALUE] .. "\n")
		end
		outfile:write("    END_OF_CHARACTER\n")
		
		-- write armour
		outfile:write("    ")
		for i, kv in ipairs(soldier.ARMOUR) do
			outfile:write(kv[KEY] .. " " .. kv[VALUE] .. " ")
		end
		outfile:write("\n")

		outfile:write("  END_OF_SOLDIER\n")
	end

	outfile:write("END_OF_ENEMIES\n")
	outfile:close()
end

-- write subinventory record to file
-- outfile is handle to output file
-- indent is number of spaces put before INVENTORY string
-- name is the name of the subinv
-- subinv is subinventory to write
function writesubinv(outfile, indent, name, subinv)
	local i = string.rep(" ", indent)

	-- Write subinventory
	outfile:write(i .. "  " .. name .. "\n")
		
	-- write each item in subinventory
	for j, item in ipairs(subinv) do
		outfile:write(i .. "    " .. item[1] .. "\n")

		local fields = item[2]
		outfile:write(i .. "      POS_X " .. fields.POS_X .. "\n")
		outfile:write(i .. "      POS_Y " .. fields.POS_Y .. "\n")
		outfile:write(i .. "      TEMPLATE " .. fields.TEMPLATE .. "\n")

		-- write weapon specific fields
		if item[1] == "WEAPON" then
			-- write magazine if present
			if fields.MAGAZINE ~= nil then
			outfile:write(i .. "      MAGAZINE\n")
			outfile:write(i .. "        POS_X " .. fields.MAGAZINE.POS_X .. "\n");
			outfile:write(i .. "        POS_Y " .. fields.MAGAZINE.POS_Y .. "\n");
			outfile:write(i .. "        TEMPLATE " .. fields.MAGAZINE.TEMPLATE .. "\n");
			outfile:write(i .. "        CAPACITY " .. fields.MAGAZINE.CAPACITY .. "\n");
			outfile:write(i .. "      END_OF_ITEM\n")
			end
			if fields.HANDLING_AVAILABLE ~= nil then
				outfile:write("        HANDLING_AVAILABLE " .. fields.HANDLING_AVAILABLE .. "\n")
			end
		else
		-- write magazine specific fields
			outfile:write(i .. "      CAPACITY " .. fields.CAPACITY .. "\n")
		end
		outfile:write(i .. "    END_OF_ITEM\n")
	end
	outfile:write(i .. "  END_OF_SUBINVENTORY\n")
end


-- change a single line in a file
-- search	Lua pattern to find in line
-- replace	Lua pattern which replaces the search string in the line
-- returns number of matches made
function changeline(file, search, replace)
	local infile = io.lines(PATH .. "\\" .. file)

	if infile == nil then
		error("changeline: could not open file " .. file)
	end

	local f = {} -- table of lines in file
	for line in infile do
		table.insert(f, line)
	end

	local found = 0
	for i, line in pairs(f) do
		f[i], result = string.gsub(line, search, replace)
		if result > 0 then
			print("changeline: Replacement line", f[i])
			found = found + 1
			end
	end

	local outfile = io.open(PATH .. "\\" .. file, "wb")
	for i, l in pairs(f) do
		outfile:write(l)
		outfile:write("\r\n")
	end

	outfile:close()
	return found
end


-- copy a file to a destination file or directory within the TDT or LTDT
-- uses operating systems' "copy" command so works with wildcards etc.
-- source	file to copy
-- dest		directory or file to copy into
-- vfs		copy into gamedata.vfs or localization.vfs
-- vfs is "l" for localizationpack.vfs and "g" or blank for gamedata.vfs
function copyfile (source, dest, vfs)
	print("Copying " .. source .. " to " .. dest)
	createdirectory(dest, vfs)

	local result
	if vfs == "l" then
		result = copy("plugins\\" .. source, LOCPATH .. "\\" .. dest)
		LocFileAdded = true
	else
		result = copy("plugins\\" .. source, PATH .. "\\" .. dest)
		GDFileAdded = true
	end
	assert(result == 0, "Copyfile: Could not copy " .. source)
end


-- creates directories in the TDT or LTDT
function createdirectory(dest, vfs)
	local pos = string.find(dest, "\\")
	if pos ~= nil then
		local dir = string.sub(dest, 1, pos-1)
		local oldpos
		while pos ~= nil do
			if vfs == "l" then
				mkdir(LOCPATH .. "\\" .. dir)
			else
				mkdir(PATH .. "\\" .. dir)
			end

			oldpos = pos+1
			pos = string.find(dest, "\\", oldpos)
			if pos ~= nil then dir = string.sub(dest, 1, pos-1) end
		end
	end
end

