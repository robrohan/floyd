#ifndef H_FLOYD_KEYBOARD
#define H_FLOYD_KEYBOARD

void enter_raw_mode();
void watch_key_press();

enum special_keys
{
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
    PAGE_UP,
    PAGE_DOWN
};

#endif
