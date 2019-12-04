mod bindings;

use lazy_static::lazy_static;
use log;
use std::sync::Mutex;

#[derive(Default, Debug)]
struct LoggerHandler {}

impl LoggerHandler {
    fn new() -> LoggerHandler {
        LoggerHandler {}
    }
    // fn handle_callback() {}
}

lazy_static! {
    static ref S_LOGGER_HANDLER: Mutex<LoggerHandler> = Mutex::new(LoggerHandler::new());
}

unsafe extern "C" fn logger_callback_helper(
    target: *mut std::os::raw::c_void,
    name: *const std::os::raw::c_char,
) {
    if target.is_null() {
        panic!("Something went terribly wrong!");
    }

    let msg = std::ffi::CStr::from_ptr(name);
    log::warn!("{}", msg.to_str().unwrap());
}

pub fn setup_log_redirection() {
    unsafe {
        bindings::add_rust_logger_handler(
            Some(logger_callback_helper),
            &mut S_LOGGER_HANDLER.lock().unwrap() as *mut _ as *mut std::ffi::c_void,
        );
    }
}
