#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LCD_PORT PORTB
#define LCD_DDR  DDRB
#define RS PD0
#define EN PD1

#define BTN_HOURS  PC0
#define BTN_MINUTES PC1
#define BTN_DATE  PC2
#define BTN_MONTHS  PC3

volatile int hours = 23, minutes = 59, seconds = 0;
volatile int day = 1, month = 1, year = 2024;

const int days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void lcd_command(unsigned char);
void lcd_data(unsigned char);
void lcd_init();
void lcd_print(char*);
void lcd_gotoxy(unsigned char, unsigned char);
void display_time_date();
void update_date();
void timer1_init();
void check_buttons();
void debounce();

void lcd_command(unsigned char cmd) {
	LCD_PORT = (LCD_PORT & 0x0F) | (cmd & 0xF0);
	PORTD &= ~(1 << RS);
	PORTD |= (1 << EN);
	_delay_ms(1);
	PORTD &= ~(1 << EN);
	_delay_ms(2);
	LCD_PORT = (LCD_PORT & 0x0F) | (cmd << 4);
	PORTD |= (1 << EN);
	_delay_ms(1);
	PORTD &= ~(1 << EN);
	_delay_ms(2);
}

void lcd_data(unsigned char data) {
	LCD_PORT = (LCD_PORT & 0x0F) | (data & 0xF0);
	PORTD |= (1 << RS);
	PORTD |= (1 << EN);
	_delay_ms(1);
	PORTD &= ~(1 << EN);
	_delay_ms(2);

	LCD_PORT = (LCD_PORT & 0x0F) | (data << 4);
	PORTD |= (1 << EN);
	_delay_ms(1);
	PORTD &= ~(1 << EN);
	_delay_ms(2);
}

void lcd_init() {
	LCD_DDR = 0xF0;
	DDRD |= (1 << RS) | (1 << EN);
	_delay_ms(20);

	lcd_command(0x02);
	lcd_command(0x28);
	lcd_command(0x0C);
	lcd_command(0x06);
	lcd_command(0x01);
	_delay_ms(2);
}

void lcd_print(char* str) {
	while (*str) {
		lcd_data(*str++);
	}
}

void lcd_gotoxy(unsigned char x, unsigned char y) {
	if (y == 0) {
		lcd_command(0x80 + x);
		} else {
		lcd_command(0xC0 + x);
	}
}

void display_time_date() {
	lcd_gotoxy(0, 0);
	lcd_print("TIME: ");

	lcd_data((hours / 10) + '0');
	lcd_data((hours % 10) + '0');
	lcd_data(':');
	lcd_data((minutes / 10) + '0');
	lcd_data((minutes % 10) + '0');
	lcd_data(':');
	lcd_data((seconds / 10) + '0');
	lcd_data((seconds % 10) + '0');

	lcd_gotoxy(0, 1);
	lcd_print("DATE: ");

	lcd_data((day / 10) + '0');
	lcd_data((day % 10) + '0');
	lcd_data('/');
	lcd_data((month / 10) + '0');
	lcd_data((month % 10) + '0');
	lcd_data('/');
	lcd_data((year / 1000) + '0');
	lcd_data(((year / 100) % 10) + '0');
	lcd_data(((year / 10) % 10) + '0');
	lcd_data((year % 10) + '0');
}

void timer1_init() {
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS12);
	OCR1A = 62499;
	TIMSK |= (1 << OCIE1A);
	sei();
}

ISR(TIMER1_COMPA_vect) {
	seconds++;
	if (seconds == 60) {
		minutes++;
		if (minutes == 60) {
			hours++;
			if (hours >= 24) {
				hours = 0;
				update_date();
				
				PORTD ^= (1 << PD7);
				_delay_ms(500);
				PORTD &= ~(1 << PD7);
			}
			minutes = 0;
		}
		seconds = 0;
	}
}

void update_date() {
	day++;
	int max_days = days_in_month[month - 1];

	if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
		max_days = 29;
	}

	if (day > max_days) {
		day = 1;
		month++;
		if (month > 12) {
			month = 1;
			year++;
		}
	}

	lcd_gotoxy(0, 0);
	lcd_print("DATE UPDATED");
	_delay_ms(500);
}

void check_buttons() {
	if (!(PINC & (1 << BTN_MINUTES))) {
		_delay_ms(200);
		minutes++;
		if (minutes == 60) {
			hours++;
			if (hours >= 24) {
				hours = 0;
				update_date();
			}
			minutes = 0;
		}
		lcd_gotoxy(0, 0);
		lcd_print("Set Minutes ");
	}
	if (!(PINC & (1 << BTN_HOURS))) {
		_delay_ms(200);
		hours++;
		if (hours >= 24) {
			hours = 0;
			update_date();
		}
		lcd_gotoxy(0, 0);
		lcd_print("Set Hours   ");
	}
	if (!(PINC & (1 << BTN_DATE))) {
		_delay_ms(200);
		update_date();
		lcd_gotoxy(0, 0);
		lcd_print("Set Date    ");
	}
	if (!(PINC & (1 << BTN_MONTHS))) {
		_delay_ms(200);
		month++;
		if (month > 12) {
			month = 1;
			year++;
		}
		lcd_gotoxy(0, 0);
		lcd_print("Set Months   ");
	}
}

void debounce() {
	_delay_ms(50);
	while (!(PINC & (1 << BTN_HOURS)) || !(PINC & (1 << BTN_MINUTES)) || !(PINC & (1 << BTN_DATE)) || !(PINC & (1 << BTN_MONTHS)) ) {
		
	}
	_delay_ms(50);
}

int main() {
	lcd_init();
	timer1_init();
	
	DDRC &= ~((1 << BTN_HOURS) | (1 << BTN_MINUTES) | (1 << BTN_DATE) | (1<<BTN_MONTHS));
	PORTC |= (1 << BTN_HOURS) | (1 << BTN_MINUTES) | (1 << BTN_DATE) | (1<< BTN_MONTHS);

	while (1) {
		display_time_date();
		check_buttons();
		_delay_ms(500);
	}
}
