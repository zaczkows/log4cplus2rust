mod handler;

pub fn redirect_log4cplus_logs() {
    handler::setup_log_redirection();
}

pub fn remove_redirection() {}

#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        assert_eq!(2 + 2, 4);
    }
}
