mod handler;

pub fn setup_log4cplus_redirection() {
    handler::setup_log_redirection();
}

pub fn remove_log4cplus_redirection() {
    handler::remove_log_redirection();
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn compiles_and_links() {
        setup_log4cplus_redirection();
        remove_log4cplus_redirection();
    }
}
