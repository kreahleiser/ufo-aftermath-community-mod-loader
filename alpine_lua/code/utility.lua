-- Aftermath Lua Plugin Engine ("ALPine")
-- Utility function library
-- Written by Andrew 'Fulby' Campbell
-- Release 2.8
-- 17th December 2003

-- Functions in this library:
-- string	toANSI		(string)
-- string	toUnicode	(string)
--			mkdir		(dir)
--			copy		(source, dest)


-- The following functions are replaced by the ALPine GUI with C versions,
-- they are provided here for compatibility with the apply command line program


-- toANSI, convert Unicode 16 bit to ANSI/ASCII 8 bit
-- function is replaced with faster C function when using ALPine GUI
if toANSI == nil then
	function toANSI (s)
		local retval = ""
		local len = string.len(s)
		for i = 1, len, 2 do
			retval = retval .. string.sub(s, i, i)
		end
		return retval
	end
end


-- toUnicode
if toUnicode == nil then
	function toUnicode (s)
		local retval = ""
		local len = string.len(s)
		for i = 1, len do
			retval = retval .. string.sub(s, i, i) .. "\0"
		end
		return retval
	end
end


-- make one directory
if mkdir == nil then
	function mkdir(dir)
	        if string.sub(dir, -1) == "\\" then
			dir = string.sub(dir, 1, string.len(dir)-1)
		end
		os.execute("mkdir \"" .. dir .. "\"");
	end
end


-- copy files, dest is an absolute path
if copy == nil then
	function copy(source, dest)
	        if string.sub(dest, -1) == "\\" then
			dest = string.sub(dest, 1, string.len(dest)-1)
		end
		return os.execute("copy \"" .. source .. "\" \"" .. dest .. "\"")
	end
end

