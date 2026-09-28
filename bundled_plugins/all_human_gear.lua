-- All Human Gear
-- UFO Aftermath Community Mod Loader
--
-- Makes every human weapon, magazine, and suit of armour available from
-- the start of the game, in unlimited supply.
--
-- Inspired by the ALPine plugin "All Human Weapons" (Andrew 'Fulby'
-- Campbell, 2003). Rewritten for this loader; not a copy of that script.

print("All Human Gear")

needs("tactical\\configs\\game\\listofweapon.txt")
needs("tactical\\configs\\game\\listofmagazine.txt")
needs("tactical\\configs\\game\\listofarmor.txt")

local weapons = loaditems("WEAPON", "tactical\\configs\\game\\listofweapon.txt")
local mags    = loaditems("MAGAZINE", "tactical\\configs\\game\\listofmagazine.txt")
local armor   = loaditems("ARMOR", "tactical\\configs\\game\\listofarmor.txt")

local human = {"ORIGIN", "HUMAN"}
moditems(getitems1(weapons, human), {{"TECH_LEVEL", 1}})
moditems(getitems1(mags, human), {{"TECH_LEVEL", 1}})

local function human_armor_id(key, value)
	if key ~= "ID" or type(value) ~= "string" then
		return false
	end
	return string.find(value, "H..0") ~= nil
end

moditems(getitemsf(armor, human_armor_id), {{"LEVEL", 1}})

saveitems("WEAPON", "tactical\\configs\\game\\listofweapon.txt", weapons)
saveitems("MAGAZINE", "tactical\\configs\\game\\listofmagazine.txt", mags)
saveitems("ARMOR", "tactical\\configs\\game\\listofarmor.txt", armor)

print("All Human Gear processed")
