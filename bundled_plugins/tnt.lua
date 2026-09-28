-- TNT
-- UFO Aftermath Community Mod Loader
--
-- Adds a throwable satchel charge. Models and icons come from the original
-- ALPine TNT pack (Andrew 'Fulby' Campbell, 2003). This script is a new
-- implementation of that idea, not a copy of the ALPine lua.
--
-- English ufopedia strings are not written yet (localization pack support
-- is still pending). The item still shows as "TNT" in the inventory.

print("TNT")

addweapon("tnt\\listofweapon.txt")
addmagazine("tnt\\listofmagazine.txt")
addglossary("WEAPON", "tnt\\listofglossary.txt")

copyfile("tnt\\h_eq-tnt*.txt", "tactical\\models\\interface\\items\\weapons\\")
copyfile("tnt\\h_eq-tnt.tga", "tactical\\textures\\interface\\items\\weapons\\")
copyfile("tnt\\h_eq-tnt_video.txt", "strategic\\models\\interface\\equipment\\weapons\\human guns\\")
copyfile("tnt\\h_eq-tnt_3d.txt", "tactical\\models\\weapons\\")
copyfile("tnt\\tnt.tga", "tactical\\textures\\weapons\\")
copyfile("tnt\\tnt_beep.pcfg", "tactical\\particles\\gun groups\\human\\")

print("TNT processed")
