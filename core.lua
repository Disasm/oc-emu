print("Started core.lua")

local function uuid()
    local template ='xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'
    local s, _ = string.gsub(template, '[xy]', function (c)
        local v = (c == 'x') and math.random(0, 0xf) or math.random(8, 0xb)
        return string.format('%x', v)
    end)
    return s
end

local addr = {}
for address, c in pairs(intcomponent.list) do
    addr[c.type] = address
end

pcall(loadfile("init.lua"))

local f = loadfile("machine.lua")
local co = coroutine.create(f)

while true do
    local ev = table.pack(get_event())
    if ev.n > 0 then
        print(table.unpack(ev))
        local send = false
        if (ev[1] == "key_down") or (ev[1] == "key_up") then
            ev = {ev[1], addr["keyboard"], ev[2], ev[3], "username", n=5}
            send = true
        end
        if (ev[1] == "mouse_down") then
            local x = math.floor(ev[2] / 8) + 1
            local y = math.floor(ev[3] / 16) + 1
            local button = 0
            if ev[4] == "left" then
                button = 1
            end
            ev = {"touch", addr["screen"], x, y, button, "username", n=6}
            send = true
        end

        if not send then
            ev = {n=0}
        end
    end
    cmd = coroutine.resume(co, table.unpack(ev))
    if cmd == "shutdown" then
        break
    end
end

