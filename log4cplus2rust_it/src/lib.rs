mod bindings;

#[cfg(test)]
mod tests {
    // use super::*;
    // use log4cplus2rust;
    use std::io::Write;
    use std::sync::Once;
    static INIT: Once = Once::new();

    /// Setup function that is only run once, even if called multiple times.
    fn setup_test() {
        INIT.call_once(|| {
            env_logger::builder()
                .format(|buf, record| {
                    writeln!(
                        buf,
                        "RUST: [{}] <{}> {} ({}:{})",
                        record.level(),
                        record.metadata().target(),
                        record.args(),
                        record.file().unwrap(),
                        record.line().unwrap()
                    )
                })
                .is_test(true)
                .init();
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
        log4cplus2rust::setup_log4cplus_redirection();
        unsafe { crate::bindings::run_example() };
        log4cplus2rust::remove_log4cplus_redirection();
    }
}
