# CVast

## Description

CVast++ is the official C++ implementation of the Vast Programming Language. It is a complete rewrite, which now instead of the previously slow and unstable interpreter, it has a fast and dependable compiler. The compiler is written in C++ and uses the LLVM library to generate the machine code. The language is syntactically similar to Python and is interpreted and dynamically typed.

Vast is a set of programming languages following the same syntax. It is meant to give the programmer utmost freedom when coding, allowing customization previously unseen in other languages. The language is still in development, and as such, the number of keywords may change.


---
## Getting Started
Getting started is simple.

1. Clone the repository in home directory (*optional*)
```bash
cd ~
git clone https://github.com/Silicon27/Vast.git
```

2. Export the path to the Vast directory in `.bashrc`, `.bash_profile` or `.zshrc` file (depending on your shell) and creating an alias for the CVast Executable
```bash
nano ~/.bashrc
export PYTHONPATH=~/Vast:$PYTHONPATH
alias vast=~/Vast/cmake-build-debug/Vast
```
> Exit the editor by pressing `Ctrl + X`, then `Y` and finally `Enter`

> [!WARNING]\
> If your wish to install Vast in a different directory then the above step would be different. Make sure to replace the path with the correct path to the Vast directory. This also applies in the case your shell is not bash.

3. Source the file
```bash
source ~/.bashrc
```

**And you are done!**

You can now run the `vast` command in your terminal to run the Vast Compiler.
```bash
vast ~/Vast/examples/cvast/test/test.cv -o output.s
```


