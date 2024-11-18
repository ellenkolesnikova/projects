Secure computation is a cryptographic technique that allows players to compute arbitrary functions over encrypted data, thus protecting the privacy of secret inputs. Garbled circuits is a popular secure computation approach.

[EMP](https://github.com/emp-toolkit) is a fast C++ toolchain that converts C++ code to garbled circuits at run-time and then securely evaluates them. EMP includes garbled circuit support for basic C++ data types, such as int and float. The work in this paper builds upon the EMP library to include a new secure Array class, along with an iterator to reduce computation time in certain circumstances. These implementations are contained in array_edited.h and array_edited.hpp. set_intersection.cc and testing_arr.cc contain examples of uses of this array class.

See [my paper](https://drive.google.com/file/d/1WahTB1pNpuT1eXTBGTfVcJQkLtUb9QIb/view?usp=sharing) for more details.
