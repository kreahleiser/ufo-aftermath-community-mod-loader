-- Aftermath Lua Plugin Engine ("ALPine")
-- Core library
-- Written by Andrew 'Fulby' Campbell
-- Release 2.7
-- 13th December 2003

-- Functions in this library:
-- item		getitem		(items, ml)	
-- items	getitems	(items, ml)
-- items	getitemsf	(items, f)
-- 			show		(item)
-- 			setval		(item, key, value)
-- value	getval		(item, key)
-- 			moditem		(item, ml)
-- 			moditems	(items, ml)
-- item		createcopy	(item)
--			additem		(items, item)
--			additems	(list, items, start, end)
-- 			deleteitems	(items, ml)
-- string	getlanguage	()
-- 			traverse	(table)

-- Datatypes:
-- items	{item, item, item, ... }	list of items
-- item		{kv, kv, kv, ... }		single item
-- kv		{key, value}			key, value pair
-- key		string				name of the field/key
-- value	string or number		value of the field
-- ml		{kv, kv, kv, ... }		list of key, value pairs to match against items
-- langindex	{kv, kv, kv, ... }		table of key, value pairs from languageindex.txt


-- Possible bugs:
-- If a number is an integer, it is saved without a decimal point.  This does not seem to affect the game.
-- The code dealing with the TDT and various directory paths could probably use some work to make it more secure and robust


-- Constants:
KEY = 1
VALUE = 2



-- Print properties of an item
-- If item is in a list, use show(list[index])
function show(item)
	for i, v in pairs(item) do
		print(v[KEY], v[VALUE])
	end
end


-- Get first item which matches from a table of items
-- t		table storing list of items
-- m		key, value pair (in a table) which item will be matched against
-- Returns item or nil if no match
function getitem1 (t, m)
	local key = m[KEY]
	local value = m[VALUE]
	
	-- Loop through items
	for index, item in ipairs(t) do
		-- Loop through stats
		for si, s in pairs(item) do
			-- Compare stats
			if s[KEY] == key then
				if type(value) == "string" then
					if s[VALUE] == value then	
						print("Found matching item")
						return item
					else
						break
					end
				else		-- value is a number
					local sv = tonumber(s[VALUE])
					local v = tonumber(value)
					if sv == v then
						print("Found matching item")
						return item
					else
						break
					end
				end
			end
		end
	end
	print("No match found")
	return nil
end


-- Get first item which matches multiple statistics, returns nil if no match found
-- t		list of items
-- ml		list of key,value pairs
function getitem(t, ml)
	local subset = t
	for i, kv in pairs(ml) do
		subset = getitems1(subset, kv)
	end
	return subset[1]
end


-- Get a subset of items from a table of items using 1 key, value pair
-- t		table storing list of items
-- m		key, value pair (in a table) which item will be matched against
-- Returns list of matching items
function getitems1 (t, m)
	local subset = {}
	local key = m[KEY]
	local value = m[VALUE]
	
	-- Loop through items
	for index, item in ipairs(t) do
		-- Loop through stats
		for si, s in pairs(item) do
			-- Compare stats
			if s[KEY] == key then
				if type(value) == "string" then
					if s[VALUE] == value then	
						table.insert(subset, item)
						break
					end
				else		-- value is a number
					local sv = tonumber(s[VALUE])
					local v = tonumber(value)
					if sv == v then
						table.insert(subset, item)
						break
					end
				end
			end
		end
	end
	return subset
end


-- Get subset of list which matches multiple statistics
-- t		list of items
-- ml		list of key,value pairs
function getitems(t, ml)
	local subset = t
	for i, kv in pairs(ml) do
		subset = getitems1(subset, kv)
	end
	return subset
end


-- runs function f on each of item's properties, returns number of successful calls
-- item		item to be checked
-- f		function which takes key and value as parameters, returns true or false
function itemf (item, f)
	local count = 0
	for i, kv in ipairs(item) do
		local key = kv[KEY]
		local value = kv[VALUE]
		if f(key, value) == true then count = count + 1
		end
	end
	return count	
end


-- calls itemf over a list of items, returns list of items where f returned true at least once
function getitemsf (items, f)
	local temp = {}
	for i, item in pairs(items) do
		local count = itemf(item, f)
		if count > 0 then table.insert(temp, item) end
	end
	return temp
end


-- set value in an item
function setval(item, key, value)
	local i = getindex(item, key)
	if i == nil then
		error("setval key " .. key .. " could not be found")
		os.exit(1)
	end
	item[i][VALUE] = value
end


-- get value from an item
function getval(item, key)
	i = getindex(item, key)
	if i == nil then
		error("getval key " .. key .. " could not be found")
		os.exit(1)
	end
	return item[i][VALUE]
end


-- Modify multiple stats in a single item
-- ml		list of key, value pairs
function moditem (item, ml)
	for i, kv in pairs(ml) do
		setval(item, kv[KEY], kv[VALUE])
	end
end


-- Modify multiple stats in every item in a list
function moditems (list, ml)
	for i, item in pairs(list) do
		moditem(item, ml)
	end
end


-- getindex, returns the index for a key in a given item, or nil it not found
function getindex(item, key)
	for i, kv in pairs(item) do
		if kv[KEY] == key then return i end
	end
	return nil
end


-- create a copy of an item
function createcopy(item)
	local temp = {}
	for i, kv in pairs(item) do
		if table.getn(kv) == 2 then
			table.insert(temp, {kv[KEY], kv[VALUE]})
		else
			table.insert(temp, createcopy(kv))
		end
	end
	return temp
end


-- add item to a list of items
function additem(list, item)
	table.insert(list, item)
end


-- add items in a list to another list
-- list		list of items to add to
-- items	list of items containing items to add
-- index1	index (1-based) of first item to add, defaults to 1-
-- index2	index of last item to add, dafaults to number of items in 'items'
function additems(list, items, index1, index2)
	if index1 == nil then index1 = 1 end
	if index2 == nil then index2 = table.getn(items) end
	for i = index1, index2 do
		additem(list, items[i])
	end
end

-- delete items from a list which match constraints
-- ml	list of key, value pairs to compare against items
function deleteitems(list, ml)
	local dellist = {}
	dellist = getitems(list, ml)
	for i, delitem in pairs(dellist) do
		for index, item in pairs(list) do
			if delitem == item then
				table.remove(list, index)
			end
		end
	end
end

-- Returns current language as a string
function getlanguage()
	local t = loadlangindex()
	local lang = getval(t, "LANGUAGE")
	lang = string.sub(lang, 2, -2) -- remove leading and trailing quotes
	return lang
end


-- Lists all the elements in a table, useful utility function
function traverse (t)
	for i, v in pairs(t) do
		print(i, v)
	end
end


-- error handler for GUI
function alpine_error_handler (message)
	local emessage = debug.traceback(message)
	emessage = string.gsub(emessage, "([^\r])\n", "%1\r\n")
	return emessage
end


