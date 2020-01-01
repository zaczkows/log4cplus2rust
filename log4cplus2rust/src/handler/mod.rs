mod bindings;

use log;

unsafe extern "C" fn logger_callback_helper(details: *const bindings::LogInfoDetails) {
    let log_level = match (*details).level {
        0 => log::Level::Trace,
        10000 => log::Level::Debug,
        20000 => log::Level::Info,
        30000 => log::Level::Warn,
        40000 => log::Level::Error,
        _ => log::Level::Error,
    };

    // Need to do it "manually", so appropriate filename and line number can be set
    if log_level <= log::STATIC_MAX_LEVEL && log_level <= log::max_level() {
        let logger = std::ffi::CStr::from_ptr((*details).logger)
            .to_str()
            .unwrap();
        let msg: &str = std::ffi::CStr::from_ptr((*details).msg).to_str().unwrap();
        let file: &str = std::ffi::CStr::from_ptr((*details).file).to_str().unwrap();
        log::logger().log(
            &log::Record::builder()
                .args(format_args!("{}", msg))
                .level(log_level)
                .target(logger)
                .module_path_static(Some(module_path!()))
                .file(Some(file))
                .line(Some((*details).line as u32))
                .build(),
        );
    }
}

pub(crate) fn setup_log_redirection() {
    unsafe {
        bindings::add_rust_logger_handler(Some(logger_callback_helper));
    }
}

pub(crate) fn remove_log_redirection() {
    unsafe {
        bindings::remove_rust_logger_handler();
    }
}
