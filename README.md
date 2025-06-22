# Hyacinth

A Simple Programming Language that is started simply for learning C++

## Development

For development purposes on specific editors, a few instructions below can be followed

### Neovim

For **Neovim**, it requires a file named `compile_flags.txt` for the **LSP** to recognize the `include/` directory and be able to use the paths directly when including header files.

The content of the `compile_flags.txt` can simply be:

```
-Iinclude
-D_GNU_SOURCE
-DSHELL="path/to/shell"
```

The shell path can generally be found in:

#### Windows
`C:/Program Files/Git/bin/sh.exe`

#### Linux
`/bin/sh`
