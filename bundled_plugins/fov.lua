-- Field of View
-- UFO Aftermath Community Mod Loader
--
-- Sets tactical camera FOV. PARAM is percent of stock (100 = 0.7 radians).
-- Blank or 0 leaves the original value.
-- PARAM_DEFAULT 100
--
-- Inspired by the ALPine FoV plugin (Andrew 'Fulby' Campbell, 2003).
-- Rewritten for this loader; not a copy of that script.

print("Field of View")

local fov = 0.7
if PARAM ~= nil and PARAM > 0 then
	fov = 0.7 * PARAM / 100
end

needs("tactical\\configs\\game\\_camera_fin2.txt")
changeline("tactical\\configs\\game\\_camera_fin2.txt", "FOV    [%.%d]+", "FOV    " .. fov)

print("Field of View processed")
