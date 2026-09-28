-- Aftermath Lua Plugin Engine ("ALPine")
-- File parsing library
-- Written by Andrew 'Fulby' Campbell
-- Release 2.8
-- 17th December 2003

-- Functions in this library:
-- # # #	line_to_args3		(line)
-- # #		line_to_args2		(line)
-- #		line_to_args1		(line)
-- table	parse_inv			(io.lines)
-- table	parse_subinv		(io.lines)
-- table	parse_inv_weapon	(io.lines)
-- table	parse_inv_mag		(io.lines)
-- table	parse_soldier		(io.lines)
-- table	parse_character		(io.ines)
-- table	parse_item			(io.lines, itemtype)
-- table	parse_subitem		(io.lines, itemtype)


function line_to_args3 (line)
	local i1, i2, t1, t2, t3
	-- match 3 args
	i1, i2, t1, t2, t3 = string.find(line, "%s*([%w_]+)%s+([%w%p]+)%s+(.+)")
	-- match 2 args
	i1, i2, t1, t2 = string.find(line, "%s*([%w_]+)%s+([%w%p]+)")
	-- match 1 arg
	i1, i2, t1 = string.find(line, "%s*([%w_]+)")
	return t1, t2, t3
end


function line_to_args2 (line)
	local i1, i2, t1, t2
	-- match 2 args
	i1, i2, t1, t2 = string.find(line, "%s*([%w_]+)%s+(.+)")
	-- match 1 arg
	i1, i2, t1 = string.find(line, "%s*([%w_]+)")
	return t1, t2
end


function line_to_args1 (line)
	local i1, i2, t1
	-- match 1 arg
	i1, i2, t1 = string.find(line, "%s*(.+)")
	return t1, t2
end


function parse_inv (infile)
	local inv = {}

	for line in infile do
		local finished = false
		local skip = false	-- set skip to true to skip to next line

		if string.byte(line) ~= 59 then

			local t1, t2 = line_to_args2(line)

			if t1 ~= nil then
				-- End of sub inventory
				if t1 == "END_OF_INVENTORY" then
					finished = true
					skip = true
				end
			end
				
			if skip == false
			and (t1 == "RIGHT_HAND"
			or t1 == "LEFT_HAND"
			or t1 == "BELT"
			or t1 == "BACKPACK") then
				local subinv = parse_subinv(infile)
				inv[t1] = subinv
				skip = true
			end
		end
		if finished then break end
	end
	return inv
end


function parse_subinv (infile)
	local subinv = {}

	for line in infile do
		local finished = false
		local skip = false	-- set skip to true to skip to next line

		if string.byte(line) ~= 59 then

			local t1, t2 = line_to_args2(line)

			if t1 ~= nil then
				-- End of sub inventory
				if t1 == "END_OF_SUBINVENTORY" then
					finished = true
					skip = true
				end
				
				if skip == false
				and t1 == "MAGAZINE" then
					local mag = parse_inv_mag(infile)
					table.insert(subinv, {"MAGAZINE", mag})
					skip = true
				end
				
				if skip == false
				and t1 == "WEAPON" then
					local weapon = parse_inv_weapon(infile)
					table.insert(subinv, {"WEAPON", weapon})
					skip = true
				end
			end
		end
		if finished then break end
	end
	return subinv
end


function parse_inv_mag (infile)
	local mag = {}

	for line in infile do
		local finished = false
		if string.byte(line) ~= 59 then

			local t1, t2 = line_to_args2(line)

			if t1 ~= nil then
				-- End of magazine
				if t1 == "END_OF_ITEM" then
					finished = true
				else
					-- add stats to mag
					mag[t1] = t2
				end
			end
		end
		if finished then break end
	end
	return mag
end


function parse_inv_weapon (infile)
	local weapon = {}

	for line in infile do
		local finished = false
		local skip = false	-- set skip to true to skip to next line

		if string.byte(line) ~= 59 then

			local t1, t2 = line_to_args2(line)

			if t1 ~= nil then
				-- End of weapon
				if t1 == "END_OF_ITEM" then
					finished = true
					skip = true
				end
				
				if skip == false
				and t1 == "MAGAZINE" then
					weapon["MAGAZINE"] = parse_inv_mag(infile)
					skip = true
				end
				
				-- add stats to item in subinventory
				if skip == false then
					weapon[t1] = t2
					skip = true
				end
			end
		end
		if finished then break end
	end
	return weapon
end


function parse_soldier (infile)
	local soldier = {}

	for line in infile do
		local finished = false
		local skip = false	-- set skip to true to skip to next line

		if string.byte(line) ~= 59 then

			local t1, t2 = line_to_args2(line)

			if t1 ~= nil then
				-- End of soldier
				if t1 == "END_OF_SOLDIER" then
					finished = true
					skip = true
				end
				
				-- inventory
				if skip == false
				and t1 == "INVENTORY" then
					soldier["INVENTORY"] = parse_inv(infile)
					skip = true
				end
				
				-- character
				if skip == false
				and t1 == "CHARACTER" then
					soldier["CHARACTER"] = parse_character(infile)
					skip = true
				end
				
				-- body armour, 8 values on one line
				if skip == false
				and t1 == "BODY_ARMOUR" then
					local i1, i2, k1, v1, k2, v2, k3, v3, k4, v4
					i1, i2, k1, v1, k2, v2, k3, v3, k4, v4 = string.find(line,
					 "%s*([%w_]+)%s+([%w%p]+)%s+([%w_]+)%s+([%w%p]+)%s+([%w_]+)%s+([%d]+)%s+([%w_]+)%s+([%d]+)")
					local armour = {{k1,v1}, {k2,v2}, {k3,v3}, {k4,v4}}
					soldier["ARMOUR"] = armour
					skip = true
				end
								
				-- add UNIT_ID and NAME to soldier
				if skip == false then
					soldier[t1] = t2
					skip = true
				end
			end
		end
		if finished then break end
	end
	return soldier
end


function parse_character (infile)
	local char = {}

	for line in infile do
		local finished = false
		if string.byte(line) ~= 59 then

			local t1, t2 = line_to_args2(line)

			if t1 ~= nil then
				-- End of character
				if t1 == "END_OF_CHARACTER" then
					finished = true
				else
					-- add stats to character
					table.insert(char, {t1, t2})
				end
			end
		end
		if finished then break end
	end
	return char
end


function parse_item (infile, itemtype)
	local item = {}

	for line in infile do
		local finished = false
		local skip = false	-- set skip to true to skip to next line

		if string.byte(line) ~= 59 then

			local t1, t2 = line_to_args2(line)

			if t1 ~= nil then
				-- End of item
				if t1 == "END_OF_" .. itemtype then
					finished = true
					skip = true
				end
				
				-- add subitem
				if skip == false
				and t1 == "SUB_" .. itemtype then
					local subitem = parse_subitem(infile, itemtype)
					table.insert(item, subitem)
					skip = true
				end
				
				-- add stats to item
				if skip == false then
					table.insert(item, {t1, t2})
					skip = true
				end
			end
		end
		if finished then break end
	end
	return item
end


function parse_subitem (infile, itemtype)
	local subitem = {}

	for line in infile do
		local finished = false
		if string.byte(line) ~= 59 then

			local t1, t2 = line_to_args2(line)

			if t1 ~= nil then
				-- End of subitem
				if t1 == "END_OF_SUB_" .. itemtype then
					finished = true
				else
					-- add stats to subitem
					if t2 ~= nil then
						table.insert(subitem, {t1, t2})
					else
						table.insert(subitem, {"", t1})
					end
				end
			end
		end
		if finished then break end
	end
	return subitem
end


