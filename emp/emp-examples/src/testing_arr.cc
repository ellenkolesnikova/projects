#include <emp-tool/emp-tool.h>
#include <emp-tool/io/net_io_channel.h>
#include <emp-sh2pc/emp-sh2pc.h>
#include "/Users/Ellen/Desktop/coding/emp/emp-tool/emp-tool/circuits/array_edited.h"
#include <chrono>
// compare corresponding array elements and create a new array with the largest of each pair
// example:
// alice has input [9, 3, 7, 4]
// bob has input [3, 8, 5, 4, 10]
// output would be [9, 8, 7, 4, 10]

using namespace emp;

const int ARR_BOUND = 500;
const int NEGATIVE_INF = -1000000;


Array<Integer> find_max_it(Array<Integer> a, Array<Integer> b) {
  Array<Integer> result = Array<Integer>(0, PUBLIC, 32);

  ArrayIterator<Integer> b_iter = b.begin();

  // iterator
  for (Integer a_val : a) {
    Bit a_greater = a_val>*b_iter; // 1 if alice's is greater, 0 otherwise
    // syntax: thing_one.If(cond, thing_2). if cond is true, do thing 2, if cond is false do thing 1
    Integer val = (*b_iter).If(a_greater, a_val);
    result.append(val);
    ++b_iter;
  }
  
  return result;
}


Array<Integer> find_max(Array<Integer> a, Array<Integer> b) {
  Array<Integer> result = Array<Integer>(0, PUBLIC, 32);
  for (int i=0; i<ARR_BOUND; i++) {
    Bit a_greater = a[i]>b[i]; // 1 if alice's is greater, 0 otherwise
    // syntax: thing_one.If(cond, thing_2). if cond is true, do thing 2, if cond is false do thing 1
    Integer val = b[i].If(a_greater, a[i]);
    result.append(val);
  }
  return result;

}  


int main(int argc, char** argv) {
  // To run a secure computation between two players, this program should be run twice.
  // For example, open two terminals.
  // In the first execute with arguments `0 15`
  // In the second execute with arguments `1 35`
  // Both players will output 15 < 35, i.e. 1

  // read in the command line arguments
  const int SET_LEN = argc-2;
  int p = atoi(argv[1]);
  int player_set[SET_LEN];
  for (int i=0; i<SET_LEN; i++) {
        player_set[i] = atoi(argv[i+2]);
  }
  

  if (p == 0) {
    // player 0 is the generator Alice

    // The players need a communication channel that the MPC protocol will use
    // to send messages over. Here, we use `NetIO` which simply sets up a TCP channel.
    // Alice will act as the server (hence nullptr) and we arbitrarily choose port `55555`.
    NetIO io { nullptr, 11111 };
    setup_semi_honest(&io, emp::ALICE);

    // alice's input
    Array<Integer> a = Array<Integer>(player_set, SET_LEN, ARR_BOUND, ALICE, 32);
    // dummy values
    Array<Integer> b = Array<Integer>(ARR_BOUND, BOB, 32);

    //std::cout << b[0].reveal<int>() << "\n";

    auto start = std::chrono::high_resolution_clock::now();
    Array<Integer> res1 = find_max_it(a, b);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);

    //std::cout << "Time taken with iterator: " << duration.count() << " milliseconds" << endl;
    
    /*auto start1 = std::chrono::high_resolution_clock::now();
    Array<Integer> res = find_max(a, b);
    auto end1 = std::chrono::high_resolution_clock::now();

    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1-start1);*/

    //std::cout << "Time taken without iterator: " << duration1.count() << " milliseconds" << endl;
    

    for (Integer val : res1) {
      int num = val.reveal<int>();
      if (num != NEGATIVE_INF) {
        std::cout << num << "\n";
      }

    }

  } else {
    // player 1 is the evaluator Bob
    // Bob's actions are symmetric to Alice's. He acts as the client and sets
    // up the second argument.
    NetIO io { "127.0.0.1", 11111 };
    setup_semi_honest(&io, emp::BOB);

    // dummy values
    Array<Integer> a = Array<Integer>(ARR_BOUND, ALICE, 32);
    // bob's input
    Array<Integer> b = Array<Integer>(player_set, SET_LEN, ARR_BOUND, BOB, 32);

    //std::cout << b[0].reveal<int>() << "\n";


    auto start = std::chrono::high_resolution_clock::now();
    Array<Integer> res1 = find_max_it(a, b);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);

    //std::cout << "Time taken with iterator: " << duration.count() << " milliseconds" << endl;
    
    /*auto start1 = std::chrono::high_resolution_clock::now();
    Array<Integer> res = find_max(a, b);
    auto end1 = std::chrono::high_resolution_clock::now();

    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1-start1);*/

    //std::cout << "Time taken without iterator: " << duration1.count() << " milliseconds" << endl;

    for (Integer val : res1) {
      int num = val.reveal<int>();
      if (num != NEGATIVE_INF) {
        std::cout << num << "\n";
      }

    }
  }
}
