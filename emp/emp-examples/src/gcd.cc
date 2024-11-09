#include <emp-tool/emp-tool.h>
#include <emp-tool/io/net_io_channel.h>
#include <emp-sh2pc/emp-sh2pc.h>


using namespace emp;

// In this example, we will securely compute the GCD of the two players' private inputs.
// The below code is the reference cleartext implementation we wish to emulate.
//
// This code can be used as a reference against which to understand the MPC implementation (below).
int gcd(int a, int b) {
  if (b == 0) return a;
  else return gcd(b, a%b);
}


// This function specifies the circuit computed by Gen and Eval.
//
// Note, GCD is (usually) computed recursively.
// In MPC, because we must represent our programs as circuits, we cannot support general recursion.
// However, we *can* support recursion up to some fixed, specified depth.
// The `bound` variable is a concrete bound on the maximum depth of the recursion.
Integer gcd(int bound, const Integer& a, const Integer& b) {
  // if we exceed the maximum recusive bound, return -1 as an indicator that the computation failed
  if (bound <= 0) {
    return Integer(32, -1, PUBLIC);
  }

  // The `if` condition compares `b` to a publicly known constant `0`.
  Bit cond = b == Integer(32, 0, PUBLIC);

  // Now, we either return `a` (if `b == 0`) or we recursively continue.
  // Note, we decrement `bound` to ensure our program always terminates.
  return gcd(bound-1, b, a % b).If(cond, a);
}


int main(int argc, char** argv) {
  // To run a secure computation between two players, this program should be run twice.
  // For example, open two terminals.
  // In the first execute with arguments `0 15`
  // In the second execute with arguments `1 35`
  // Both players will output gcd(15, 35), i.e. 5


  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <player-id (0 or 1)> <player-input>\n";
    std::cerr << "Example: " << argv[0] << " 0 35\n";
    std::exit(1);
  }

  // read in the command line arguments
  int p = atoi(argv[1]);
  int inp = atoi(argv[2]);


  if (p == 0) {
    // player 0 is the generator Alice

    // The players need a communication channel that the MPC protocol will use
    // to send messages over. Here, we use `NetIO` which simply sets up a TCP channel.
    // Alice will act as the server (hence nullptr) and we arbitrarily choose port `55555`.
    NetIO io { nullptr, 19283 };
    // A required call which sets the circuit execution and protocol execution engines
    setup_semi_honest(&io, ALICE);

    // `a` is Alice's input, so she passes `inp`.
    const Integer a { 32, inp, ALICE };

    // `b` is Bob's input. We use a dummy value `0`.
    const Integer b { 32, 0, BOB };

    // Finally, compute GCD and print the revealed output.
    std::cout << gcd(10, a, b).reveal<int>() << "\n";
  } else {
    // player 1 is the evaluator Bob
    // Bob's actions are symmetric to Alice's. He acts as the client and sets
    // up the second argument.
    NetIO io { "127.0.0.1", 19283 };
    setup_semi_honest(&io, BOB);

    Integer a { 32, 0, ALICE };
    Integer b { 32, inp, BOB };
    std::cout << gcd(10, a, b).reveal<int>() << "\n";
  }
}
