#include <pebble.h>

#define BUFF 64
static Window* window;
static TextLayer* text_layer;
static TextLayer* s_time_layer;

static char quote[] = "Text here";

static Layer* s_layer;

static void layer_set_text(char* quote, const char* set) {
  quote = malloc(strlen(set) + 1);
  strcpy(quote, set);
}

static void update_layer_callback(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_frame(layer);

  graphics_context_set_text_color(ctx, GColorChromeYellow);
  graphics_draw_text(ctx, quote, fonts_get_system_font(FONT_KEY_FONT_FALLBACK), GRect(5, 5, bounds.size.w - 10, 100), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  //graphics_draw_text(ctx, "And text here as well.", fonts_get_system_font(FONT_KEY_FONT_FALLBACK), GRect(90, 100, bounds.size.w - 95, 60), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
}

static AppSync sync;
static uint8_t sync_buffer[BUFF];

enum MsgKeys {
  MONEY = 0x0
};

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void sync_error(DictionaryResult dict_error, 
                       AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_success(const uint32_t key, const Tuple* new_tuple, 
                         const Tuple* old_tuple, void* context) {
APP_LOG(APP_LOG_LEVEL_DEBUG, "App Sync Success %s", new_tuple->value->cstring);
  text_layer_set_text(text_layer, new_tuple->value->cstring);
  layer_set_text(quote, new_tuple->value->cstring);
}



static void window_load(Window* window) {
  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  window_set_background_color(window, GColorOxfordBlue);
  text_layer = text_layer_create((GRect) {
    .origin = {0, 0},
    .size   = {bounds.size.w, bounds.size.h}
  });
  text_layer_set_text(text_layer, "Hello, World!");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorChromeYellow);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_overflow_mode(text_layer, GTextOverflowModeTrailingEllipsis);
 // layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  Tuplet initial_value[] = {
    TupletCString(MONEY, "$12,345,678")
  };
  
  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_value, 
                ARRAY_LENGTH(initial_value), sync_success, sync_error, NULL);
  
   // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 85, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorChromeYellow);
  text_layer_set_text(s_time_layer, "00:00");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  // Make sure the time is displayed from the start
  update_time();
  
  // set graphics layer
  s_layer = layer_create(bounds);
  layer_set_update_proc(s_layer, update_layer_callback);
  layer_add_child(window_layer, s_layer);
}

static void window_unload(Window* window) {
  text_layer_destroy(text_layer);
  text_layer_destroy(s_time_layer);
  layer_destroy(s_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
  
static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers){
    .load   = window_load,
    .unload = window_unload
  });
  bool animated = true;
  app_message_open(BUFF, BUFF);
  window_stack_push(window, animated);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}