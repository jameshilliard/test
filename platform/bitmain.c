#include "platform_interface.h"

#if SYS_ENABLE_LOGO
static const char logo[] =
"\r\n"
"                Bitmain, INC.\r\n"
"\r\n"
"         5555555555555555555555555\r\n"
"        5555                   5555\r\n"
"       5555                     5555\r\n"
"      5555                       5555\r\n"
"     5555       5555555555555555555555\r\n"
"    5555       555555555555555555555555\r\n"
"   5555                             5555\r\n"
"  5555                               5555\r\n"
" 5555                                 5555\r\n"
"5555555555555555555555555555          55555\r\n"
" 55555           555555555           55555\r\n"
"   55555           55555           55555\r\n"
"     55555           5           55555\r\n"
"       55555                   55555\r\n"
"         55555               55555\r\n"
"           55555           55555\r\n"
"             55555       55555\r\n"
"               55555   55555\r\n"
"                 555555555\r\n"
"                   55555\r\n"
"                     5\r\n"
"\r\n"
"           Bitmain RISC-V Coreplex\r\n";

#endif

long platform__disabled_hart_mask = 0x0;

#if SYS_ENABLE_LOGO
const char *platform__get_logo(void)
{
  return logo;
}
#endif

int platform__use_htif(void)
{
  return 1;
}
