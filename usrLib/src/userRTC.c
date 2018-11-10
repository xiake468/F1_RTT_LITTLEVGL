#include "userRTC.h"

static volatile struct Calendar calendar;
static volatile struct Calendar*  pCalendar=&calendar;
static void vInit_RTC_ONCE(void);

void vInit_RTC(void)
{
    /* enable PMU and BKPI clocks */
    rcu_periph_clock_enable(RCU_BKPI);
    rcu_periph_clock_enable(RCU_PMU);
    /* allow access to BKP domain */
    pmu_backup_write_enable();

    if(bkp_read_data(BKP_DATA_0) != RTC_BKP_FLG)
    {
        vInit_RTC_ONCE();
    }
    else
    {
        /* wait for RTC registers synchronization */
        rtc_register_sync_wait();
        /* enable the RTC second interrupt*/
        rtc_interrupt_enable(RTC_INT_SECOND);
        /* wait until last write operation on RTC registers has finished */
        rtc_lwoff_wait();
    }

    nvic_irq_enable(RTC_IRQn, 0, 0);
    RTC_Get();

    if(calendar.w_year <= 1970)
    {
        vSet_RTC_Time(2018, 6, 9, 9, 23, 0);
    }
}

static void vInit_RTC_ONCE(void)
{
    /* reset backup domain */
    bkp_deinit();
    /* enable LXTAL */
    rcu_osci_on(RCU_LXTAL);
    /* wait till LXTAL is ready */
    rcu_osci_stab_wait(RCU_LXTAL);

    /* select RCU_LXTAL as RTC clock source */
    rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);

    /* enable RTC Clock */
    rcu_periph_clock_enable(RCU_RTC);
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
    /* wait for RTC registers synchronization */
    rtc_register_sync_wait();
    /* enable the RTC second interrupt*/
    rtc_interrupt_enable(RTC_INT_SECOND);
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
    rtc_configuration_mode_enter();
    /* set RTC prescaler: set RTC period to 1s */
    rtc_prescaler_set(32767);
    vSet_RTC_Time(2018, 5, 14, 9, 0, 0);
    rtc_configuration_mode_exit();
    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
    bkp_write_data(BKP_DATA_0, RTC_BKP_FLG);
}

//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
uint8_t Is_Leap_Year(uint16_t year)
{
    if(year % 4 == 0) //�����ܱ�4����
    {
        if(year % 100 == 0)
        {
            if(year % 400 == 0)return 1; //�����00��β,��Ҫ�ܱ�400����
            else return 0;
        }
        else return 1;
    }
    else return 0;
}
//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�
uint8_t const table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5}; //���������ݱ�
//ƽ����·����ڱ�
const uint8_t mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

uint8_t vSet_RTC_Time(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{
    uint16_t t;
    uint32_t seccount = 0;

    if(syear < 1970 || syear > 2099)return 1;

    for(t = 1970; t < syear; t++)	//��������ݵ��������
    {
        if(Is_Leap_Year(t))seccount += 31622400; //�����������
        else seccount += 31536000;			 //ƽ���������
    }

    smon -= 1;

    for(t = 0; t < smon; t++)	 //��ǰ���·ݵ����������
    {
        seccount += (uint32_t)mon_table[t] * 86400; //�·����������

        if(Is_Leap_Year(syear) && t == 1)seccount += 86400; //����2�·�����һ���������
    }

    seccount += (uint32_t)(sday - 1) * 86400; //��ǰ�����ڵ����������
    seccount += (uint32_t)hour * 3600; //Сʱ������
    seccount += (uint32_t)min * 60;	 //����������
    seccount += sec; //�������Ӽ���ȥ

    /* enable PMU and BKPI clocks */
    rcu_periph_clock_enable(RCU_BKPI);
    rcu_periph_clock_enable(RCU_PMU);
    /* allow access to BKP domain */
    pmu_backup_write_enable();
    rtc_counter_set(seccount);	//����RTC��������ֵ

    rtc_lwoff_wait();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
    RTC_Get();
    return 0;
}

void vSetTimeUnixForm(uint32_t   seccount)
{
    /* enable PMU and BKPI clocks */
    rcu_periph_clock_enable(RCU_BKPI);
    rcu_periph_clock_enable(RCU_PMU);
    /* allow access to BKP domain */
    pmu_backup_write_enable();
    rtc_counter_set(seccount);	//����RTC��������ֵ

    rtc_lwoff_wait();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
    RTC_Get();
}

uint8_t RTC_Get(void)
{
    static uint16_t daycnt = 0;
    uint32_t timecount = 0;
    uint32_t temp = 0;
    uint16_t temp1 = 0;
    timecount = rtc_counter_get();
    temp = timecount / 86400; //�õ�����(��������Ӧ��)

    if(daycnt != temp) //����һ����
    {
        daycnt = temp;
        temp1 = 1970;	//��1970�꿪ʼ

        while(temp >= 365)
        {
            if(Is_Leap_Year(temp1))//������
            {
                if(temp >= 366)temp -= 366; //�����������
                else
                {
                    temp1++;
                    break;
                }
            }
            else temp -= 365;	 //ƽ��

            temp1++;
        }

        calendar.w_year = temp1; //�õ����
        temp1 = 0;

        while(temp >= 28) //������һ����
        {
            if(Is_Leap_Year(calendar.w_year) && temp1 == 1) //�����ǲ�������/2�·�
            {
                if(temp >= 29)temp -= 29; //�����������
                else break;
            }
            else
            {
                if(temp >= mon_table[temp1])temp -= mon_table[temp1]; //ƽ��
                else break;
            }

            temp1++;
        }

        calendar.w_month = temp1 + 1;	//�õ��·�
        calendar.w_date = temp + 1;  	//�õ�����
    }

    temp = timecount % 86400;     		//�õ�������
    calendar.hour = temp / 3600;     	//Сʱ
    calendar.min = (temp % 3600) / 60; 	//����
    calendar.sec = (temp % 3600) % 60; 	//����
    calendar.week = RTC_Get_Week(calendar.w_year, calendar.w_month, calendar.w_date); //��ȡ����
    return 0;
}

//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//�������������������
//����ֵ�����ں�
uint8_t RTC_Get_Week(uint16_t year, uint8_t month, uint8_t day)
{
    uint16_t temp2;
    uint8_t yearH, yearL;

    yearH = year / 100;
    yearL = year % 100;

    // ���Ϊ21����,�������100
    if(yearH > 19)yearL += 100;

    // ����������ֻ��1900��֮���
    temp2 = yearL + yearL / 4;
    temp2 = temp2 % 7;
    temp2 = temp2 + day + table_week[month - 1];

    if(yearL % 4 == 0 && month < 3)temp2--;

    return(temp2 % 7);
}

void RTC_IRQHandler(void)
{
    if(rtc_flag_get(RTC_FLAG_SECOND) != RESET) //�����ж�
    {
        RTC_Get();//����ʱ��
    }

    if(rtc_flag_get(RTC_FLAG_ALARM) != RESET) //�����ж�
    {
        rtc_flag_clear(RTC_FLAG_ALARM);		//�������ж�
    }

    rtc_flag_clear(RTC_FLAG_SECOND | RTC_FLAG_OVERFLOW);		//�������ж�
    rtc_lwoff_wait();
}

volatile struct Calendar * getter_pCalendar(void)
{
    return pCalendar;
}






