# Building and Running EMP

This repository is meant to aid you in getting the EMP-Toolkit properly installed to the point where you can run some minimal examples.
By following the instructions below, you should be able to build and install EMP and be to run the included examples.

EMP provides an install script.
The instructions are [here](https://github.com/emp-toolkit/emp-readme),
or you can just run the following in a fresh directory:

```bash
wget https://raw.githubusercontent.com/emp-toolkit/emp-readme/master/scripts/install.py
python install.py -install -tool -ot -sh2pc
```

From here, try building the examples:
```bash
mkdir build
cd build
cmake ..
make
```

Read through the source code and try running the examples. E.g.:

in one terminal:
```bash
./GCD 0 15
```

in a second terminal:
```bash
./GCD 1 35
```

both terminals should output `5`.


I recommend looking first at `src/millionaire.cc`, then `src/gcd.cc`, then `src/circuitfile.cc`.
