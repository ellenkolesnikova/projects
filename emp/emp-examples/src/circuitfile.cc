#include <emp-tool/execution/plain_prot.h>
#include <emp-sh2pc/emp-sh2pc.h>
#include <emp-tool/circuits/bit.h>
#include <emp-tool/circuits/circuit_file.h>

#include <iostream>


// In this example, we'll play with EMP's convenient feature for writing out circuit netlists to files.
// We can write circuits by writing stylized C++ programs, and later we can
// read the circuits back in from file for our purposes.

// We'll specify `test.txt` as the file we wish to write the netlist to.
// After you run this file, you should be able to take a look at `test.txt` to see the circuit written out.
static const char* FILENAME = "test.txt";

int main() {
  // Our Circuit format will be so called `Brisol Format`. Don't worry about
  // the name: the format is a simple listing of the gates line-by-line in
  // order.
  using Circuit = emp::BristolFormat;
  {
    // First, lets write a simple circuit out to a file. The high level
    // idea is that we will write our circuit as a highly stylized C++ program
    // in the context of an EMP "protocol execution". This protocol execution
    // will handle the details of constructing the circuit netlist for us.

    // We'll set up a so-called "plain execution", which will allow us to compile
    // our program to a circuit netlist file.
    emp::setup_plain_prot(true, FILENAME);

    // Next, we'll set up a simple circuit computation where Alice and Bob AND their private bits.

    // Here, the `false` value are simply dummy values.
    emp::Bit a_bit = { false, emp::ALICE };
    emp::Bit b_bit = { false, emp::BOB };

    emp::Bit res_bit = a_bit & b_bit;

    // Now, we can tell the protocol execution that we would like this result to be
    // part of the circuit output by calling `reveal`.
    res_bit.reveal<bool>();

    // Close the protocol execution.
    emp::finalize_plain_prot();
  }


  {
    // Second, we'll read in the same circuit from the file.
    // Note that this second piece i
    // EMP provides a `CircuitFile` struct for allowing us to interface with
    // netlist files.

    // Setup the protocol execution.
    emp::setup_plain_prot(true, "tmp");

    // We'll construct a circuit that reads in the file written in the first
    // part of this program.
    Circuit and_program { FILENAME };

    // The circuit file can then be executed by providing two pointers to
    // arrays of input bits (one from each of two allowable players).
    // Finally, the circuit takes an array of output bits.

    // Let's start by setting up the arrays;
    emp::Bit a_bits[] = {{ true, emp::ALICE }};
    emp::Bit b_bits[] = {{ true, emp::BOB }};
    emp::Bit out_bits[1];

    // Run the circuit file...
    and_program.compute(
        (emp::block*)(out_bits),
        (emp::block*)(a_bits),
        (emp::block*)(b_bits));

    // Should print 1 because AND computes TRUE here.
    std::cout << out_bits[0].reveal<bool>() << "\n";

    // Close the protocol execution.
    emp::finalize_plain_prot();
  }
}
