-- Crisp Fonts
-- UFO Aftermath Community Mod Loader
--
-- Dilates each glyph in the stock bitmap atlases by 1 pixel into empty
-- space so bilinear filtering (and 16:9 stretch) does not pick up the
-- black cell next door. Helps HUD text and in-mission dialogue.
--
-- Does not replace Crisp UI; that still nearest-filters other HUD art.

print("Crisp Fonts")

local fonts = {
	"default",
	"nakamura_10",
	"nakamura_8",
	"verdana_10_bo",
	"courier_new_12",
}

for i = 1, #fonts do
	local name = fonts[i]
	local tga = "share\\textures\\fonts\\" .. name .. ".tga"
	local cfg = "share\\configs\\fonts\\" .. name .. ".txt"
	needs(tga)
	needs(cfg)
	local tga_path = PATH .. "\\" .. tga
	local cfg_path = PATH .. "\\" .. cfg
	if pad_font(tga_path, cfg_path) then
		print("padded " .. name)
	else
		print("failed " .. name)
	end
end

print("Crisp Fonts processed")
