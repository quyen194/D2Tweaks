
#include <windows.h>


namespace d2_tweaks {
namespace ui {

// Function to find the Diablo II window handle
HWND FindDiabloIIWindow() { return FindWindow("Diablo II", NULL); }

}  // namespace ui
}  // namespace d2_tweaks
