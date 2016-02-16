#ifndef SCANCODE_H
#define SCANCODE_H

struct k_info {
  unsigned char code;
  unsigned char type;
  char ch;
};

enum k_type {
  K_UNKNOWN = 0,
  K_NORMAL,
  K_EXTENDED,
  K_NUMLOCK_HACK,
  K_SHIFT_HACK,
};

static struct k_info keymap[SDL_NUM_SCANCODES] = {
  [SDL_SCANCODE_A] = { 0x1e, K_NORMAL, 'a' },
  [SDL_SCANCODE_B] = { 0x30, K_NORMAL, 'b' },
  [SDL_SCANCODE_C] = { 0x2e, K_NORMAL, 'c' },
  [SDL_SCANCODE_D] = { 0x20, K_NORMAL, 'd' },
  [SDL_SCANCODE_E] = { 0x12, K_NORMAL, 'e' },
  [SDL_SCANCODE_F] = { 0x21, K_NORMAL, 'f' },
  [SDL_SCANCODE_G] = { 0x22, K_NORMAL, 'g' },
  [SDL_SCANCODE_H] = { 0x23, K_NORMAL, 'h' },
  [SDL_SCANCODE_I] = { 0x17, K_NORMAL, 'i' },
  [SDL_SCANCODE_J] = { 0x24, K_NORMAL, 'j' },
  [SDL_SCANCODE_K] = { 0x25, K_NORMAL, 'k' },
  [SDL_SCANCODE_L] = { 0x26, K_NORMAL, 'l' },
  [SDL_SCANCODE_M] = { 0x32, K_NORMAL, 'm' },
  [SDL_SCANCODE_N] = { 0x31, K_NORMAL, 'n' },
  [SDL_SCANCODE_O] = { 0x18, K_NORMAL, 'o' },
  [SDL_SCANCODE_P] = { 0x19, K_NORMAL, 'p' },
  [SDL_SCANCODE_Q] = { 0x10, K_NORMAL, 'q' },
  [SDL_SCANCODE_R] = { 0x13, K_NORMAL, 'r' },
  [SDL_SCANCODE_S] = { 0x1f, K_NORMAL, 's' },
  [SDL_SCANCODE_T] = { 0x14, K_NORMAL, 't' },
  [SDL_SCANCODE_U] = { 0x16, K_NORMAL, 'u' },
  [SDL_SCANCODE_V] = { 0x2f, K_NORMAL, 'v' },
  [SDL_SCANCODE_W] = { 0x11, K_NORMAL, 'w' },
  [SDL_SCANCODE_X] = { 0x2d, K_NORMAL, 'x' },
  [SDL_SCANCODE_Y] = { 0x15, K_NORMAL, 'y' },
  [SDL_SCANCODE_Z] = { 0x2c, K_NORMAL, 'z' },

  [SDL_SCANCODE_1] = { 0x02, K_NORMAL, '1' },
  [SDL_SCANCODE_2] = { 0x03, K_NORMAL, '2' },
  [SDL_SCANCODE_3] = { 0x04, K_NORMAL, '3' },
  [SDL_SCANCODE_4] = { 0x05, K_NORMAL, '4' },
  [SDL_SCANCODE_5] = { 0x06, K_NORMAL, '5' },
  [SDL_SCANCODE_6] = { 0x07, K_NORMAL, '6' },
  [SDL_SCANCODE_7] = { 0x08, K_NORMAL, '7' },
  [SDL_SCANCODE_8] = { 0x09, K_NORMAL, '8' },
  [SDL_SCANCODE_9] = { 0x0a, K_NORMAL, '9' },
  [SDL_SCANCODE_0] = { 0x0b, K_NORMAL, '0' },

  [SDL_SCANCODE_RETURN]    = { 0x1c, K_NORMAL, '\n' },
  [SDL_SCANCODE_ESCAPE]    = { 0x76, K_NORMAL, 0 },
  [SDL_SCANCODE_BACKSPACE] = { 0x0e, K_NORMAL, 0 },
  [SDL_SCANCODE_TAB]       = { 0x0f, K_NORMAL, '\t' },
  [SDL_SCANCODE_SPACE]     = { 0x39, K_NORMAL, ' ' },

  [SDL_SCANCODE_MINUS]        = { 0x0c, K_NORMAL, '-' },
  [SDL_SCANCODE_EQUALS]       = { 0x0d, K_NORMAL, '=' },
  [SDL_SCANCODE_LEFTBRACKET]  = { 0x1a, K_NORMAL, '[' },
  [SDL_SCANCODE_RIGHTBRACKET] = { 0x1b, K_NORMAL, ']' },
  [SDL_SCANCODE_BACKSLASH]    = { 0x2b, K_NORMAL, '\\' },
  [SDL_SCANCODE_NONUSHASH]    = { 0x5D, K_NORMAL, 0 },  // same key as BACKSLASH

  [SDL_SCANCODE_SEMICOLON]  = { 0x27, K_NORMAL, ':' },
  [SDL_SCANCODE_APOSTROPHE] = { 0x28, K_NORMAL, '\'' },
  [SDL_SCANCODE_GRAVE]      = { 0x29, K_NORMAL, '`' },
  [SDL_SCANCODE_COMMA]      = { 0x33, K_NORMAL, ',' },
  [SDL_SCANCODE_PERIOD]     = { 0x34, K_NORMAL, '.' },
  [SDL_SCANCODE_SLASH]      = { 0x35, K_NORMAL, '/' },

  [SDL_SCANCODE_F1]  = { 0x3b, K_NORMAL, 0 },
  [SDL_SCANCODE_F2]  = { 0x3c, K_NORMAL, 0 },
  [SDL_SCANCODE_F3]  = { 0x3d, K_NORMAL, 0 },
  [SDL_SCANCODE_F4]  = { 0x3e, K_NORMAL, 0 },
  [SDL_SCANCODE_F5]  = { 0x3f, K_NORMAL, 0 },
  [SDL_SCANCODE_F6]  = { 0x40, K_NORMAL, 0 },
  [SDL_SCANCODE_F7]  = { 0x41, K_NORMAL, 0 },
  [SDL_SCANCODE_F8]  = { 0x42, K_NORMAL, 0 },
  [SDL_SCANCODE_F9]  = { 0x43, K_NORMAL, 0 },
  [SDL_SCANCODE_F10] = { 0x44, K_NORMAL, 0 },
  [SDL_SCANCODE_F11] = { 0x57, K_NORMAL, 0 },
  [SDL_SCANCODE_F12] = { 0x58, K_NORMAL, 0 },

  // Most of the keys below are not used by Oberon

  [SDL_SCANCODE_INSERT]   = { 0xd2, K_NUMLOCK_HACK, 0 },
  [SDL_SCANCODE_HOME]     = { 0xc7, K_NUMLOCK_HACK, 0 },
  [SDL_SCANCODE_PAGEUP]   = { 0xc9, K_NUMLOCK_HACK, 0 },
  [SDL_SCANCODE_DELETE]   = { 0xd3, K_NUMLOCK_HACK, 0 },
  [SDL_SCANCODE_END]      = { 0xcf, K_NUMLOCK_HACK, 0 },
  [SDL_SCANCODE_PAGEDOWN] = { 0xd1, K_NUMLOCK_HACK, 0 },
  [SDL_SCANCODE_RIGHT]    = { 0xcd, K_NUMLOCK_HACK, 0 },
  [SDL_SCANCODE_LEFT]     = { 0xcb, K_NUMLOCK_HACK, 0 },
  [SDL_SCANCODE_DOWN]     = { 0xd0, K_NUMLOCK_HACK, 0 },
  [SDL_SCANCODE_UP]       = { 0xc8, K_NUMLOCK_HACK, 0 },

  [SDL_SCANCODE_KP_DIVIDE]   = { 0xb5, K_SHIFT_HACK, 0 },
  [SDL_SCANCODE_KP_MULTIPLY] = { 0x37, K_NORMAL, 0 },
  [SDL_SCANCODE_KP_MINUS]    = { 0x4a, K_NORMAL, 0 },
  [SDL_SCANCODE_KP_PLUS]     = { 0x4e, K_NORMAL, 0 },
  [SDL_SCANCODE_KP_ENTER]    = { 0x9c, K_EXTENDED, 0 },
  [SDL_SCANCODE_KP_1]        = { 0x4f, K_NORMAL, 0 },
  [SDL_SCANCODE_KP_2]        = { 0x50, K_NORMAL, 0 },
  [SDL_SCANCODE_KP_3]        = { 0x51, K_NORMAL, 0 },
  [SDL_SCANCODE_KP_4]        = { 0x4b, K_NORMAL, 0 },
  [SDL_SCANCODE_KP_5]        = { 0x4c, K_NORMAL, 0 },
  [SDL_SCANCODE_KP_6]        = { 0x4d, K_NORMAL, 0 },
  [SDL_SCANCODE_KP_7]        = { 0x47, K_NORMAL, 0 },
  [SDL_SCANCODE_KP_8]        = { 0x48, K_NORMAL, 0 },
  [SDL_SCANCODE_KP_9]        = { 0x49, K_NORMAL, 0 },
  [SDL_SCANCODE_KP_0]        = { 0x52, K_NORMAL, 0 },
  [SDL_SCANCODE_KP_PERIOD]   = { 0x53, K_NORMAL, 0 },

  [SDL_SCANCODE_NONUSBACKSLASH] = { 0x61, K_NORMAL, 0 },
  [SDL_SCANCODE_APPLICATION]    = { 0x2F, K_EXTENDED, 0 },

  [SDL_SCANCODE_LCTRL]  = { 0x1d, K_NORMAL, 0 },
  [SDL_SCANCODE_LSHIFT] = { 0x2a, K_NORMAL, 0 },
  [SDL_SCANCODE_LALT]   = { 0x38, K_NORMAL, 0 },
  [SDL_SCANCODE_LGUI]   = { 0x1F, K_EXTENDED, 0 },
  [SDL_SCANCODE_RCTRL]  = { 0x9d, K_EXTENDED, 0 },
  [SDL_SCANCODE_RSHIFT] = { 0x36, K_NORMAL, 0 },
  [SDL_SCANCODE_RALT]   = { 0xb8, K_EXTENDED, 0 },
  [SDL_SCANCODE_RGUI]   = { 0x27, K_EXTENDED, 0 },
};


#endif /* SCANCODE_H */

