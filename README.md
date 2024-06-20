# RFLang
> **Notice**
> This language is in active development and shouldnt be used quite yet.

RFLang is a programming language that has been in development for a number of months now. I've been interested in language development for a while now, and have been combining what I learn in school with what I learn through trial and error doing the wrong thing. As you can see from the repo name this is the third attempt at writing this language.

# Grammar
I didn't realize how much the grammar would evolve over the development of the language, which is really interesting.
If you want to look at the grammar, take a look at the `grammar.g` file. (Note: this is just a scratch text file that I used to experiment with various reduction rules.

> **Fun Fact**
> There is no parser generator used in this language

# Building
```bash
$ make
$ make install     # INSTALL_DIR = /usr/local/bin (default)
                   # export INSTALL_DIR=dir       (custom)
```
- The executable is located in `out/` and/or where `INSTALL_DIR` points

# CodeGen (Very WIP)
```bash
docker pull rfmineguy/rflang-x86-testing:latest
make build-x86
docker run --rm -t -v $(pwd):$(pwd) -w $(pwd) rfmineguy/rflang-x86-testing:latest
./out/rfc_x86 -f <file> -a codegen -t x86
```

# Libraries
- [cvector](https://github.com/eteran/c-vector)
- arena
    * [arena-original](https://github.com/tsoding/arena)
    * [arena-modified](https://github.com/rfmineguy/arena)
- [sv](https://github.com/tsoding/sv)
- [malloc-trace](https://github.com/rfmineguy/malloc-trace)
- [c-generics](https://github.com/rfmineguy/c-generics)

# External
- [docker-image](https://hub.docker.com/repository/docker/rfmineguy/rflang-x86-testing/general)
