#ifndef __SUNRISE__
#define __SUNRISE__

#include "qlobal.h"

#define RTC_MODE2_CLOCK_SECOND_Pos  0            /**< \brief (RTC_MODE2_CLOCK) Second */
#define RTC_MODE2_CLOCK_SECOND_Msk  (0x3Ful << RTC_MODE2_CLOCK_SECOND_Pos)
#define RTC_MODE2_CLOCK_SECOND(value) ((RTC_MODE2_CLOCK_SECOND_Msk & ((value) << RTC_MODE2_CLOCK_SECOND_Pos)))
#define RTC_MODE2_CLOCK_MINUTE_Pos  6            /**< \brief (RTC_MODE2_CLOCK) Minute */
#define RTC_MODE2_CLOCK_MINUTE_Msk  (0x3Ful << RTC_MODE2_CLOCK_MINUTE_Pos)
#define RTC_MODE2_CLOCK_MINUTE(value) ((RTC_MODE2_CLOCK_MINUTE_Msk & ((value) << RTC_MODE2_CLOCK_MINUTE_Pos)))
#define RTC_MODE2_CLOCK_HOUR_Pos    12           /**< \brief (RTC_MODE2_CLOCK) Hour */
#define RTC_MODE2_CLOCK_HOUR_Msk    (0x1Ful << RTC_MODE2_CLOCK_HOUR_Pos)
#define RTC_MODE2_CLOCK_HOUR(value) ((RTC_MODE2_CLOCK_HOUR_Msk & ((value) << RTC_MODE2_CLOCK_HOUR_Pos)))
#define RTC_MODE2_CLOCK_HOUR_PM_Val     0x10ul  /**< \brief (RTC_MODE2_CLOCK) Afternoon Hour */
#define RTC_MODE2_CLOCK_HOUR_PM     (RTC_MODE2_CLOCK_HOUR_PM_Val   << RTC_MODE2_CLOCK_HOUR_Pos)
#define RTC_MODE2_CLOCK_DAY_Pos     17           /**< \brief (RTC_MODE2_CLOCK) Day */
#define RTC_MODE2_CLOCK_DAY_Msk     (0x1Ful << RTC_MODE2_CLOCK_DAY_Pos)
#define RTC_MODE2_CLOCK_DAY(value)  ((RTC_MODE2_CLOCK_DAY_Msk & ((value) << RTC_MODE2_CLOCK_DAY_Pos)))
#define RTC_MODE2_CLOCK_MONTH_Pos   22           /**< \brief (RTC_MODE2_CLOCK) Month */
#define RTC_MODE2_CLOCK_MONTH_Msk   (0xFul << RTC_MODE2_CLOCK_MONTH_Pos)
#define RTC_MODE2_CLOCK_MONTH(value) ((RTC_MODE2_CLOCK_MONTH_Msk & ((value) << RTC_MODE2_CLOCK_MONTH_Pos)))
#define RTC_MODE2_CLOCK_YEAR_Pos    26           /**< \brief (RTC_MODE2_CLOCK) Year */
#define RTC_MODE2_CLOCK_YEAR_Msk    (0x3Ful << RTC_MODE2_CLOCK_YEAR_Pos)
#define RTC_MODE2_CLOCK_YEAR(value) ((RTC_MODE2_CLOCK_YEAR_Msk & ((value) << RTC_MODE2_CLOCK_YEAR_Pos)))
#define RTC_MODE2_CLOCK_MASK        0xFFFFFFFFul /**< \brief (RTC_MODE2_CLOCK) MASK Register */



void value_to_time(u32* value, ST_Time* otime);
void time_to_value(ST_Time* time, u32* outval);
void sun_rise_set(ST_Time* time, float lat_it, float long_it, u32* osunrise, u32* osunset);
void time_to_text(u32* time, u8* text,bool onlytime);
#endif /* SUNRISE_H_ */




