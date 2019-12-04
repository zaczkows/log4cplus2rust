mod bindings;
// use log4cplus2rust;

#[cfg(test)]
mod tests {
    // use super::*;
    use std::sync::Once;

    static INIT: Once = Once::new();

    /// Setup function that is only run once, even if called multiple times.
    fn setup_test() {
        INIT.call_once(|| {
            env_logger::init();
        });
    }

    #[test]
    fn check_basic_setup() {
        setup_test();
        unsafe {
            crate::bindings::setup_logging();
            crate::bindings::run_example();
        }
    }

    #[test]
    fn check_basic_logging() {
        setup_test();
        unsafe { crate::bindings::setup_logging() };
        log4cplus2rust::redirect_log4cplus_logs();
        unsafe { crate::bindings::run_example() };
        log4cplus2rust::remove_redirection();
    }
}
