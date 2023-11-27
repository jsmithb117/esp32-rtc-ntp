int dayOfWeek(int year, int month, int day) {
  int y = year;
  byte m = month;
  byte d = day;

  if (m < 3) {
    y--;
    m += 12;
  }

  return (y + y / 4 - y / 100 + y / 400 + (13 * m + 8) / 5 + d) % 7;
}

bool isDST(int year, int month, int day, int hour) {
  if (month < 3 || month > 11) return false; // not DST in Jan, Feb, Dec
  if (month > 3 && month < 11) return true; // DST in Apr, May, Jun, Jul, Aug, Sep, Oct
  int previousSunday = day - (dayOfWeek(year, month, day) - 1);
  // in March, we are DST if our previous Sunday was on or after the 8th
  if (month == 3) return previousSunday >= 8;
  // in November we must be before the first Sunday to be DST
  // that means the previous Sunday must be before the 1st
  return previousSunday <= 0;
}