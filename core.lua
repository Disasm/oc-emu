print("Started core.lua")

local function uuid()
    local template ='xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'
    local s, _ = string.gsub(template, '[xy]', function (c)
        local v = (c == 'x') and math.random(0, 0xf) or math.random(8, 0xb)
        return string.format('%x', v)
    end)
    return s
end

local function component_register(type, t)
    t.type = type
    local addr = uuid()
    intcomponent.list[addr] = t
end

local argv = {...}
for i = 1,#argv do
    print("arg:"..argv[i])
end

if #argv == 0 then
    return
end

intcomponent = {list={}}
component_register("eeprom", {__path=argv[1]})
for i = 2,#argv do
    component_register("filesystem", {__path=argv[i]})
end
component_register("keyboard", {})
component_register("screen", {})
component_register("computer", {})
component_register("gpu", int_gpu)
component_register("transposer", {})
component_register("redstone", {})
local scancodes = loadfile("scancode.lua")()

local addr = {}
for address, c in pairs(intcomponent.list) do
    addr[c.type] = address
end

pcall(loadfile("init.lua"))

local f = loadfile("machine.lua")
local co = coroutine.create(f)

local event_queue = {}

computer.pushSignal = function(signal, ...)
  print("computer.pushSignal: "..signal)
  local t = table.pack(signal, ...)
  event_queue[#event_queue+1] = t
end

while true do
    local ev = table.pack(get_event())
    if ev.n > 0 then
        print(table.unpack(ev))
        local send = false
        if (ev[1] == "key_down") or (ev[1] == "key_up") then
            local sdl_scancode = ev[2]
            local m = scancodes[sdl_scancode]
            if m ~= nil then
                print(table.unpack(m))
                local scancode = m[1]

                local ch1
                if m[2] ~= nil then
                    ch1 = string.byte(m[2])
                else
                    ch1 = 0
                end
                local ch2
                if m[3] ~= nil then
                    ch2 = string.byte(m[3])
                else
                    ch2 = ch1
                end

                local ch
                if ev[3] == 1 then
                    ch = ch2
                else
                    ch = ch1
                end
                ev = {ev[1], addr["keyboard"], ch, scancode, "username", n=5}
                send = true
            end
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
    if (ev.n == 0) and (#event_queue > 0) then
      ev = event_queue[1]
      table.remove(event_queue, 1)
    end
    cmd = table.pack(coroutine.resume(co, table.unpack(ev)))
    if (cmd.n > 0) and (type(cmd[1]) == "string") then
      event_queue[#event_queue+1] = cmd
    end
end

