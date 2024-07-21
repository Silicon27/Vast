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