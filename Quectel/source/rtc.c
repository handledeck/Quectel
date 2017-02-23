
#include <qlobal.h>

u8 day_week(ST_Time *const time) {
	u8 a = (14 -time->month) / 12;
	u16 y = time->year - a;
	u8 m = time->month + 12 * a - 2;
	return (((7000 + (time->day + y + y / 4 - y / 100 + y / 400 + (31 * m) / 12))) % 7);
}

u32 rtc_calendar_time_to_register_value(ST_Time *time)
{
	u32 register_value;
	register_value = (time->year - 2000) <<	RTC_MODE2_CLOCK_YEAR_Pos;
	register_value |= (time->month << RTC_MODE2_CLOCK_MONTH_Pos);
	register_value |= (time->day << RTC_MODE2_CLOCK_DAY_Pos);
	register_value |= (time->hour << RTC_MODE2_CLOCK_HOUR_Pos);
	register_value |= (time->minute << RTC_MODE2_CLOCK_MINUTE_Pos);
	register_value |= (time->second << RTC_MODE2_CLOCK_SECOND_Pos);
	return register_value;
}


void rtc_calendar_register_value_to_time(const u32 register_value,ST_Time *time)
{
	time->year = ((register_value & RTC_MODE2_CLOCK_YEAR_Msk) >> RTC_MODE2_CLOCK_YEAR_Pos) + 2000;
	time->month = ((register_value & RTC_MODE2_CLOCK_MONTH_Msk) >> RTC_MODE2_CLOCK_MONTH_Pos);
	time->day = ((register_value & RTC_MODE2_CLOCK_DAY_Msk) >>	RTC_MODE2_CLOCK_DAY_Pos);
	time->hour = ((register_value & RTC_MODE2_CLOCK_HOUR_Msk) >> RTC_MODE2_CLOCK_HOUR_Pos);
	time->minute = ((register_value & RTC_MODE2_CLOCK_MINUTE_Msk) >> RTC_MODE2_CLOCK_MINUTE_Pos);
	time->second = ((register_value & RTC_MODE2_CLOCK_SECOND_Msk) >> RTC_MODE2_CLOCK_SECOND_Pos);
}