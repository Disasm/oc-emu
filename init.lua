intcomponent.lfs = lfs

function time()
    return mseconds() / 1000.0
end

unicode = {}
for k,v in pairs(string) do
    unicode[k] = v
end
unicode.wlen = function(value)
    return string.len(value)
end

unicode2koi8r = {
    [0x2500] = 0x80,
    [0x2502] = 0x81,
    [0x250c] = 0x82,
    [0x2510] = 0x83,
    [0x2514] = 0x84,
    [0x2518] = 0x85,
    [0x251c] = 0x86,
    [0x2524] = 0x87,
    [0x252c] = 0x88,
    [0x2534] = 0x89,
    [0x253c] = 0x8a,
    [0x2580] = 0x8b,
    [0x2584] = 0x8c,
    [0x2588] = 0x8d,
    [0x258c] = 0x8e,
    [0x2590] = 0x8f,
    [0x2591] = 0x90,
    [0x2592] = 0x91,
    [0x2593] = 0x92,
    [0x2320] = 0x93,
    [0x25a0] = 0x94,
    [0x2219] = 0x95,
    [0x221a] = 0x96,
    [0x2248] = 0x97,
    [0x2264] = 0x98,
    [0x2265] = 0x99,
    [0x00a0] = 0x9a,
    [0x2321] = 0x9b,
    [0x00b0] = 0x9c,
    [0x00b2] = 0x9d,
    [0x00b7] = 0x9e,
    [0x00f7] = 0x9f,
    [0x2550] = 0xa0,
    [0x2551] = 0xa1,
    [0x2552] = 0xa2,
    [0x0451] = 0xa3,
    [0x2553] = 0xa4,
    [0x2554] = 0xa5,
    [0x2555] = 0xa6,
    [0x2556] = 0xa7,
    [0x2557] = 0xa8,
    [0x2558] = 0xa9,
    [0x2559] = 0xaa,
    [0x255a] = 0xab,
    [0x255b] = 0xac,
    [0x255c] = 0xad,
    [0x255d] = 0xae,
    [0x255e] = 0xaf,
    [0x255f] = 0xb0,
    [0x2560] = 0xb1,
    [0x2561] = 0xb2,
    [0x0401] = 0xb3,
    [0x2562] = 0xb4,
    [0x2563] = 0xb5,
    [0x2564] = 0xb6,
    [0x2565] = 0xb7,
    [0x2566] = 0xb8,
    [0x2567] = 0xb9,
    [0x2568] = 0xba,
    [0x2569] = 0xbb,
    [0x256a] = 0xbc,
    [0x256b] = 0xbd,
    [0x256c] = 0xbe,
    [0x00a9] = 0xbf,
    [0x044e] = 0xc0,
    [0x0430] = 0xc1,
    [0x0431] = 0xc2,
    [0x0446] = 0xc3,
    [0x0434] = 0xc4,
    [0x0435] = 0xc5,
    [0x0444] = 0xc6,
    [0x0433] = 0xc7,
    [0x0445] = 0xc8,
    [0x0438] = 0xc9,
    [0x0439] = 0xca,
    [0x043a] = 0xcb,
    [0x043b] = 0xcc,
    [0x043c] = 0xcd,
    [0x043d] = 0xce,
    [0x043e] = 0xcf,
    [0x043f] = 0xd0,
    [0x044f] = 0xd1,
    [0x0440] = 0xd2,
    [0x0441] = 0xd3,
    [0x0442] = 0xd4,
    [0x0443] = 0xd5,
    [0x0436] = 0xd6,
    [0x0432] = 0xd7,
    [0x044c] = 0xd8,
    [0x044b] = 0xd9,
    [0x0437] = 0xda,
    [0x0448] = 0xdb,
    [0x044d] = 0xdc,
    [0x0449] = 0xdd,
    [0x0447] = 0xde,
    [0x044a] = 0xdf,
    [0x042e] = 0xe0,
    [0x0410] = 0xe1,
    [0x0411] = 0xe2,
    [0x0426] = 0xe3,
    [0x0414] = 0xe4,
    [0x0415] = 0xe5,
    [0x0424] = 0xe6,
    [0x0413] = 0xe7,
    [0x0425] = 0xe8,
    [0x0418] = 0xe9,
    [0x0419] = 0xea,
    [0x041a] = 0xeb,
    [0x041b] = 0xec,
    [0x041c] = 0xed,
    [0x041d] = 0xee,
    [0x041e] = 0xef,
    [0x041f] = 0xf0,
    [0x042f] = 0xf1,
    [0x0420] = 0xf2,
    [0x0421] = 0xf3,
    [0x0422] = 0xf4,
    [0x0423] = 0xf5,
    [0x0416] = 0xf6,
    [0x0412] = 0xf7,
    [0x042c] = 0xf8,
    [0x042b] = 0xf9,
    [0x0417] = 0xfa,
    [0x0428] = 0xfb,
    [0x042d] = 0xfc,
    [0x0429] = 0xfd,
    [0x0427] = 0xfe,
    [0x042a] = 0xff,
}
unicode.char = function(code)
    local c = unicode2koi8r[code]
    if c ~= nil then
        return string.char(c)
    end
    if code > 0xff then
        --print("char "..code)
        return '?'
    else
        return string.char(code)
    end
end
unicode.charWidth = function()
    return 1
end
unicode.isWide = function()
    return false
end
unicode.wtrunc = function(line, width)
    return string.sub(line, 1, width)
end

system = {
    timeout = function()
        return 0
    end,

    allowBytecode = function()
        return true
    end,
}

userdata = {
    methods = function()
        return {}
    end,

    dispose = function()
    end,
}

start_time = time()

computer = {
    isRobot = false,
    address = function()
        return component.getPrimaryAddr("computer")
    end,
    tmpAddress = function()
        return nil
    end,
    freeMemory = function()
        return 1024 * 1024 - 100
    end,
    totalMemory = function()
        return 1024 * 1024
    end,
    uptime = function()
        return time() - start_time
    end,
    energy = 1000000,
    maxEnergy = 1000000,
    users = {},
    addUser = function() end,
    removeUser = function() end,

    pushSignal = function(signal, ...)
        io.write("pushSignal: ")
        t = table.pack(signal, ...)
        for i = 1,t.n do
            io.write(tostring(t[i]).." ")
        end
        io.write("\n")
    end,

    realTime = function()
        --return os.time() * 0.01
        --return time()
        return 111
    end,
}

local computer_api = {
    beep = function()
    end,
}

local function validatePath(self, path)
    local path = self.__path .. "/" .. path
    if string.sub(path, -1) == "/" then
        path = string.sub(path, 1, -2)
    end
    --print("Validated path: "..path)
    return path
end

local filesystem = {
    open = function(self, path, mode)
        local path = validatePath(self, path)
        if mode == nil then
            mode = "r"
        end
        local f, err = io.open(path, mode)
        if f == nil then
            return nil, err
        end
        return f
    end,

    read = function(self, f, size)
        if size > 100000 then
            size = 100000
        end
        return f:read(size)
    end,

    write = function(self, f, data)
        return f:write(data)
    end,

    close = function(self, f)
        f:close()
    end,

    getLabel = function(self)
        return ""
    end,

    list = function(self, dir)
        local path = validatePath(self, dir)

        local attr = lfs.attributes(path)
        if attr == nil then
            return nil, "no such directory"
        end

        t = {}
        for f in lfs.dir(path) do
            if f ~= "." and f ~= ".." then
                p2 = path.."/"..f
                if lfs.attributes(p2).mode == "directory" then
                    f = f.."/"
                end
                t[#t+1] = f
            end
        end
        return t
    end,

    isDirectory = function(self, path)
        local path = validatePath(self, path)
        local attr = lfs.attributes(path)
        if attr == nil then
            return false
        else
            return attr.mode == "directory"
        end
    end,

    exists = function(self, path)
        local path = validatePath(self, path)
        local attr = lfs.attributes(path)
        return attr ~= nil
    end,

    isReadOnly = function()
        return false
    end,
}

component = {
    list = function(filter, exact)
        r = {}
        if filter == nil then
            for k,v in pairs(intcomponent.list) do
                r[k] = v.type
            end
            return r
        else
            for k,v in pairs(intcomponent.list) do
                if exact then
                    if v.type == filter then
                        r[k] = v.type
                    end
                else
                    if string.find(v.type, filter) ~= nil then
                        r[k] = v.type
                    end
                end
            end
            return r
        end
    end,

    methods = function(address)
        local c = intcomponent.list[address]
        if c == nil then
            return {}
        end

        local m = c.methods
        if m == nil then
            print("Method table not found for "..c.type)
            return {}
        end

        local r = {}
        for k,v in pairs(m) do
            if type(v) == "function" then
                r[k] = {direct = true} -- TODO: .getter, .setter
            end
        end
        return r
    end,

    invoke = function(...)
        local args = {...}
        if #args < 2 then
            error("invoke: bad args")
        end
        address = args[1]
        method = args[2]
        args = table.pack(table.unpack(args, 3, args.n))

        local c = intcomponent.list[address]
        if c == nil then
            error("No such component: "..address)
        end

        local m = c.methods
        if m == nil then
            error("Method table not found for "..address)
        end

        m = m[method]
        if m == nil then
            error("No such method: "..c.type..":"..method)
        end

        --[[io.write("invoke "..address..":"..method.."(")
        for i=1,args.n do
            if i ~= 1 then
                io.write(", ")
            end
            io.write(tostring(args[i]))
        end
        io.write(")\n")]]--

        local r = table.pack(m(c, table.unpack(args)))
        --[[print("result:")
        for k,v in pairs(r) do
            print("  "..k.." => "..tostring(v))
        end]]--
        return true, table.unpack(r)
    end,

    type = function(address)
        local c = intcomponent.list[address]
        if c == nil then
            return nil
        end

        return c.type
    end,

    slot = function(address)
        return 0
    end,

    -- custom function!
    getPrimaryAddr = function(type)
        for address,c in pairs(intcomponent.list) do
            if c.type == type then
                return address
            end
        end
    end,

    doc = function()
        return ""
    end,
}

computer_mt = {
    __index = function(self, index)
        print("Not implemented: computer."..index)
    end
}

component_mt = {
    __index = function(self, index)
        print("Not implemented: component."..index)
    end
}

setmetatable(computer, computer_mt)
setmetatable(component, component_mt)

computer.__addr = next(component.list("computer", true))

print("Components:")
for address,c in pairs(intcomponent.list) do
    print(address.." => "..c.type)
    if c.type == "eeprom" then
        local function get_file(self, filename)
            local path = validatePath(self, filename)
            local f = io.open(path, "r")
            if f == nil then
                return nil
            end
            local data = f:read(100000)
            f:close()
            return data
        end

        local function set_file(self, filename, data)
            local path = validatePath(self, filename)
            local f = io.open(path, "w")
            if f == nil then
                return false
            end
            f:write(data)
            f:close()
            return true
        end

        c.methods = {
            get = function(self)
                local data = get_file(self, "init.lua")
                if data == nil then
                    print("Can't read init.lua file in EEPROM directory")
                end
                return data
            end,

            set = function(self, data)
                return set_file(self, "init.lua", data)
            end,

            getLabel = function(self)
                return get_file(self, "label")
            end,

            setLabel = function(self, label)
                return set_file(self, "label", label)
            end,

            getSize = function(self)
                return 1024 -- TODO: FIX
            end,

            getData = function(self)
                return get_file(self, "data")
            end,

            setData = function(self, label)
                return set_file(self, "data", label)
            end,
        }
    end
    if c.type == "filesystem" then
        c.methods = filesystem
    end
    if c.type == "computer" then
        c.methods = computer_api
    end
    if c.type == "screen" then
        c.methods = {
            getKeyboards = function()
                return {"zzz"}
            end
        }
    end
    if c.type == "gpu" then
        c.methods = {
            bind = function()
            end,

            getResolution = function(self)
                return 71, 25
            end,

            maxResolution = function(self)
                return 71, 25
            end,

            setResolution = function(self, x, y)
            end,

            setBackground = function(self, color)
                return self:set_color(0, color)
            end,

            setForeground = function(self, color)
                return self:set_color(1, color)
            end,

            getBackground = function(self)
                return self:get_color(0)
            end,

            getForeground = function(self)
                return self:get_color(1)
            end,

            fill = function(self, x, y, width, height, char)
                for yi=y,(y+height-1) do
                    for xi=x,(x+width-1) do
                        self:set(xi-1, yi-1, string.byte(char))
                    end
                end
            end,

            copy = function(self, x, y, w, h, tx, ty)
                self:copy(x-1, y-1, w, h, x+tx-1, y+ty-1)
            end,

            set = function(self, x, y, value, vertical)
                if vertical then
                    error("gpu:set: vertical is not implemented", 2)
                end
                if value == nil then
                    error("gpu.set: nil value")
                    return false
                end
                for i = 1,string.len(value) do
                    ch = string.sub(value, i, i)
                    self:set(x + i - 2, y - 1, string.byte(ch))
                end
                return true
            end,

            get = function(self, x, y)
                return self:get(x - 1, y - 1)
            end,

            getScreen = function()
                return component.getPrimaryAddr("screen")
            end,

            getDepth = function()
                return 8
            end,
        }
    end
    if c.type == "keyboard" then
        c.methods = {}
    end
    if c.type == "transposer" then
        c.methods = {
            getInventorySize = function(self, side)
                return 16
            end,

            getStackInSlot = function(self, side, slot)
              return nil
            end,

            transferItem = function(self, ...)
              return false
            end,
        }
    end

    c.methods.address = address
end


--[[types = {}
for address,c in pairs(intcomponent.list) do
    types[c.type] = true
end
for type,_ in pairs(types) do
    address = component.getPrimaryAddr(type)
    c = intcomponent.list[address]
    component[type] = c.methods
end]]--

keyboard_addr = computer.keyboard

