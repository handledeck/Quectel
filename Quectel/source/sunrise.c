#include <qlobal.h>


double pi = 3.14159;
double degs;
double rads;

double L, g, daylen;
double SunDia = 0.53;
double AirRefr = 34.0 / 60.0;


u8 day_of_week(int day, int month, int year)
{
	u64 tmp, res;
	if (month>2)
		month++;
	else
	{
		month += 13;
		year -= 1;
	}

	tmp = floor(365.25*year) + floor(30.6*month) + day - 621050;
	res = tmp - floor(tmp / 7) * 7 + 1;

	return res;
}


double FNday(int y, int m, int d, float h) {
	long int luku = -7 * (y + (m + 9) / 12) / 4 + 275 * m / 9 + d;
	luku += (long int)y * 367;
	return (double)luku - 730531.5 + h / 24.0;
};


double FNrange(double x) {
	double b = 0.5*x / pi;
	double a = 2.0*pi * (b - (long)(b));
	if (a < 0) a = 2.0*pi + a;
	return a;
};


double f0(double lat, double declin) {
	double fo, dfo;

	dfo = rads*(0.5*SunDia + AirRefr); if (lat < 0.0) dfo = -dfo;
	fo = tan(declin + dfo) * tan(lat*rads);
	if (fo>0.99999) fo = 1.0;
	fo = asin(fo) + pi / 2.0;
	return fo;
};

double f1(double lat, double declin) {
	double fi, df1;
	df1 = rads * 6.0; if (lat < 0.0) df1 = -df1;
	fi = tan(declin + df1) * tan(lat*rads);
	if (fi>0.99999) fi = 1.0;
	fi = asin(fi) + pi / 2.0;
	return fi;
};


double FNsun(double d) {

	L = FNrange(280.461 * rads + .9856474 * rads * d);
	g = FNrange(357.528 * rads + .9856003 * rads * d);
	return FNrange(L + 1.915 * rads * sin(g) + .02 * rads * sin(2 * g));
};


void showhrmn(double dhr, u8* filds) {
	volatile int hr, mn;
	hr = (int)dhr;
	mn = (dhr - (double)hr) * 60;
	sprintf((char*)filds, "%.2d:%.2d %c", hr, mn, '\0');
};


void sun_rise_set(ST_Time* time, float lat_it, float long_it, u32* osunrise, u32* osunset)
{
	double y, m, day, h, latit, longit;
	float inlat, inlon, intz;
	double tzone, d, lambda;
	double obliq, alpha, delta, LL, equation, ha, hb, twx;
	double twam, altmax, noont, settm, riset, twpm;

	degs = 180.0 / pi;
	rads = pi / 180.0;
	y = time->year;
	m = time->month;
	day = time->day;
	h = 12;
	latit = lat_it;
	longit = long_it;
	tzone = time->timezone;
	d = FNday(y, m, day, h);
	lambda = FNsun(d);
	obliq = 23.439 * rads - .0000004 * rads * d;
	alpha = atan2(cos(obliq) * sin(lambda), cos(lambda));
	delta = asin(sin(obliq) * sin(lambda));
	LL = L - alpha;
	if (L < pi) LL += 2.0*pi;
	equation = 1440.0 * (1.0 - LL / pi / 2.0);
	ha = f0(latit, delta);
	hb = f1(latit, delta);
	twx = hb - ha;
	twx = 12.0*twx / pi;
	daylen = degs*ha / 7.5;
	if (daylen<0.0001) { daylen = 0.0; }
	riset = 12.0 - (12.0 * ha / pi) + tzone - (longit / 15.0) + (equation / 60.0);
	settm = 12.0 + 12.0 * ha / pi + tzone - longit / 15.0 + equation / 60.0;
	noont = riset + 12.0 * ha / pi;
	altmax = 90.0 + delta * degs - latit;
	if (latit < delta * degs) altmax = 180.0 - altmax;
	twam = riset - twx;
	twpm = settm + twx;
	if (riset > 24.0) riset -= 24.0;
	if (settm > 24.0) settm -= 24.0;
	/*rmcpck->day_lenght = daylen;*/
	u8 hr = (u8)settm;
	u8 mn = (settm - (double)hr) * 60;
	ST_Time time_loc;
	time_loc.year = time->year;
	time_loc.month = time->month;
	time_loc.day = time->day;
	time_loc.hour = hr;
	time_loc.minute = mn;
	time_loc.second = 0;
	time_loc.timezone = time->timezone;
	time_to_value(&time_loc, osunrise);
	hr = (u8)riset;
	mn = (riset - (double)hr) * 60;
	time_loc.hour = hr;
	time_loc.minute = mn;
	time_to_value(&time_loc, osunset);
}

void time_to_text(u32* time, u8* text,bool onlytime) {
	ST_Time ti;
	value_to_time(time, &ti);
	if(onlytime)
		Ql_sprintf(text, "%.2d:%.2d:%.2d",ti.hour, ti.minute, ti.second);
	else
	Ql_sprintf(text, "%.2d.%.2d.%.2d %.2d:%.2d:%.2d", ti.year, ti.month, ti.day, ti.hour, ti.minute, ti.second);
}


void value_to_time(u32* value, ST_Time* otime) {
	u32 tvalue = *value;
	otime->year = ((tvalue & RTC_MODE2_CLOCK_YEAR_Msk) >>
		RTC_MODE2_CLOCK_YEAR_Pos) + 2000;
	otime->month = ((tvalue & RTC_MODE2_CLOCK_MONTH_Msk) >>
		RTC_MODE2_CLOCK_MONTH_Pos);
	otime->day = ((tvalue & RTC_MODE2_CLOCK_DAY_Msk) >>
		RTC_MODE2_CLOCK_DAY_Pos);
	otime->hour = ((tvalue & RTC_MODE2_CLOCK_HOUR_Msk) >>
		RTC_MODE2_CLOCK_HOUR_Pos);
	otime->minute = ((tvalue & RTC_MODE2_CLOCK_MINUTE_Msk) >>
		RTC_MODE2_CLOCK_MINUTE_Pos);
	otime->second = ((tvalue & RTC_MODE2_CLOCK_SECOND_Msk) >>
		RTC_MODE2_CLOCK_SECOND_Pos);
}

void time_to_value(ST_Time* time, u32* outval)
{
	u32 val = 0;
	val = ((time->year) - 2000) << RTC_MODE2_CLOCK_YEAR_Pos;
	val |= time->month << RTC_MODE2_CLOCK_MONTH_Pos;
	val |= time->day << RTC_MODE2_CLOCK_DAY_Pos;
	val |= time->hour << RTC_MODE2_CLOCK_HOUR_Pos;
	val |= time->minute << RTC_MODE2_CLOCK_MINUTE_Pos;
	val |= time->second << RTC_MODE2_CLOCK_SECOND_Pos;
	*outval = val;
}

bool in_interval(u32 current_time, u32 start_time, u32 end_time) {

	if (current_time>start_time && current_time<end_time)
		return TRUE;
	else
		return FALSE;
}
