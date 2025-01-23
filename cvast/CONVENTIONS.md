# Conventions

When contributing to CVast++, conventions are used to generalize and help improve the efficiency of the codebase. These conventions are **always** to be followed, and if not, the pull request will be denied.


## Before we begin...

Due to the frequent use of the following types, aliases have been assigned to them, as is as follows:

`std::vector<std::string>  ->  vec_str`

```cpp
using vec_str = std::vector<std::string>;
```

`std::tuple<int, bool  ->  parser_rt` (parser return type)

```cpp
using parser_rt = std::tuple<int, bool>;
```

Remember all these aliases as they are used frequently throughout the codebase (we obligate that you use these over writing out the entire type).

---
### `func_map`

`func_map`, the map for functions, is of type: `std::map<std::string, vec_str>`. This map is used to store the function name, return and argument types.

The order used in the `vec_str` is as follows:
- Return Type (`[index 0]`)
- Argument Types (`[index 1 - n]`)



---
### `parsers.h`

All parsers in the namespaces of `parser.h` is to be started with `_p`.

Parsers part of the `keywords` namespace only require a prefixed `_p`.

Symbols part of the `symbol` namespace are all to be ended with `_sym`.

An example of a `symbol` parser:
```cpp
inline parser_rt _pequals_sym(int position, const vec_str &tokenizedOutput) {
    if (tokenizedOutput[position] == "=") {
        return {position + 1, true};
    }
    return {position, false};
}
```



