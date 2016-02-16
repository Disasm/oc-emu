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
    [0x2500] = 128,
    [0x2502] = 129,
    [0x250C] = 130,
    [0x2510] = 131,
    [0x2514] = 132,
    [0x2518] = 133,
    [0x251C] = 134,
    [0x2524] = 135,
    [0x252C] = 136,
    [0x2534] = 137,
    [0x253C] = 138,
    [0x2580] = 139,
    [0x2584] = 140,
    [0x2588] = 141,
    [0x258C] = 142,
    [0x2590] = 143,
    [0x2550] = 160,
    [0x2551] = 161,
    [0x2552] = 162,
    [0x2553] = 164,
    [0x2554] = 165,
    [0x2555] = 166,
    [0x2556] = 167,
    [0x2557] = 168,
    [0x2558] = 169,
    [0x2559] = 170,
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

        io.write("invoke "..address..":"..method.."(")
        for i=1,args.n do
            if i ~= 1 then
                io.write(", ")
            end
            io.write(tostring(args[i]))
        end
        io.write(")\n")

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
        c.methods = {
            get = function(self)
                return self:get_file("init.lua")
            end,

            set = function(self, data)
                self:set_file("init.lua", data)
            end,

            getLabel = function(self)
                return self:get_file("label")
            end,

            setLabel = function(self, label)
                self:set_file("label", label)
            end,

            getSize = function(self)
                return 1024 -- TODO: FIX
            end,
            
            getData = function(self)
                return self:get_file("data")
            end,

            setData = function(self, label)
                self:set_file("data", label)
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
                return 80, 25
            end,
            
            maxResolution = function(self)
                return 80, 25
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
                local r = self:get(x - 1, y - 1)
                return r
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

