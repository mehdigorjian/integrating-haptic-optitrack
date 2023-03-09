#include <cstdlib>

#include "hapticpos.h"

int main(int argc, char* argv[]) {
    // hcoord::curr_opti = {(double)std::rand() / RAND_MAX, (double)std::rand() / RAND_MAX, (double)std::rand() / RAND_MAX};

    hcoord::print_coord();
    for (int i = 0; i < hcoord::haptic_tip_positions.size(); i++) {
        printf("(%f,%f,%f)\t", hcoord::haptic_tip_positions[i][0], hcoord::haptic_tip_positions[i][1], hcoord::haptic_tip_positions[i][2]);
    }
    // for (int i = 0; i < hcoord::opti_postitions.size(); i++) {
    //     printf("(%f,%f,%f)\t", hcoord::opti_postitions[i][0], hcoord::opti_postitions[i][1], hcoord::opti_postitions[i][2]);
    // }
    return 0;
}