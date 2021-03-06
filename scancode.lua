local scancodes = {
    [4]  = {0x1e, 'a', 'A'},
    [5]  = {0x30, 'b', 'B'},
    [6]  = {0x2e, 'c', 'C'},
    [7]  = {0x20, 'd', 'D'},
    [8]  = {0x12, 'e', 'E'},
    [9]  = {0x21, 'f', 'F'},
    [10] = {0x22, 'g', 'G'},
    [11] = {0x23, 'h', 'H'},
    [12] = {0x17, 'i', 'I'},
    [13] = {0x24, 'j', 'J'},
    [14] = {0x25, 'k', 'K'},
    [15] = {0x26, 'l', 'L'},
    [16] = {0x32, 'm', 'M'},
    [17] = {0x31, 'n', 'N'},
    [18] = {0x18, 'o', 'O'},
    [19] = {0x19, 'p', 'P'},
    [20] = {0x10, 'q', 'Q'},
    [21] = {0x13, 'r', 'R'},
    [22] = {0x1f, 's', 'S'},
    [23] = {0x14, 't', 'T'},
    [24] = {0x16, 'u', 'U'},
    [25] = {0x2f, 'v', 'V'},
    [26] = {0x11, 'w', 'W'},
    [27] = {0x2d, 'x', 'X'},
    [28] = {0x15, 'y', 'Y'},
    [29] = {0x2c, 'z', 'Z'},

    [30] = {0x02, '1', '!'},
    [31] = {0x03, '2', '@'},
    [32] = {0x04, '3', '#'},
    [33] = {0x05, '4', '$'},
    [34] = {0x06, '5', '%'},
    [35] = {0x07, '6', '^'},
    [36] = {0x08, '7', '&'},
    [37] = {0x09, '8', '*'},
    [38] = {0x0a, '9', '('},
    [39] = {0x0b, '0', ')'},

    [40] = {0x1c, '\n'}, -- return
    [41] = {0x00}, -- escape
    [42] = {0x0e}, -- backspace
    [43] = {0x0f, '\t'}, -- tab
    [44] = {0x39, ' '}, -- space
    [45] = {0x0c, '-', '_'}, -- minus
    [46] = {0x0d, '=', '+'}, -- equals
    [47] = {0x1a, '[', '{'}, -- leftbracket
    [48] = {0x1b, ']', '}'}, -- rightbracket
    [49] = {0x2b, '\\', '|'}, -- backslash

    [51] = {0x27, ';', ':'}, -- semicolon
    [52] = {0x28, "'", '"'}, -- apostrophe
    [53] = {0x29, '`', '~'}, -- grave
    [54] = {0x33, ',', '<'}, -- comma
    [55] = {0x34, '.', '>'}, -- period
    [56] = {0x35, '/', '?'}, -- slash

    [57] = {0x3a}, -- caps lock

    [58] = {0x3b}, -- f1
    [59] = {0x3c}, -- f2
    [60] = {0x3d}, -- f3
    [61] = {0x3e}, -- f4
    [62] = {0x3f}, -- f5
    [63] = {0x40}, -- f6
    [64] = {0x41}, -- f7
    [65] = {0x42}, -- f8
    [66] = {0x43}, -- f9
    [67] = {0x44}, -- f10
    [68] = {0x57}, -- f11
    [69] = {0x58}, -- f12

    [70] = {0x00}, -- printscreen
    [71] = {0x46}, -- scroll lock
    [72] = {0xc5}, -- pause

    [73] = {0xd2}, -- insert
    [74] = {0xc7}, -- home
    [75] = {0xc9}, -- pageup
    [76] = {0xd3}, -- delete
    [77] = {0xcf}, -- end
    [78] = {0xd1}, -- pagedown
    [79] = {0xcd}, -- right
    [80] = {0xcb}, -- left
    [81] = {0xd0}, -- down
    [82] = {0xc8}, -- up

    [224] = {0x1d}, -- lctrl
    [225] = {0x2a}, -- lshift
    [226] = {0x38}, -- lalt
    [227] = {0x00}, -- lgui
    [228] = {0x9d}, -- rctrl
    [229] = {0x36}, -- rshift
    [230] = {0xb8}, -- ralt
    [230] = {0x00}, -- rgui
}

return scancodes

