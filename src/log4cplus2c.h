extern "C" {
typedef void (*log_callback)(void *data, const char *const name);

void add_rust_logger_handler(log_callback callback, void *callback_data);
}
