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
    level: u32,
    name: *const std::os::raw::c_char,
) {
    if target.is_null() {
        panic!("Something went terribly wrong!");
    }

    let msg = std::ffi::CStr::from_ptr(name);
    match level {
        0 => log::trace!("{}", msg.to_str().unwrap()),
        10000 => log::debug!("{}", msg.to_str().unwrap()),
        20000 => log::info!("{}", msg.to_str().unwrap()),
        30000 => log::warn!("{}", msg.to_str().unwrap()),
        40000 => log::error!("{}", msg.to_str().unwrap()),
        _ => log::error!("{}: {}", level, msg.to_str().unwrap())
    }
}

pub fn setup_log_redirection() {
    unsafe {
        bindings::add_rust_logger_handler(
            Some(logger_callback_helper),
            &mut S_LOGGER_HANDLER.lock().unwrap() as *mut _ as *mut std::ffi::c_void,
        );
    }
}
