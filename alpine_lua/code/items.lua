-- Aftermath Lua Plugin Engine ("ALPine")
-- Advanced item handling library
-- Written by Andrew 'Fulby' Campbell
-- Release 2.8
-- 17th December 2003

-- The functions in this file load an item from a plugin file and add them to the game.
-- The plugin files should be in the same format as the equivalent UFO data file.

-- Functions in this library:
--		addweapon	(filename, index)
--		addmagazine	(filename, index)
--		addglossary	(type, filename, index)
--		addtext		(language, locfile, filename, index)
--		addtech		(filename, index)
--		addarmor	(filename, index)




-- add the first weapon in file filename to the game
-- filename 	path is relative to plugins directory
-- index	index of item (1-based), defaults to 1
function addweapon(filename, index)
	print("addweapon: Adding " .. filename)
	needs("tactical\\configs\\game\\listofweapon.txt")
	local weaplist = loaditems("WEAPON", "tactical\\configs\\game\\listofweapon.txt")

	local myweaplist = loaditemse("WEAPON", filename)
	if table.getn(myweaplist) < 1 then
		print("addweapon: Could not load plugin's weapon")
		error("addweapon: Could not load plugin's weapon")
	end
	
	if index == nil then index = 1 end
	local myweap = myweaplist[index]
	if myweap == nil then
		print("addweapon: No weapon at index " .. index)
		error("addweapon: No weapon at index " .. index)
	end

	additem(weaplist, myweap)
	saveitems("WEAPON", "tactical\\configs\\game\\listofweapon.txt", weaplist)

	print("addweapon: Successful")
end


-- add the first magazine in file filename to the game
-- filename 	path is relative to plugins directory
-- index	index of item (1-based), defaults to 1
function addmagazine(filename, index)
	print("addmagazine: Adding " .. filename)
	needs("tactical\\configs\\game\\listofmagazine.txt")
	local maglist = loaditems("MAGAZINE", "tactical\\configs\\game\\listofmagazine.txt")

	local mymaglist = loaditemse("MAGAZINE", filename)
	if table.getn(mymaglist) < 1 then
		print("addmagazine: Could not load plugin's magazine")
		error("addmagazine: Could not load plugin's magazine")
	end

	if index == nil then index = 1 end
	local mymag = mymaglist[index]
	if mymag == nil then
		print("addmagazine: No magazine at index " .. index)
		error("addmagazine: No magazine at index " .. index)
	end

	additem(maglist, mymag)
	saveitems("MAGAZINE", "tactical\\configs\\game\\listofmagazine.txt", maglist)

	print("addmagazine: Successful")
end


-- add a glossary entry
-- type		one of "WEAPON", "ARMOR", "TECH", "MUTANT", "UFO", "BASES" or "TUTORIAL"
-- filename 	path is relative to plugins directory
-- index	index of item (1-based), defaults to 1
function addglossary(type, filename, index)
	print("addglossary: Adding " .. filename)
	local glosindex = 0
	if type == "WEAPON" then glosindex = 1 end
	if type == "ARMOR" then glosindex = 2 end
	if type == "TECH" then glosindex = 3 end
	if type == "MUTANT" then glosindex = 4 end
	if type == "UFO" then glosindex = 5 end
	if type == "BASES" then glosindex = 6 end
	if type == "TUTORIAL" then glosindex = 7 end
	if glosindex == 0 then
		print("addglossary: Unrecognised type in parameter 1")
		error("addglossary: Unrecognised type in parameter 1")
	end

	needs("strategic\\configs\\ufopedia\\listofglossary.txt")
	local metagloslist = loadsub("GLOSSARY", "strategic\\configs\\ufopedia\\listofglossary.txt")
	local gloslist = metagloslist[glosindex]

	local mygloslist = loaditemse("SUB_GLOSSARY", filename)
	if table.getn(mygloslist) < 1 then
		print("addglossary: Could not load plugin's glossary")
		error("addglossary: Could not load plugin's glossary")
	end

	if index == nil then index = 1 end
	local myglos = mygloslist[index]
	if myglos == nil then
		print("addglossary: No glossary entry at index " .. index)
		error("addglossary: No glossary entry at index " .. index)
	end
	
	additem(gloslist, myglos)
	savesub("GLOSSARY", "strategic\\configs\\ufopedia\\listofglossary.txt", metagloslist)

	print("addglossary: Successful")
end


-- add localized text
-- language	language, must equal folder name in localizationpack.vfs
-- locfile	file name of one of the files in the localization 'language' directory 
-- filename 	path is relative to plugins directory
-- index	index of item (1-based), defaults to 1
function addtext(language, locfile, filename, index)
	print("addtext: Adding " .. filename)
	if needsl(language .. "\\" .. locfile) == false then
		print("addtext: Could not open localization file")
		error("addtext: Could not open localization file")
	end
	local textlist = loaditemsl("STR_RES", language .. "\\" .. locfile)

	local mytextlist = loaditemse("STR_RES", filename)
	if table.getn(mytextlist) < 1 then
		print("addtext: Could not load plugin's text file")
		error("addtext: Could not load plugin's text file")
	end

	if index == nil then index = 1 end
	local mytext = mytextlist[index]
	if mytext == nil then
		print("addtext: No text entry at index " .. index)
		error("addtext: No text entry at index " .. index)
	end

	additem(textlist, mytext)
	saveitemsl("STR_RES", language .. "\\" .. locfile, textlist)

	print("addtext: Successful")
end


-- adds a tech entry
-- filename 	path is relative to plugins directory
-- index	index of item (1-based), defaults to 1
function addtech(filename, index)
	print("addtech: Adding " .. filename)
	needs("strategic\\configs\\r&d\\listoftech.txt")
	local techlist = loadtech("strategic\\configs\\r&d\\listoftech.txt")

	-- load plugin's tech list
	local infile = io.lines("plugins\\" .. filename)
	if infile == nil then
		print("addtech: Could not load plugin's tech file")
		error("addtech: Could not load plugin's tech file")
	end

	local mytechlist	-- list of plugin's techs
	mytechlist = loadteche(filename)

	if table.getn(mytechlist) < 1 then
		print("addtech: No techs in plugin's tech file")
		error("addtech: No techs in plugin's tech file")
	end
	
	if index == nil then index = 1 end
	local mytech = mytechlist[index]
	if mytech == nil then
		print("addtech: No tech at index " .. index)
		error("addtech: No tech at index " .. index)
	end

	additem(techlist, mytech)
	savetech("strategic\\configs\\r&d\\listoftech.txt", techlist)

	print("addtech: Successful")
end


-- add the first armor in file filename to the game
-- filename 	path is relative to plugins directory
-- index	index of item (1-based), defaults to 1
function addarmor(filename, index)
	print("addarmor: Adding " .. filename)
	needs("tactical\\configs\\game\\listofarmor.txt")
	local armorlist = loaditems("ARMOR", "tactical\\configs\\game\\listofarmor.txt")

	local myarmorlist = loaditemse("ARMOR", filename)
	if table.getn(myarmorlist) < 1 then
		print("addarmor: Could not load plugin's armor")
		error("addarmor: Could not load plugin's armor")
	end

	if index == nil then index = 1 end
	local myarmor = myarmorlist[index]
	if myarmor == nil then
		print("addarmor: No armor at index " .. index)
		error("addarmor: No armor at index " .. index)
	end

	additem(armorlist, myarmor)
	saveitems("ARMOR", "tactical\\configs\\game\\listofarmor.txt", armorlist)

	print("addarmor: Successful")
end


