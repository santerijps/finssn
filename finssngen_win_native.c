#ifndef UNICODE
  #define UNICODE
#endif

#include <windows.h>

inline int string_to_int(const char *s) {
  int i = strlen(s) - 1;
  int j = 1;
  int sum = 0;
  while (i >= 0) {
    sum += (s[i] - 48) * j;
    j *= 10;
    i -= 1;
  }
  return sum;
}

size_t wcstombs(char *_Dest, const wchar_t *_Source, size_t _MaxCount);

void _start(void) {

  HANDLE STDERR = GetStdHandle(STD_ERROR_HANDLE);
  SYSTEMTIME current_time;
  GetLocalTime(&current_time);

  char *out_file_path = "out.txt";
  unsigned short from_year = current_time.wYear, to_year = current_time.wYear + 1;
  unsigned char quiet = 0;

  // TODO:
  // for (int i = 1; i < argc; i++) {
  //   if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
  //     const char s[] =
  //       "Usage: finssngen [options]\n"
  //       "\n"
  //       "Efficiently generates every valid Finnish social security number.\n"
  //       "\n"
  //       "Options:\n"
  //       "  -h, --help\n"
  //       "  -q, --quiet\n"
  //       "  -o, --out-file     {output file path (default = \"out.txt\")}\n"
  //       "  --from             {inclusive year (default = 1900)}\n"
  //       "  --to               {exclusive year (default = current year + 1)}\n"
  //       "\n";
  //     WriteFile(STDERR, s, sizeof(s), NULL, NULL);
  //     ExitProcess(0);
  //   }
  //   else if (!strcmp(argv[i], "-q") || !strcmp(argv[i], "--quiet")) {
  //     quiet = 1;
  //   }
  //   else if ((!strcmp(argv[i], "-o") || !strcmp(argv[i], "--out-file")) && (i + 1 < argc)) {
  //     out_file_path = argv[++i];
  //   }
  //   else if (!strcmp(argv[i], "--from") && (i + 1 < argc)) {
  //     from_year = atoi(argv[++i]);
  //   }
  //   else if (!strcmp(argv[i], "--to") && (i + 1 < argc)) {
  //     to_year = atoi(argv[++i]);
  //   }
  // }

  OFSTRUCT ofs = {0};
  HFILE out_file = OpenFile(out_file_path, &ofs, OF_CREATE | OF_WRITE);
  if (out_file == HFILE_ERROR) {
    const char s[] = "Failed to open output file for writing!\n";
    WriteFile(STDERR, s, sizeof(s), NULL, NULL);
    ExitProcess(1);
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

      switch (year / 100) {
        case 18:
          century_symbol = '+';
          break;
        case 19:
          century_symbol = '-';
          break;
        default:
          century_symbol = 'A';
          break;
      }

      for (individual_number = 2; individual_number < 900; individual_number++) {

        WCHAR buf_9digits[10] = {0};
        {
          INT opt = FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY;
          LPWSTR fmt = L"%1!02u!%2!02u!%3!2u!%4!03u!";
          DWORD_PTR args[] = { (DWORD_PTR)day, (DWORD_PTR)month, (DWORD_PTR)year % 100, (DWORD_PTR)individual_number };
          DWORD size = 10;
          if (!FormatMessage(opt, fmt, 0, 0, buf_9digits, size, (va_list*)args)) {
            const char s[] = "Format message failed (9-digit number)!\n";
            WriteFile(STDERR, s, sizeof(s), NULL, NULL);
            ExitProcess(1);
          }
        }

        CHAR buf_9digits_as_cstring[20] = {0};
        wcstombs(buf_9digits_as_cstring, buf_9digits, 20);

        switch ((string_to_int(buf_9digits_as_cstring) % 31)) {
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
            const char s[] = "Invalid modulo in check symbol!\n";
            WriteFile(STDERR, s, sizeof(s), NULL, NULL);
            ExitProcess(1);
          }
        }

        WCHAR buf_ssn[14] = {0};
        {
          INT opt = FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY;
          LPWSTR fmt = L"%1!02u!%2!02u!%3!2u!%4!c!%5!03u!%6!c!\n";
          DWORD_PTR args[] = {
            (DWORD_PTR)day,
            (DWORD_PTR)month,
            (DWORD_PTR)year % 100,
            (DWORD_PTR)century_symbol,
            (DWORD_PTR)individual_number,
            (DWORD_PTR)check_symbol,
          };
          DWORD size = 14;
          if (!FormatMessage(opt, fmt, 0, 0, buf_ssn, size, (va_list*)args)) {
            const char s[] = "Format message failed (ssn)!\n";
            WriteFile(STDERR, s, sizeof(s), NULL, NULL);
            ExitProcess(1);
          }
        }

        WriteFile((HANDLE)out_file, buf_ssn, wcslen(buf_ssn) * sizeof(WCHAR), NULL, NULL);
        output_lines_written += 1;

      }
    }

    month += 1;
    day = 1;

    if (month > 12) {
      year += 1;
      month = 1;
    }

  } while (year < to_year);

  if (CloseHandle((HANDLE)out_file) == ERROR) {
    const char s[] = "Failed to close output file!\n";
    WriteFile(STDERR, s, sizeof(s), NULL, NULL);
    ExitProcess(1);
  }

  if (!quiet) {
    INT opt = FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY;
    LPWSTR fmt = L"Wrote %1!d! lines!\n";
    DWORD_PTR args[] = { (DWORD_PTR)output_lines_written };
    DWORD size = 64;
    WCHAR buf[64] = {0};
    if (!FormatMessage(opt, fmt, 0, 0, buf, size, (va_list*)args)) {
      const char s[] = "Format message failed (output_lines_written)!\n";
      WriteFile(STDERR, s, sizeof(s), NULL, NULL);
      ExitProcess(1);
    }
    WriteFile(STDERR, buf, sizeof(buf), NULL, NULL);
  }

  ExitProcess(0);
}
