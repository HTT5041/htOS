#include <keyboard.h>
#include <ports.h>
#include <util.h>
#include <vga.h>

#define SC_BACKSPACE 0x0E
#define SC_LSHIFT 0x2A
#define SC_RHSIFT 0x36
#define SC_CAPSLOCK 0x3A
#define SC_ENTER 0x1C
#define SC_LCTRL 0x1D

#define KEYBOARD_BUF_LEN 128

//TODO: Do special keys like F1-12
//      Do numpad
//      

char kb_buf[128];
unsigned int kb_buf_index = 0;

bool accepting_input = false;

bool caps_lock = false;
bool extended_key = false;

bool pressed_keys[86] = { false };

const char keys[] = {
    0,    27,   '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',
    '-',  '=',  0,    9,    'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',
    'o',  'p',  '[',  ']',  0,    0,    'a',  's',  'd',  'f',  'g',  'h',
    'j',  'k',  'l',  ';',  '\'', '`',  0,    '\\', 'z',  'x',  'c',  'v',
    'b',  'n',  'm',  ',',  '.',  '/',  0,    '*',  0,    ' ',  0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0x1B, 0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0x0E, 0x1C, 0,    0,    0,
    0,    0,    0,    0,    0,    '/',  0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0,
    0,    0,    0,    0,    0,    0,    0,    0x2C,
};

const char shifted_keys[] = {
    0,    27,   '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',  ')',
    '_',  '+',  0,    9,    'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',
    'O',  'P',  '{',  '}',  0,    0,    'A',  'S',  'D',  'F',  'G',  'H',
    'J',  'K',  'L',  ':',  '"',  '~',  0,    '|',  'Z',  'X',  'C',  'V',
    'B',  'N',  'M',  '<',  '>',  '?',  0,    '*',  0,    ' ',  0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0x1B, 0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0x0E, 0x1C, 0,    0,    0,
    0,    0,    0,    0,    0,    '?',  0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0,
    0,    0,    0,    0,    0,    0,    0,    0x2C,
};

void handle_key_action(unsigned char scancode){
    if(scancode > 0x58){
        //Key is being released
        scancode -= 0x80;

        if(scancode == SC_CAPSLOCK){
            return; // The caps-lock logic is handled in the switch case below
        }

        pressed_keys[(int) scancode] = false;
        return;
    }
    pressed_keys[(int) scancode] = true;

    if(!accepting_input) return;

    switch (scancode){
        case SC_LSHIFT:
            break;
        case SC_RHSIFT:
            break;
        case SC_CAPSLOCK:
            caps_lock = !caps_lock;
            //Todo write to the keyboard the status
            break;
        case SC_ENTER:
            // The action that this key performs really depends on what is going on
            // kprint("\n");
            kb_buf[kb_buf_index] = '\0';
            accepting_input = false;
            break;
        case SC_LCTRL:
            break;
        default:
            if(pressed_keys[SC_LSHIFT] || pressed_keys[SC_RHSIFT] || caps_lock){
                size_t index = (size_t) scancode;

                if(index > sizeof(shifted_keys) / sizeof(char)) break;

                kprint_char(shifted_keys[index]);

                if(kb_buf_index < KEYBOARD_BUF_LEN - 1) { //Minus one for the trailing \0
                    kb_buf[kb_buf_index] = shifted_keys[index];
                    kb_buf_index++;
                }
            } else {
                size_t index = (size_t) scancode;

                if(index > sizeof(keys) / sizeof(char)) break;

                kprint_char(keys[index]);

                if(kb_buf_index < KEYBOARD_BUF_LEN - 1) { //Minus one for the trailing \0
                    kb_buf[kb_buf_index] = keys[index];
                    kb_buf_index++;
                }
            }
    }
}

void keyboard_handler(registers_t r){

    unsigned char scancode = b_in(0x60);

    //char buf[3];
    //char_to_hex(scancode, buf);
    //kprint("0x");
    //kprint(buf);

    if(scancode == SC_BACKSPACE){
        if(kb_buf_index > 0){
            kb_buf_index--;
            kb_buf[kb_buf_index] = 0;
        }
        terminal_backspace(true);
        return;
    }

    if (scancode == 0xE0){
        extended_key = true;
        return;
    }

    if (extended_key){
        //Todo handle extended keys

        extended_key = false;
        return;
    }
    handle_key_action(scancode);
}

char* get_string() {
    accepting_input = true;

    while(accepting_input) {
        ;
    }

    kb_buf_index = 0;
    return kb_buf;
}