/************************************************************
■日数計算（日付−日付） - 高精度計算サイト - Keisan
	https://keisan.casio.jp/exec/system/1177658154
	
	
■std::tm
	https://en.cppreference.com/w/cpp/chrono/c/tm
	
■【c++】明日、昨日、指定した日付を取得する
	https://blog.dalt.me/1248
	
■C の時間操作関数は tm 構造体の BSD 拡張を無視するという話
	https://blog.outer-inside.net/2021/02/c-bsd.html
	
■[C言語] 時間を扱う
	https://qiita.com/edo_m18/items/364ffc6713b81c4d1c87

■localtime関数
	https://programming-place.net/ppp/contents/c/appendix/reference/localtime.html
	contents
		返されるポインタは、標準ライブラリ内のどこかにある静的記憶域期間を持った構造体変数を指している。この構造体変数の内容は、他の標準ライブラリ関数の呼び出しによって上書きされることがある。
		たとえば、次回の gmtime関数や ctime関数の呼び出しで上書きされるかもしれない。そのため、あとで結果を使う場合は、コピーを取っておくべきである。
	
■時間情報の取得方法と扱い方
	https://www.mm2d.net/main/prog/c/time-01.html
	
	contents
	-	2038年問題

■UNIX時間 【UNIX time】 エポック秒 / UNIX時刻 / POSIX時間
	https://e-words.jp/w/UNIX%E6%99%82%E9%96%93.html
************************************************************/
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <ctime>

/************************************************************
************************************************************/

/******************************
******************************/
int main(void){
	/********************
	********************/
	int _born_year = 1978;
	int _born_month = 1 - 1; // 1月 = zero.
	int _born_day = 4;
	int _life_years = 80;
	
	int death_year = _born_year + _life_years - 1900;
	int death_month = _born_month;
	int death_day = _born_day;
	
	struct std::tm tm_death = {0, 0, 0, 1, 0, 0}; // 1900.1.1 00:00:00
	tm_death.tm_sec = 0;			// seconds after the minute [0-60]
	tm_death.tm_min = 0;			// minutes after the hour [0-59]
	tm_death.tm_hour = 0;	 		// hours since midnight [0-23]
	tm_death.tm_mday = death_day;	// day of the month [1-31]
	tm_death.tm_mon = death_month;	//months since January [0-11]
	tm_death.tm_year = death_year;	// years since 1900
	// tm_death.tm_wday;			// days since Sunday [0-6]
	// tm_death.tm_yday;			// days since January 1 [0-365]
	// tm_death.tm_isdst;			// Daylight Savings Time flag
	// tm_death.tm_gmtoff;			// offset from CUT in seconds
	
	printf("> CLOCKS_PER_SEC = \n");
	printf("%d\n", CLOCKS_PER_SEC);	// mac : 1000000, windows : 1000
	printf("\n");
	
	printf(">sizeof time_t\n");
	printf("%d [Byte] = %d [bit]\n", sizeof(time_t), sizeof(time_t) * 8);	// 8 [Byte] = 64 [bit]
	printf("\n");
	
	printf(">sizeof clock_t\n");
	printf("%d [Byte] = %d [bit]\n", sizeof(clock_t), sizeof(clock_t) * 8);	// 8 [Byte] = 64 [bit]
	printf("\n");
	
	printf("> I was born in\n");
	printf("%d.%02d.%02d\n", _born_year, _born_month + 1, _born_day);		// 1978.01.04
	printf("\n");
	
	printf("> end of my life is\n");
	printf("%d.%02d.%02d\n", death_year + 1900, death_month + 1, death_day);	// 2058.01.04
	printf("\n");
	
	/********************
	********************/
	struct std::tm *p_tm_now_local;
	struct std::tm *p_tm_now_global;
	
	time_t UnixTime;
	
	UnixTime = time(NULL);					// 現在時刻を取得
	p_tm_now_local = localtime(&UnixTime);	// 地方時に変換
	p_tm_now_global = gmtime(&UnixTime);	// 標準時に変換
	
	printf("> unix time                = %d\n", UnixTime);
	printf("mktime(p_tm_now_local)     = %d\n", std::mktime(p_tm_now_local));
	printf("timelocal(p_tm_now_local)  = %d\n", timelocal(p_tm_now_local));
	// printf("timegm(p_tm_now_local)     = %d\n", timegm(p_tm_now_local));
	// printf("mktime(p_tm_now_global)    = %d\n", std::mktime(p_tm_now_global));
	// printf("timelocal(p_tm_now_global) = %d\n", timelocal(p_tm_now_global));
	printf("timegm(p_tm_now_global)    = %d\n", timegm(p_tm_now_global));
	printf("\n");
	
	printf("> address of tm\n");			// different address
	printf("localtime : %p\n", p_tm_now_local);
	printf("gmtime    : %p\n", p_tm_now_global);
	printf("\n");
	
	printf("> timeZone : local\n");
	if(p_tm_now_local->tm_zone)	printf("timezone = %s\n", p_tm_now_local->tm_zone);			// JST
	else						printf("timezone = null\n");
	printf("\n");
	
	printf("> timeZone : global\n");
	if(p_tm_now_global->tm_zone)	printf("timezone = %s\n", p_tm_now_global->tm_zone);	// UTC
	else							printf("timezone = null\n");
	printf("\n");
	
	
	/********************
	********************/
	double Left_sec = 0;
	int Left_weeks = 0;
	int Left_days = 0;
	int Left_hours = 0;
	int Left_min = 0;
	
	std::time_t t_now	= std::mktime(p_tm_now_local);
	std::time_t t_death	= std::mktime(&tm_death);
	
	printf("> now is(local, ctime)\n");
	std::cout << std::ctime(&t_now);
	printf("\n");
	
	printf("> now is(local, asctime)\n");
	printf("%s", asctime(p_tm_now_local));
	printf("\n");
	
	printf("> now is(global, asctime)\n");
	printf("%s", asctime(p_tm_now_global));
	printf("\n");
	
	printf("> my life ends at\n");
	std::cout << std::ctime(&t_death);
	printf("\n");
	
	if ( t_death != (std::time_t)(-1) && t_now != (std::time_t)(-1) )
	{
		Left_sec = std::difftime(t_death, t_now);
		printf("> (difftime, minus) = \n");
		printf("%f\n", Left_sec);			// same result
		printf("%d\n", t_death - t_now);
		printf("\n");
		
		Left_days = int( Left_sec / (60 * 60 * 24) );
		Left_weeks = int(Left_days / 7);
		Left_sec -= Left_days * (60 * 60 * 24);
		
		Left_hours = int( Left_sec / (60 * 60) );
		Left_sec -= Left_hours * (60 * 60);
		
		Left_min = int( Left_sec / 60 );
		Left_sec -= Left_min * 60;
		
		
		printf("> my life left is\n");
		printf("%4d weeks (%6d days)  %02d:%02d:%02d\n", Left_weeks, Left_days, Left_hours, Left_min, int(Left_sec));
		printf("\n");
	}
	
	
	/********************
	********************/
	return 0;
}

