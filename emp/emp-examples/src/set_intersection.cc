#include <emp-tool/emp-tool.h>
#include <emp-tool/io/net_io_channel.h>
#include <emp-sh2pc/emp-sh2pc.h>
#include "/Users/Ellen/Desktop/coding/emp/emp-tool/emp-tool/circuits/array_edited.h"
#include <vector>
#include <chrono>




using namespace emp;

const int EMP_INF=-1000000;

const int BOUND=1000;

// individual number finding in set
Bit individual_set_intersect(int bound, Integer val, Integer set[]) {

    

    Bit found = {false};
    const Bit ONE = {true};
    //auto start = std::chrono::high_resolution_clock::now();
    for (int i=0; i<bound; i++) {
        // cond is true if val == set[i]
        Bit cond = val==set[i];

        // found is a flag that turns true once a value i in set has been found
        // such that val == set[i]
        found = cond.If(found, ONE);

    }
    //auto end = std::chrono::high_resolution_clock::now();

    //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);

    //std::cout << "Time taken in individual set intersect: " << duration.count() << " microseconds" << endl;
    
    return found;
    
}

// set intersection function
/*std::vector<Integer> set_intersect(int bound, Integer a[], Integer b[]) {
    std::vector<Integer> curr_vec;
    const Integer INTEGER_INF {32, EMP_INF, PUBLIC};

    for (int i=0; i<bound; i++) {
        Integer a_num = a[i];

        Bit cond = {false};
        // cond is true when a_num is in b
        cond = individual_set_intersect(bound, a_num, b);

        Integer x = INTEGER_INF.If(cond, a_num);
        curr_vec.push_back(x);

    }
    return curr_vec;

}*/

std::vector<Integer> set_intersect(std::vector<Integer> a, std::vector<Integer> b) {
    const Integer INTEGER_INF {32, EMP_INF, PUBLIC};
    std::vector<Integer> result;
    const Bit ONE = {true};

    for (Integer a_val : a) {
        Bit found = {false};

        for (Integer b_val : b) {
            Bit cond = a_val==b_val;
            found = cond.If(found, ONE);
        }

    
    Integer x = INTEGER_INF.If(found, a_val);
            
    result.push_back(x);
    }

    return result;

}

Array<Integer> iterator_set_intersect(Array<Integer> a, Array<Integer> b) {
    const Integer INTEGER_INF {32, EMP_INF, PUBLIC};
    Array<Integer> result = Array<Integer>(0, PUBLIC, 32);
    const Bit ONE = {true};

    for (Integer a_val : a) {
        //auto start = std::chrono::high_resolution_clock::now();
        Bit found = {false};

        for (Integer b_val : b) {
            Bit cond = a_val==b_val;
            found = cond.If(found, ONE);
            

            //auto start = std::chrono::high_resolution_clock::now();
            //auto end = std::chrono::high_resolution_clock::now();

            //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);

            //std::cout << "Time taken in iterator function to append: " << duration.count() << " microseconds" << endl;
            

            
        }
        Integer x = INTEGER_INF.If(found, a_val);
            
        result.append(x);

        
        
    }
    return result;
}

int main(int argc, char** argv) {
    // make sure there are a correct number of args
    //std::cout<<"argscount: " << argc << "\n";
    if (argc<3) {
        std::cerr << "Usage: " << argv[0] << " <player-id (0 or 1)> <space-separated-set-values\n";
        std::cerr << "Example: " << argv[0] << " 0 0 1 2 3\n";
    }    

    const int SET_LEN = argc-2;

    // read in the inputs
    int player = atoi(argv[1]);
    int player_set[SET_LEN];
    for (int i=0; i<SET_LEN; i++) {
        player_set[i] = atoi(argv[i+2]);
    }


    if (player==0) {
        // this is the generator, alice
        // this sets up a channel for alice and bob to interact
        // alice is the server, hence the nullptr
        NetIO io {nullptr, 8888};
        // sets up the garble garble
        setup_semi_honest(&io, ALICE);


        // this is alice's array
        Integer a_set_Integer[BOUND];
        // this is bob's array (filled with dummy values)
        Integer b_set_Integer[BOUND];

        std::vector<Integer> a_vector;
        std::vector<Integer> b_vector;


        // filling arrays with alice and bob's inputs
        for (int i=0; i<BOUND; i++) {
            if (i< SET_LEN ){
                a_set_Integer[i] = Integer(32, player_set[i], ALICE);
                a_vector.push_back(Integer(32, player_set[i], ALICE));
            }
            else {
                a_set_Integer[i] = Integer (32, EMP_INF, ALICE);
                a_vector.push_back(Integer(32, EMP_INF, ALICE)); // same INF. to check
            }
        }

        for (int i=0; i<BOUND; i++) {
            b_set_Integer[i] = Integer (32, EMP_INF, BOB);
            b_vector.push_back(Integer(32, EMP_INF, BOB));

        }

        Array<Integer> iterator_a_set = Array<Integer>(player_set, SET_LEN, BOUND, ALICE, 32);
        Array<Integer> iterator_b_set = Array<Integer>(BOUND, BOB, 32);

        // compute the set intersection function
        //std::vector<Integer> vec;
        /*auto start = std::chrono::high_resolution_clock::now();
        std::vector<Integer> encrypted_calculated_set = set_intersect(a_vector, b_vector);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);*/

        //std::cout << "Time taken without iterator: " << duration.count() << " milliseconds" << endl;
        
        
        auto start1 = std::chrono::high_resolution_clock::now();
        Array<Integer> iterator_encrypted_calculated_set = iterator_set_intersect(iterator_a_set, iterator_b_set);
        auto end1 = std::chrono::high_resolution_clock::now();

        auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1-start1);

        //std::cout << "Time taken with iterator: " << duration1.count() << " milliseconds" << endl;
        
        //std::vector<int> final_set;
        std::vector<int> final_set_iterator;
        // get rid of blank values in the set and reveal correct values
        /*for (Integer val : encrypted_calculated_set) {
            int new_Val = val.reveal<int>();
            if (new_Val != EMP_INF) {
                final_set.push_back(new_Val);
            }
        }*/

        for (Integer val : iterator_encrypted_calculated_set) {
            int new_Val = val.reveal<int>();
            if (new_Val != EMP_INF) {
                //std::cout<<"hi\n";
                final_set_iterator.push_back(new_Val);
            }
        }



        for (int i : final_set_iterator) {
            std::cout << i << "\n";
        }


        
    }
    else {
        // player 1 is the evaluator bob
        // he pretty much does the exact same thing as alice

        NetIO io {"127.0.0.1", 8888};
        setup_semi_honest(&io, BOB);


        // this is alice's array
        Integer a_set_Integer[BOUND];
        // this is bob's array (filled with dummy values)
        Integer b_set_Integer[BOUND];

        std::vector<Integer> a_vector;
        std::vector<Integer> b_vector;


        // filling arrays
        for (int i=0; i<BOUND; i++) {
            a_set_Integer[i] = Integer (32, EMP_INF, ALICE);
            a_vector.push_back(Integer(32, EMP_INF, ALICE));
            
        }

        for (int i=0; i<BOUND; i++) {
            
            if (i<SET_LEN) {
                b_set_Integer[i] = Integer(32, player_set[i], BOB);
                b_vector.push_back(Integer(32, player_set[i], BOB));
            }
            else {
                b_set_Integer[i] = Integer (32, EMP_INF, BOB);
                b_vector.push_back(Integer(32, EMP_INF, BOB));
            }
            
        }

        Array<Integer> iterator_a_set = Array<Integer>(BOUND, ALICE, 32);
        Array<Integer> iterator_b_set = Array<Integer>(player_set, SET_LEN, BOUND, BOB, 32);
        

        // compute the set intersection function
        //std::vector<Integer> vec;
        /*auto start = std::chrono::high_resolution_clock::now();
        std::vector<Integer> encrypted_calculated_set = set_intersect(a_vector, b_vector);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);*/

        // << "Time taken without iterator: " << duration.count() << " milliseconds" << endl;
        

        auto start1 = std::chrono::high_resolution_clock::now();
        Array<Integer> iterator_encrypted_calculated_set = iterator_set_intersect(iterator_a_set, iterator_b_set);
        auto end1 = std::chrono::high_resolution_clock::now();

        auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1-start1);

        // << "Time taken with iterator: " << duration1.count() << " milliseconds" << endl;

        
        //std::vector<int> final_set;
        std::vector<int> final_set_iterator;
        // get rid of blank values in the set and reveal correct values
        /*for (Integer val : encrypted_calculated_set) {
            int new_Val = val.reveal<int>();
            if (new_Val != EMP_INF) {
                final_set.push_back(new_Val);
            }
        }*/

        for (Integer val : iterator_encrypted_calculated_set) {
            int new_Val = val.reveal<int>();
            if (new_Val != EMP_INF) {
                //std::cout<<"hi\n";
                final_set_iterator.push_back(new_Val);
            }
        }




        for (int i : final_set_iterator) {
            std::cout << i << "\n";
        }
    }
    

}
