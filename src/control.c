// -----------------------------------------------------------------------------
// controller - the glue between the engine and the hardware
//
// reacts to events (grid press, clock etc) and translates them into appropriate
// engine actions. reacts to engine updates and translates them into user 
// interface and hardware updates (grid LEDs, CV outputs etc)
//
// should talk to hardware via what's defined in interface.h only
// should talk to the engine via what's defined in engine.h only
// ----------------------------------------------------------------------------

#include "compiler.h"
#include "string.h"

#include "control.h"
#include "interface.h"
#include "engine.h"

preset_meta_t meta;
preset_data_t preset;
shared_data_t shared;
int selected_preset;

// ----------------------------------------------------------------------------
// firmware dependent stuff starts here

#define SCREEN_LINE_COUNT 8
#define SCREEN_TIMER 0
#define SCREEN_REFRESH_TIME 63

u8 screenLine = 0;

static void updateScreen(void);

// ----------------------------------------------------------------------------
// functions for main.c

void init_presets(void) {
    // called by main.c if there are no presets saved to flash yet
    // initialize meta - some meta data to be associated with a preset, like a glyph
    // initialize shared (any data that should be shared by all presets) with default values
    // initialize preset with default values 
    // store them to flash
    
    for (u8 i = 0; i < get_preset_count(); i++) {
        store_preset_to_flash(i, &meta, &preset);
    }

    store_shared_data_to_flash(&shared);
    store_preset_index(0);
}

void init_control(void) {
    // load shared data
    // load current preset and its meta data
    
    load_shared_data_from_flash(&shared);
    selected_preset = get_preset_index();
    load_preset_from_flash(selected_preset, &preset);
    load_preset_meta_from_flash(selected_preset, &meta);

    // set up any other initial values and timers
    updateScreen();
    
    // set up repeating timer for teletype screen refresh
    add_timed_event(SCREEN_TIMER, SCREEN_REFRESH_TIME, 1);
}

static void updatePressure(u8 bar, u8 pressure) {
    set_cv(bar, (u16)pressure << 7);
}

static void updateGate(u8 button, u8 pressed) {
    if (get_gate_output_count() > 4) {
        set_gate(button, pressed);
    } else {
        if (button == 0)
            set_gate(0, pressed);
        else if (button == 2)
            set_gate(1, pressed);
        else if (button == 4)
            set_gate(2, pressed);
        else if (button == 6)
            set_gate(3, pressed);
    }
}

void process_event(u8 event, u8 *data, u8 length) {
    switch (event) {
        case MAIN_CLOCK_RECEIVED:
            break;
        
        case MAIN_CLOCK_SWITCHED:
            break;
    
        case GATE_RECEIVED:
            break;
        
        case GRID_CONNECTED:
            break;
        
        case GRID_KEY_PRESSED:
            break;
    
        case GRID_KEY_HELD:
            break;
            
        case ARC_ENCODER_COARSE:
            break;
    
        case FRONT_BUTTON_PRESSED:
            break;
    
        case FRONT_BUTTON_HELD:
            break;
    
        case BUTTON_PRESSED:
            break;
    
        case I2C_RECEIVED:
            break;
            
        case TIMED_EVENT:
            if (data[0] == SCREEN_TIMER) updateScreen();
            break;
        
        case MIDI_CONNECTED:
            break;
        
        case MIDI_NOTE:
            break;
        
        case MIDI_CC:
            break;
            
        case MIDI_AFTERTOUCH:
            break;
            
        case SHNTH_BAR:
            updatePressure(data[0], data[1]);
            break;
            
        case SHNTH_ANTENNA:
            break;
            
        case SHNTH_BUTTON:
            updateGate(data[0], data[1]);
            break;
            
        default:
            break;
    }
}

void updateScreen(void) {
    if (++screenLine >= SCREEN_LINE_COUNT) screenLine = 0;
    
    clear_screen();
    draw_str("SHNTH CONTROL", screenLine, 8, 0);
    refresh_screen();
}

void render_grid(void) {
    // render grid LEDs here or leave blank if not used
}

void render_arc(void) {
    // render arc LEDs here or leave blank if not used
}