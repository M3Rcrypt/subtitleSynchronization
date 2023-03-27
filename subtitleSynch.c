#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 256
struct time {
  char hh[3];
  char mm[3];
  char sec[7];
};

int convertToMs(struct time *t1) {
  int hhFirst, hhSecond, mmFirst, mmSecond;
  int hour, minute, second, ms;
  int msTotal;
  hhFirst = (t1->hh[0] - '0') * 10;
  hhSecond = t1->hh[1] - '0';
  hour = hhFirst + hhSecond;
  mmFirst = (t1->mm[0] - '0') * 10;
  mmSecond = t1->mm[1] - '0';
  minute = mmFirst + mmSecond;
  second = ((t1->sec[0] - '0') * 10) + (t1->sec[1] - '0');
  ms = ((t1->sec[3] - '0') * 100) + ((t1->sec[4] - '0') * 10) +
       (t1->sec[5] - '0');
  msTotal = (hour * 3600000) + (minute * 60000) + (second * 1000) + ms;
  return msTotal;
}

struct time *convertTohhmmss(int msTotal) {
  struct time t1;
  struct time *time_ptr = &t1;
  char hh[3], mm[3], ss[7];
  int hour, minute, second, ms;
  hour = msTotal / 3600000;
  msTotal -= hour * 3600000;
  hh[0] = (hour / 10) + '0';
  hh[1] = (hour % 10) + '0';
  strcpy(time_ptr->hh, hh);

  minute = msTotal / 60000;
  msTotal -= minute * 60000;
  mm[0] = (minute / 10) + '0';
  mm[1] = (minute % 10) + '0';
  strcpy(time_ptr->mm, mm);

  ss[0] = (msTotal / 10000) + '0';
  ss[1] = ((msTotal / 1000) % 10) + '0';
  ss[2] = ',';
  ss[3] = ((msTotal / 100) % 10) + '0';
  ss[4] = ((msTotal / 10) % 10) + '0';
  ss[5] = (msTotal % 10) + '0';
  strcpy(time_ptr->sec, ss);
  return time_ptr;
}

void arrangeSrt(char filename[BUFFER_SIZE], int ms) {
  struct time t1;
  struct time *time_ptr = &t1;
  char hh[3], mm[3], ss[7];
  char buffer[BUFFER_SIZE];
  struct time t2;
  struct time *time_ptr2 = &t2;
  char hh1[3], mm1[3], ss1[7];

  FILE *wp;
  FILE *fp;

  wp = fopen("new.srt", "w");

  // Open the file in read mode
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error opening file");
  }
  int current_line = 1;
  // Read each line of the file
  while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {

    // If this is the line we want to modify
    if (strstr(buffer, "-->") != NULL) {
      // Replace the line with the modified string
      hh[0] = buffer[0];
      hh[1] = buffer[1];
      strcpy(time_ptr->hh, hh);
      mm[0] = buffer[3];
      mm[1] = buffer[4];
      strcpy(time_ptr->mm, mm);
      ss[0] = buffer[6];
      ss[1] = buffer[7];
      ss[2] = buffer[8];
      ss[3] = buffer[9];
      ss[4] = buffer[10];
      ss[5] = buffer[11];
      strcpy(time_ptr->sec, ss);

      int totMS = convertToMs(time_ptr);
      totMS += ms;

      time_ptr = convertTohhmmss(totMS);
      buffer[0] = time_ptr->hh[0];
      buffer[1] = time_ptr->hh[1];
      buffer[3] = time_ptr->mm[0];
      buffer[4] = time_ptr->mm[1];
      buffer[6] = time_ptr->sec[0];
      buffer[7] = time_ptr->sec[1];
      buffer[8] = time_ptr->sec[2];
      buffer[9] = time_ptr->sec[3];
      buffer[10] = time_ptr->sec[4];
      buffer[11] = time_ptr->sec[5];

      hh1[0] = buffer[17];
      hh1[1] = buffer[18];
      strcpy(time_ptr2->hh, hh1);
      mm1[0] = buffer[20];
      mm1[1] = buffer[21];
      strcpy(time_ptr2->mm, mm1);
      ss1[0] = buffer[23];
      ss1[1] = buffer[24];
      ss1[2] = buffer[25];
      ss1[3] = buffer[26];
      ss1[4] = buffer[27];
      ss1[5] = buffer[28];
      strcpy(time_ptr2->sec, ss1);

      int totMS2 = convertToMs(time_ptr2);
      totMS2 += ms;

      time_ptr2 = convertTohhmmss(totMS2);

      buffer[17] = time_ptr2->hh[0];
      buffer[18] = time_ptr2->hh[1];
      buffer[20] = time_ptr2->mm[0];
      buffer[21] = time_ptr2->mm[1];
      buffer[23] = time_ptr2->sec[0];
      buffer[24] = time_ptr2->sec[1];
      buffer[25] = time_ptr2->sec[2];
      buffer[26] = time_ptr2->sec[3];
      buffer[27] = time_ptr2->sec[4];
      buffer[28] = time_ptr2->sec[5];

      // Write the line to the output file
      fputs(buffer, wp);
    } else {
      fputs(buffer, wp);
    }

    current_line++;
  }
  printf("%d", current_line);
  // Close the file
  fclose(fp);
  fclose(wp);
}

int main(int argc, char *argv[]) {
  char *filename = "subtitles.srt";
  int ms = 2000;
  arrangeSrt(filename, ms);
  return 0;
}