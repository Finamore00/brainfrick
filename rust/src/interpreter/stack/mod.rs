pub struct Stack<T> {
    elems: Vec<T>,
}

impl<T: Sized + Copy> Stack<T> {
    pub fn new(cap: usize) -> Self {
        Stack { 
            elems: Vec::with_capacity(cap), 
        }
    }

    pub fn is_empty(&self) -> bool {
        self.elems.len() <= 0
    }

    pub fn push(&mut self, elem: T) {
        self.elems.push(elem);
    }

    pub fn peek(&self) -> Option<T> {
        if self.is_empty() {
            return None;
        }

        Some(self.elems[self.elems.len() - 1])
    }

    pub fn pop(&mut self) -> Option<T> {
        self.elems.pop()
    }
}