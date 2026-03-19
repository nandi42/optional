# Optional (C++)

A lightweight implementation of an `std::optional`-like type in C++, built to better understand object lifetime, memory handling, and modern C++ design, developed to work with older versions

## Features

* Stores optional values without dynamic allocation
* Manual control over construction and destruction

## Example

```cpp
op::optional<int> x;
if (!x) x = 5;
std::cout << x.value();
```

## Motivation

This project was created to:

* explore object lifetime management
* understand placement new and manual destruction
* use for later projects

## Future improvements

* Move semantics support
* Exception safety guarantees

## License

MIT License
