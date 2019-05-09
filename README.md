# AAA

"Aaa" is an end-to-end encrypted peer-to-peer instant messaging software.

**_Proudly Powered by Curve25519_**

## Disclaimer

Due to our limited ability, the complexity introduced, and our poor software engineering, this project is remained unfinished. It does not achieve all designed functionalities, includng chatting.

We have had a lesson from this project... generally speaking,

- don't introduce too many new technologies in a new project
- always have a good architecture first, then code
- team members should agree with most of the technologies involved (at least know about them)

This is a course project, far from finished, for reference only. Do not:

- use it
- take it serious

## Why "Aaa"?

This is an idea from Yvonne, which has no actual meaning.

## Build

```
make
```

The result binary will be in the current directory. Shadow build is not supported.

I should use `meson` instead of writing `Makefile`s by hand.

## Dependencies

- GTK 3
- GLib, GIO
- libsodium

## License

The software is licensed under [the MIT license](COPYING).
