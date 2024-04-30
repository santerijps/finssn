#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


errno_t main(int argc, char **argv) {

  errno_t error = 0;
  time_t seconds = time(NULL);
  struct tm *current_time = localtime(&seconds);

  FILE *out_file = NULL;
  char *out_file_path = "out.txt";
  unsigned short from_year = 1900, to_year = current_time->tm_year + 1900 + 1;
  unsigned char quiet = 0, write_to_stdout = 0;

  for (int i = 1; i < argc; i++) {

    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
      fprintf(
        stderr,
        "Usage: finssn-gen [options]\n"
        "\n"
        "Efficiently generates every valid Finnish social security number.\n"
        "Valid SSN's are generated for birth dates between the years 1800 and 2099 (inclusive).\n"
        "\n"
        "Options:\n"
        "  -h, --help\n"
        "  -q, --quiet        (Don't log messages)\n"
        "  -s, --stdout       (Print SSNs to stdout)\n"
        "  -o, --out-file     {output file path (default = \"out.txt\")}\n"
        "  --from             {inclusive year (default = 1900)}\n"
        "  --to               {exclusive year (default = current year + 1)}\n"
        "\n"
      );
      goto CLEAN_UP;
    }

    else if (!strcmp(argv[i], "-q") || !strcmp(argv[i], "--quiet")) {
      quiet = 1;
    }

    else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--stdout")) {
      write_to_stdout = 1;
    }

    else if ((!strcmp(argv[i], "-o") || !strcmp(argv[i], "--out-file")) && (i + 1 < argc)) {
      out_file_path = argv[++i];
    }

    else if (!strcmp(argv[i], "--from") && (i + 1 < argc)) {
      from_year = atoi(argv[++i]);
    }

    else if (!strcmp(argv[i], "--to") && (i + 1 < argc)) {
      to_year = atoi(argv[++i]);
    }

  }

  if (from_year < 1800 || from_year > 2099 || to_year < 1800 || to_year > 2099) {
    fprintf(stderr, "The options --from (%d) and --to (%d) must be within 1800 - 2099 (inclusive).", from_year, to_year);
    error = 1;
    goto CLEAN_UP;
  }

  if (from_year >= to_year) {
    fprintf(stderr, "The option --from (%d) must be smaller than --to (%d)", from_year, to_year);
    error = 1;
    goto CLEAN_UP;
  }

  if (!write_to_stdout && (out_file = fopen(out_file_path, "wb")) == NULL) {
    fprintf(stderr, "Failed to open output file (%s) for writing.", out_file_path);
    error = 1;
    goto CLEAN_UP;
  }

  unsigned short year = from_year, individual_number;
  unsigned char month = 1, day = 1, days_in_month;
  unsigned char century_symbol, check_symbol;
  unsigned int output_lines_written = 0;

  do {

    switch (month) {
      case 1: case 3: case 5: case 7:
      case 8: case 10: case 12:
        days_in_month = 31;
        break;
      case 4: case 6:
      case 9: case 11:
        days_in_month = 30;
        break;
      case 2:
        if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
          days_in_month = 29;
        } else {
          days_in_month = 28;
        }
        break;
    }

    for (; day <= days_in_month; day++) {

      char nineteenth_century_symbols[1]    = {'+'};
      char twentieth_century_symbols[6]     = {'-', 'U', 'V', 'W', 'X', 'Y'};
      char twenty_first_century_symbols[6]  = {'A', 'B', 'C', 'D', 'E', 'F'};
      char *century_symbol_array = NULL;
      unsigned char century = year / 100;

      switch (century) {
        case 18:
          century_symbol_array = nineteenth_century_symbols;
          break;
        case 19:
          century_symbol_array = twentieth_century_symbols;
          break;
        default:
          century_symbol_array = twenty_first_century_symbols;
          break;
      }

      for (unsigned char i = 0; i < (century == 18 ? 1 : 6); i++) {
        century_symbol = century_symbol_array[i];

        for (individual_number = 2; individual_number < 900; individual_number++) {
          char buf[10] = {0};
          sprintf(buf, "%02u%02u%02u%03u", day, month, year % 100, individual_number);

          switch ((atoi(buf) % 31)) {
            case  0: check_symbol = '0'; break;
            case  1: check_symbol = '1'; break;
            case  2: check_symbol = '2'; break;
            case  3: check_symbol = '3'; break;
            case  4: check_symbol = '4'; break;
            case  5: check_symbol = '5'; break;
            case  6: check_symbol = '6'; break;
            case  7: check_symbol = '7'; break;
            case  8: check_symbol = '8'; break;
            case  9: check_symbol = '9'; break;
            case 10: check_symbol = 'A'; break;
            case 11: check_symbol = 'B'; break;
            case 12: check_symbol = 'C'; break;
            case 13: check_symbol = 'D'; break;
            case 14: check_symbol = 'E'; break;
            case 15: check_symbol = 'F'; break;
            case 16: check_symbol = 'H'; break;
            case 17: check_symbol = 'J'; break;
            case 18: check_symbol = 'K'; break;
            case 19: check_symbol = 'L'; break;
            case 20: check_symbol = 'M'; break;
            case 21: check_symbol = 'N'; break;
            case 22: check_symbol = 'P'; break;
            case 23: check_symbol = 'R'; break;
            case 24: check_symbol = 'S'; break;
            case 25: check_symbol = 'T'; break;
            case 26: check_symbol = 'U'; break;
            case 27: check_symbol = 'V'; break;
            case 28: check_symbol = 'W'; break;
            case 29: check_symbol = 'X'; break;
            case 30: check_symbol = 'Y'; break;
            default: {
              fprintf(stderr, "Invalid modulo in check symbol. This error should never occur ;)");
              error = 1;
              goto CLEAN_UP;
            }
          }

          int bytes_written = fprintf(
            write_to_stdout ? stdout : out_file,
            "%02u%02u%02u%c%03u%c\n",
            day, month, year % 100,
            century_symbol, individual_number,
            check_symbol
          );

          if (bytes_written != 12) {
            fprintf(stderr, "Failed to write correct amount of SSN bytes to file.");
            error = 1;
            goto CLEAN_UP;
          }

          output_lines_written += 1;

        }
      }
    }

    month += 1;
    day = 1;

    if (month > 12) {
      year += 1;
      month = 1;
    }

  } while (year < to_year);

  if (!quiet) {
    fprintf(stderr, "Wrote %d lines!\n", output_lines_written);
  }

CLEAN_UP:

  if (!write_to_stdout && out_file != NULL && fclose(out_file)) {
    fprintf(stderr, "Failed to close output file (%s).", out_file_path);
    error = 1;
  }

  return error;

}
