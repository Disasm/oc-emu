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
        local send = false
        if (ev[1] == "key_down") or (ev[1] == "key_up") then
            ev = {ev[1], addr["keyboard"], ev[2], ev[3], "username", n=5}
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

