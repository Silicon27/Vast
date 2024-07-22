# Debug logs in Vast

In your Vast interpreter, you have a variable that can be toggled on and off, and it's called the `debug_mode`.

This exists within both the `interpret.py` and `xvastrunner.py` files. This mode allows for visualization of a more wide view of actions taken by the interpreter. 
Almost every action taken by the interpreter has a log, and by switching on this view, it is possible to see beneath the curtains, and get to that pesky bug.

---
### How to read the logs
If you just got started with Vast, and find the debug view a bit too complicated to understand, don't worry, because this documentation dives explicitly into the ins and outs of what each log mean.

---
### Color

The logs are sorted into different colors that are unique to every type of log.

- `\033[0;36m` Is a shade of dark turquoise, and is used to define when any function logic is invoked
- `\033[0;32m` Is a dark green color, which used to define the normal output without debug mode
- `\033[1;31m` Is bright red, used to define the end of a chunk or errors that may show up
- `\033[1;33m` Is a dark yellow color, used to show the expanded packages
- `\033[94m` Is sky blue, used to show the contents of expanded `__package__` files

The remaining colors are either bound to be changed, or is as of currently, undefined.

---
### Chunks

`chunk` are blocks of code that are executed within a statement, and the debugged outputs from that `chunk` are the inner workings of that function.

As of currently, the following statements support `chunk`:

> - `expand` <br>
>   The `expand` statement's chunks look something like this: <br>
>    `_______________________________ 1 Expanded Chunk(s) _______________________________`<br>
>   **NOTE**: If the amount of expanded chunks exceed `1`, that means the interpreter has expanded the same package **twice**, and in the case that it does happen, please be sure to contact maintainers of Vast.
<br>

