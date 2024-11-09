#include <emp-tool/emp-tool.h>
#include <emp-tool/io/net_io_channel.h>
#include <emp-sh2pc/emp-sh2pc.h>

// In this example, we will securely compute a comparison of the two players' private inputs.

int main(int argc, char** argv) {
  // To run a secure computation between two players, this program should be run twice.
  // For example, open two terminals.
  // In the first execute with arguments `0 15`
  // In the second execute with arguments `1 35`
  // Both players will output 15 < 35, i.e. 1

  // read in the command line arguments
  int p = atoi(argv[1]);
  int inp = atoi(argv[2]);

  if (p == 0) {
    // player 0 is the generator Alice

    // The players need a communication channel that the MPC protocol will use
    // to send messages over. Here, we use `NetIO` which simply sets up a TCP channel.
    // Alice will act as the server (hence nullptr) and we arbitrarily choose port `55555`.
    NetIO io { nullptr, 55555 };
    setup_semi_honest(&io, emp::ALICE);

    // `a` is Alice's input, so she passes `inp`.
    const emp::Integer a { 32, inp, emp::ALICE };

    // `b` is Bob's input. We use a dummy value `0`.
    const emp::Integer b = { 32, 0, emp::BOB };

    // Finally, compute `<` and print the revealed output.
    emp::Bit res = a < b;
    std::cout << res.reveal<bool>() << "\n";

  } else {
    // player 1 is the evaluator Bob
    // Bob's actions are symmetric to Alice's. He acts as the client and sets
    // up the second argument.
    NetIO io { "127.0.0.1", 55555 };
    setup_semi_honest(&io, emp::BOB);

    const emp::Integer a { 32, 0, emp::ALICE };
    const emp::Integer b { 32, inp, emp::BOB };
    emp::Bit res = a < b;
    std::cout << res.reveal<bool>() << "\n";
  }
}
