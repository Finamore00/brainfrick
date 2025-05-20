mod interpreter;
use std::env;
use std::fs::File;
use std::io::BufReader;

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() != 2 {
        eprintln!("ERROR: Wrong usage");
        eprintln!("Usage: brainfrick <src_file>");
        return;
    }
    
    let src: File;

    match File::open(args[1].as_str()) {
        Ok(f) => src = f,
        Err(_) => {
            eprintln!("ERROR: Error encountered when opening file.");
            return;
        }
    }

    let mut int = interpreter::Interpreter::new(BufReader::new(src), std::io::stdin(), 20);
    
    if let Err(message) = int.run() {
        eprintln!("{}", message);
    }

    return;
}
