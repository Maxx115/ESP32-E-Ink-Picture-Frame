#include "test/EPD_7IN3F_tester/EPD_7IN3F_tester.hpp"
#include "test/Tester.hpp"

#include "self_arduino.hpp"
#include "EPD_7IN3F_Module.hpp"
#include "imagedata.hpp"

/* REQ 3.1 */
/* parameters: image data, xstart, ystart, image_width, image_height */
/* returns: success-flag */
//bool epd_showImage(const ubyte_t *image_data, int refresh_cnt, uword_t xstart = 0, uword_t ystart = 0, uword_t image_width = EPD_WIDTH, uword_t image_height = EPD_HEIGHT, bool color = false);

/* REQ 3.2 */
/* parameters: epd color code */
/* returns: success-flag */
//bool epd_showColor(ubyte_t color, int refresh_cnt, uword_t xstart = 0, uword_t ystart = 0, uword_t image_width = EPD_WIDTH, uword_t image_height = EPD_HEIGHT);

/* REQ 3.3 */
/* parameters: - */
/* returns: success-flag */
//bool epd_reset(void);

/* REQ 3.4 */
/* parameters: - */
/* returns: success-flag */
//bool epd_deepSleep(void);

/* REQ 3.5 */
/* parameters: - */
/* returns: success-flag */
//bool epd_init(int rst_pin_param = RST_PIN_DEFAULT, int dc_pin_param = DC_PIN_DEFAULT, int busy_pin_param = BUSY_PIN_DEFAULT);

void EPD_7IN3F_tester(void)
{
    Tester epd_init_test("Test-Unit: epd_init", "This test shall test the REQ 3.5");
    epd_init_test.begin();
    epd_init_test.tester_assert(epd_init(), true);
    epd_init_test.end();

    Tester epd_showImage_test("Test-Unit: epd_showImage", "This test shall test the REQ 3.1");
    epd_showImage_test.begin();
    epd_showImage_test.tester_assert(epd_showImage(gImage_7in3f, 1, 250, 150, 300, 180), true);
    epd_showImage_test.end();

    Tester epd_showColor_test("Test-Unit: epd_showColor", "This test shall test the REQ 3.2");
    epd_showColor_test.begin();
    epd_showColor_test.tester_assert(epd_showColor(EPD_7IN3F_WHITE, 1), true);
    epd_showColor_test.end();

    Tester epd_reset_test("Test-Unit: epd_reset", "This test shall test the REQ 3.3");
    epd_reset_test.begin();
    epd_reset_test.tester_assert(epd_reset(), true);
    epd_reset_test.end();

    Tester epd_deepSleep_test("Test-Unit: epd_deepSleep", "This test shall test the REQ 3.4");
    epd_deepSleep_test.begin();
    epd_deepSleep_test.tester_assert(epd_deepSleep(), true);
    epd_deepSleep_test.end();
}
