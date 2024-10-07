# Zombie
Zombie, or **Z**iarZer's **O**wn **M**ade **B**rainfuck **I**nt**E**rpreter, is a BrainFuck interpreter written in C.

## How to install
You can compile the project by simply running `make` (or `make zombie`) at the root of the repository.
If you want, you can copy the `zombie` binary file obtained in your `/bin` directory so you can use it from anywhere in your file system.

## How to use
The simplest way of using Zombie is to run `zombie file.bf`, which will interprete the BrainFuck code contained in `file.bf` with an allocated array of 100 bytes by default.\
If you want to decide of the size of the array, you can add the wanted size to the parameters. For example, if you wanted an array of 10 bytes, you should run `zombie file.bf 10`.

When interpreting the `,` command, inputs will be read from the standard input stream, so you can either type them as the program executes, or use a pipe to use another program's output as input:
```bash
echo 'Hello world!' | zombie take_input.bf
```

### Debug mode
You can get more explicit information on printed chars by enabling debug mode:
```bash
zombie file.bf 100 -d
```
A more complete debugger will eventually be implemented.
