//! This is just simple include for automatically generated bindgen bindings to C/C++ code

#![allow(dead_code)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
