#![allow(dead_code)]
#![allow(non_camel_case_types)]

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

#[test]
fn check_basic_logging() {
    unsafe { setup_logging() };
}

#[test]
fn check_basic_setup() {
    unsafe {
        setup_logging();
        run_example();
    }
}
