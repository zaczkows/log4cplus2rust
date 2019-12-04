mod handler;

pub fn redirect_log4cplus_logs() {
    handler::setup_log_redirection();
}

pub fn remove_redirection() {}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn compiles_and_links() {
        redirect_log4cplus_logs();
        remove_redirection();
    }
}
