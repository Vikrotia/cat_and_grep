
void menu(int parametr, int *flags);
void func_flag_b(char *str, int flag_b, int *non_empty_str_val);
void func_flag_n(int flag_n, int flag_b, int *str_val);
int func_flag_v(int flag_v, char str);
int func_flag_s(int flag_s, char *str, int *empty_str);
int func_flag_t(int flag_t, char str);
int func_flag_e(int flag_e, char str);
int main_menu(int argc, char *argv[]);
int application_of_flags(int *optind, int argc, char *argv[], int *flags);

int main(int argc, char *argv[]) {
  main_menu(argc, argv);
  return 0;
}

int main_menu(int argc, char *argv[]) {
  int flags[6] = {0};
  const char *short_options = "+benstv";
  int parametr;
  const struct option long_options[] = {
      // длинные параметры
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};
  while (
      (parametr = getopt_long(argc, argv, short_options, long_options, NULL)) !=
      -1) {  // сопостваление параметров флагам
    menu(parametr, flags);  //  подсчет переданных флагов
  }
  application_of_flags(&optind, argc, argv, flags);
  return 0;
}

int application_of_flags(int *optind, int argc, char *argv[], int *flags) {
  FILE *file;
  int empty_str = 0;
  for (; *optind < argc; (*optind)++) {
    file = fopen(argv[*optind], "r");
    if (file == NULL) {
      fprintf(stderr, "No such file or directory\n");
    } else {
      int non_empty_str_val = 0;  // количество непустых строк
      int str_val = 0;            // количество строк
      char str[1000] = {0};
      while (fgets(str, 1000, file)) {  // считывание строки из файла
        if (func_flag_s(flags[2], str, &empty_str)) {
          func_flag_b(str, flags[0], &non_empty_str_val);
          func_flag_n(flags[1], flags[0], &str_val);
          for (long unsigned int j = 0; j < sizeof(str);
               j++) {  // преобразование каждого сивола из строки
            if (func_flag_t(flags[3], str[j]) == 0 &&
                func_flag_e(flags[4], str[j]) == 0 &&
                func_flag_v(flags[5], str[j]) == 0) {
              printf("%c", str[j]);
            }
            if (str[j + 1] == 0 && str[j + 2] == 0) {
              break;
            }
          }
        }
        for (int i = 0; i < 1000; i++) {
          str[i] = 0;
        }
      }
    }
    fclose(file);
  }
  return 0;
}
void func_flag_b(char *str, int flag_b, int *non_empty_str_val) {
  if (flag_b) {
    if (strcmp(str, "\n") != 0) {
      (*non_empty_str_val)++;
      printf("%6.d	", *non_empty_str_val);
    }
  }
}
void func_flag_n(int flag_n, int flag_b, int *str_val) {
  if (flag_n && flag_b == 0) {
    (*str_val)++;
    printf("%6.d	", *str_val);
  }
}
int func_flag_s(int flag_s, char *str, int *empty_str) {
  int res = 1;
  if (flag_s) {
    if (str[0] == '\n') {
      (*empty_str)++;
    } else {
      *empty_str = 0;
    }
    if (*empty_str > 1) {
      res = 0;
    }
  }
  return res;
}
int func_flag_t(int flag_t, char str) {
  int res = 0;
  if (flag_t) {
    if (str == '\t') {
      printf("^I");
      res = 1;
    }
  }
  return res;
}
int func_flag_e(int flag_e, char str) {
  int res = 0;
  if (flag_e) {
    if (str == '\n') {
      printf("$");
      printf("\n");
      res = 1;
    }
  }
  return res;
}
int func_flag_v(int flag_v, char str) {
  int res = 0;
  if (flag_v) {
    if ((str >= 0 && str < 9) || (str > 10 && str < 32) || (str > 126)) {
      printf("^");
      if (str > 126) {
        str -= 64;
        printf("%c", str);
      } else {
        str += 64;
        printf("%c", str);
      }
      res = 1;
    }
  }
  return res;
}
void menu(int parametr, int *flags) {
  switch (parametr) {
    case 'b':
      flags[0] = 1;
      break;
    case 'n':
      flags[1] = 1;
      break;
    case 's':
      flags[2] = 1;
      break;
    case 't':
      flags[3] = 1;
      flags[5] = 1;
      break;
    case 'e':
      flags[4] = 1;
      flags[5] = 1;
      break;
    case 'v':
      flags[5] = 1;

      break;
    default:
      break;
  }
}

