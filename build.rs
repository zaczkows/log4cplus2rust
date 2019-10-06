use cc;
use std::env;
use std::path::PathBuf;
// use pkg_config;

fn main() {
    // Compile C++ code
    // This will work only on linux... for now...
    let include_path = "/usr/include/log4cplus";
    let lib_path = "/usr/lib/x86_64-linux-gnu";
    let lib_name = "log4cplus";
    {
        // let target = std::env::var("TARGET").unwrap();

        // let log4cplus_lib = match pkg_config::probe_library("log4cplus") {
        //     Ok(lib) => lib,
        //     Err(e) => panic!("Failed to find required log4cplus library: {}", e),
        // };

        cc::Build::new()
            .cpp(true)
            .flag("-std=c++11")
            .flag(&format!("-I{}", include_path))
            // .flag(&format!("-L{}", lib_path))
            // .flag(&format!("-l{}", lib_name))
            .file("src/log4cplus2c.cpp")
            .shared_flag(false)
            .static_flag(true)
            .compile("log4cplus2c");
    }

    // Generate bindings
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    {
        let bindings = bindgen::Builder::default()
            .header("src/log4cplus2c.h")
            .rust_target(bindgen::RustTarget::Stable_1_33)
            .clang_args(["-std=c++11", "-x", "c++"].iter())
            .whitelist_function("add_rust_logger_handler")
            .generate()
            .expect("Failed to generate bindings");

        // Write the bindings to the $OUT_DIR/bindings.rs file.
        bindings
            .write_to_file(out_path.join("bindings.rs"))
            .expect("Couldn't write bindings!");
    }

    println!("cargo:rerun-if-changed={}", "src/log4cplus2c.cpp");
    println!("cargo:rerun-if-changed={}", "src/log4cplus2c.h");
    println!("cargo:rustc-link-search={}", out_path.to_str().unwrap());
    println!("cargo:rustc-link-lib=static=log4cplus2c");
    println!("cargo:rustc-link-search={}", lib_path);
    println!("cargo:rustc-link-lib=dylib={}", lib_name);
    println!("cargo:rustc-flags=-l dylib=stdc++");
}
