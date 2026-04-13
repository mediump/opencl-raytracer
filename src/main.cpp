#include "app/state.h"

int main()
{
  State::init_cl();
  State::set_current_cl_program("kernels/kernel.cl",
                               "render_kernel");
  State::init_app(1280, 720);
}

// TODO (10/15):
  // * Connect CL kernel program to viewport to display
  //    the resulting image
