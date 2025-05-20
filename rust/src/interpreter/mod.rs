mod stack;
use std::io::{Read, BufReader};

pub struct Interpreter<I: Read> {
    src: BufReader<std::fs::File>,
    input: I,
    mem_cap: usize,
    memory: Vec<u8>,
    mem_ptr: usize,
    pc: u64,
    st: stack::Stack<u64>
}

const TOKENS: [char;8] = ['+', '-', '>', '<', '.', ',', '[', ']'];

impl <I: Read> Interpreter<I> {
    pub fn new(src: BufReader<std::fs::File>, input: I, mem_cap: usize) -> Self {
        Interpreter { src: src, 
            input: input,
            mem_cap: mem_cap, 
            memory: vec![0; mem_cap], 
            mem_ptr: 0, 
            pc: 0, 
            st: stack::Stack::new(50) 
        }
    }

    pub fn run(&mut self) -> Result<(), String> {
        let mut src_buf: [u8;1] = [0];
        let mut input_buf: [u8;1] = [0];

        while let Ok(val) = self.src.read(&mut src_buf) {
            if val == 0 {
                break;
            }
            
            match src_buf[0] as char {
                '+' => self.memory[self.mem_ptr] = self.memory[self.mem_ptr].wrapping_add(1),
                '-' => self.memory[self.mem_ptr] = self.memory[self.mem_ptr].wrapping_sub(1),
                '>' => {
                    if self.mem_ptr < self.mem_cap - 1 {
                        self.mem_ptr += 1;
                    }
                },
                '<' => {
                    if self.mem_ptr > 0 {
                        self.mem_ptr -= 1;
                    }
                },
                '.' => {
                    print!("{}", self.memory[self.mem_ptr] as char);
                },
                ',' => {
                    match self.input.read(&mut input_buf) {
                        Ok(_) => self.memory[self.mem_ptr] = input_buf[0],
                        Err(_) => return Err(String::from("ERROR: Encountered error when reading from input buffer.")),
                    }
                },
                '[' => {
                    if self.memory[self.mem_ptr] == 0  {
                        //Look for matching ']' token
                        let mut depth: u64 = 1;
                        while depth > 0 {
                            if let Err(_) = self.src.read(&mut src_buf) {
                                break;
                            }

                            if src_buf[0] as char == '[' {
                                depth += 1;
                            }
                            if src_buf[0] as char == ']' {
                                depth -= 1
                            }

                            if TOKENS.contains(&(src_buf[0] as char)) {
                                self.pc += 1;
                            }
                        }

                        if depth != 0 {
                            return Err(String::from("ERROR: Found '[' token without matching ']'"));
                        }
                    } else {
                        self.st.push(self.pc);
                    }
                }
                ']' => {
                    if self.st.is_empty() {
                        return Err(String::from("ERROR: Found ']' token without matching '['"));
                    }

                    if self.memory[self.mem_ptr] != 0 {
                        let tgt_pc: u64 = self.st.peek().unwrap();
                        while self.pc > tgt_pc {
                            if let Err(_) = self.src.seek_relative(-2) {
                                return Err(String::from("ERROR: Encountered error when rewinding file."));
                            }

                            if let Err(_) = self.src.read(&mut src_buf) {
                                return Err(String::from("ERROR: Error when reading from source file."));
                            }

                            if TOKENS.contains(&(src_buf[0] as char)) {
                                self.pc -= 1;
                            }
                        }
                    } else {
                        self.st.pop();
                    }
                }
                _ => continue //Everything else is dismissed as a comment
            }

            self.pc += 1;
        }
        Ok(())
    }
}