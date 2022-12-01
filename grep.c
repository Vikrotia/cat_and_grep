int func_flag_o(regex_t *regex_str, int *flags, int numbers_of_files, int d,
                int number_of_str, int *numbers_of_sovp, int *flag, int *flag1,
                int *flag2, regmatch_t match, size_t *offset, char *argv[],
                size_t *plus, char *str);
int output(int *flags, int numbers_of_files, int numbers_of_sovp, char *argv[],
           int d, int flag_error);
int func_of_flag_e(int *flag_povtoreniya, int *flags, int numbers_of_files,
                   char *argv[], int number_of_str, int *numbers_of_sovp, int d,
                   char *str);
int func_of_flag_v(int *flag_povtoreniya, int *flags, int numbers_of_files,
                   char *argv[], int number_of_str, int *numbers_of_sovp, int d,
                   char *str, int number_of_templates);
int eprst(int rez, int *number_of_templates, char *optarg,
          char (*temptates)[1000], int *flags);
int comp_paterns(regex_t *regex_str, int *flags, char *template);
int regex_read_file(char temptates[][strLength], int *number_of_templates,
                    char optarg[]);
int file_check(FILE *file, char *argv[], int i, int *flags);
int func_flag_e(int *flags, regex_t regex_str, char *str);
int func_flag_v(int *flags, regex_t regex_str, char *str);
int menu(int argc, char *argv[]);
void application_of_flags(int files, int argc, char *argv[], int flags[10],
                          int number_of_templates, int numbers_of_files,
                          char temptates[][strLength]);

int main(int argc, char *argv[]) {
  menu(argc, argv);
  return 0;
}

int menu(int argc, char *argv[]) {
  int flags[10] = {0};                  // массив флагов
  int number_of_templates = 0;          // количество шаблонов
  char temptates[10][strLength] = {0};  // шаблоны
  int template_without_e = 0;  // флаг проверки шаблонов без e
  int files = 0;               // индекс первого argc файла
  int numbers_of_files = 0;  // флаг проверки множественности файлов
  int res = 0;
  const char *short_options = "e:ivclnhsf:o";
  const struct option long_options[] = {{"regexp", 1, NULL, 'e'},
                                        {"ignore-case", 2, NULL, 'i'},
                                        {"invert-match", 2, NULL, 'v'},
                                        {"count", 2, NULL, 'c'},
                                        {"files-with-matches", 2, NULL, 'l'},
                                        {"line-number", 2, NULL, 'n'},
                                        {"no-filename", 2, NULL, 'h'},
                                        {"no-messages", 2, NULL, 's'},
                                        {"file", 1, NULL, 'f'},
                                        {"only-matching", 2, NULL, 'o'},
                                        {NULL, 0, NULL, 0}};
  int rez = 1;
  int option_index = -1;
  if (argc < 3) {
    fprintf(stderr, "Bye\n");
    res = 1;
  } else {
    while ((rez = getopt_long(argc, argv, short_options, long_options,
                              &option_index)) != -1) {
      eprst(rez, &number_of_templates, optarg, temptates, flags);
      option_index = -1;
    }
    if (strcmp(argv[optind], argv[1]) == 0) {
      template_without_e = 1;
      strcpy(temptates[number_of_templates], argv[optind]);
      number_of_templates++;
      flags[0] = 1;
    }
    if (!flags[0] && !flags[8] && !template_without_e) {
      strcpy(temptates[number_of_templates], argv[optind]);
      number_of_templates++;
    }
    if ((flags[0] || flags[8]) && !template_without_e) {
      files = optind;
    } else {
      files = optind + 1;
    }
    if (files != argc - 1) {
      numbers_of_files = 1;
    }
  }
  application_of_flags(files, argc, argv, flags, number_of_templates,
                       numbers_of_files, temptates);
  return res;
}

int eprst(int rez, int *number_of_templates, char *optarg,
          char (*temptates)[1000], int *flags) {
  switch (rez) {
    case 'e':
      flags[0] = 1;
      strcpy(temptates[*number_of_templates], optarg);
      (*number_of_templates)++;
      break;
    case 'i':
      flags[1] = 1;
      break;
    case 'v':
      flags[2] = 1;
      break;
    case 'c':
      flags[3] = 1;
      break;
    case 'l':
      flags[4] = 1;
      break;
    case 'n':
      flags[5] = 1;
      break;
    case 'h':
      flags[6] = 1;
      break;
    case 's':
      flags[7] = 1;
      break;
    case 'f':
      flags[8] = 1;
      regex_read_file(temptates, number_of_templates, optarg);
      break;
    case 'o':
      flags[9] = 1;
      break;
    default:
      break;
  }
  return 0;
}
void application_of_flags(int files, int argc, char *argv[], int flags[10],
                          int number_of_templates, int numbers_of_files,
                          char temptates[][strLength]) {
  int number_of_str = 1;  // количество строк файла
  int numbers_of_sovp = 0;  // количество совпадений для флага с
  int flag_povtoreniya = 0;
  FILE *file = NULL;
  regex_t regex_str;
  regmatch_t match;
  size_t offset = 0;
  int flag_error = 0;
  for (int d = files; d < argc; d++) {
    file = fopen(argv[d], "r");
    if (file_check(file, argv, d, flags)) {
      char str[strLength] = {0};
      while (fgets(str, strLength, file)) {
        int flag = 1, flag1 = 1, flag2 = 1;
        for (long unsigned int i = 0; i < strlen(str); i++) {
          if (str[i] == '\n') {
            str[i] = '\0';
            break;
          }
        }
        for (int k = 0; k < number_of_templates; k++) {
          if (!comp_paterns(&regex_str, flags, temptates[k])) {
            if (func_flag_e(flags, regex_str, str)) {
              func_of_flag_e(&flag_povtoreniya, flags, numbers_of_files, argv,
                             number_of_str, &numbers_of_sovp, d, str);
            }
            if (func_flag_v(flags, regex_str, str)) {
              func_of_flag_v(&flag_povtoreniya, flags, numbers_of_files, argv,
                             number_of_str, &numbers_of_sovp, d, str,
                             number_of_templates);
            }
            if (flags[9] && !flags[2]) {
              size_t plus = offset;
              func_flag_o(&regex_str, flags, numbers_of_files, d, number_of_str,
                          &numbers_of_sovp, &flag, &flag1, &flag2, match,
                          &offset, argv, &plus, str);
            }
          } else {
            fprintf(stderr,
                    "grep:Incorrect compilation of regular expression %s\n",
                    temptates[k]);
            flag_error = 1;
          }
          regfree(&regex_str);
        }
        flag_povtoreniya = 0;
        number_of_str++;
        offset = 0;
      }
      output(flags, numbers_of_files, numbers_of_sovp, argv, d, flag_error);
      numbers_of_sovp = 0;
      number_of_str = 1;
      fclose(file);
    }
  }
}

int file_check(FILE *file, char *argv[], int i, int *flags) {
  int res = 1;
  if (file == NULL) {
    if (!flags[7]) {
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[i]);
    }
    res = 0;
  }
  return res;
}
int comp_paterns(regex_t *regex_str, int *flags, char *template) {
  int res = 0;
  int flag_comp = REG_EXTENDED | REG_NEWLINE;
  if (flags[1]) {
    flag_comp |= REG_ICASE;
  }
  if (regex_str != NULL) {
    res = regcomp(regex_str, template, flag_comp);
  } else {
    regfree(regex_str);
    free(regex_str);
  }
  return res;
}

int regex_read_file(char temptates[][1000], int *number_of_templates,
                    char optarg[]) {
  FILE *file;
  file = fopen(optarg, "r");
  if (file != NULL) {
    char str[1000] = {0};
    while (fgets(str, 1000, file)) {
      char str1[1000] = {0};
      int k = 0;
      for (long unsigned int i = 0; i < strlen(str); i++) {
        if ((str[i] != '\n' && str[i] != '\0') ||
            (i == 0 && (str[i] == '\n' || str[i] == '\0'))) {
          str1[k] = str[i];
          k++;
        }
      }
      strcpy(temptates[*number_of_templates], str1);
      (*number_of_templates)++;
    }
    fclose(file);
  } else {
    fprintf(stderr, "grep: %s: No such file or directory\n", optarg);
  }
  return 0;
}
int func_flag_e(int *flags, regex_t regex_str, char *str) {
  int res = 0;
  if (!flags[2] && !flags[9]) {
    if (!(regexec(&regex_str, str, 0, NULL, 0))) {
      res = 1;
    }
  }
  return res;
}
int func_flag_v(int *flags, regex_t regex_str, char *str) {
  int res = 0;
  if (flags[2]) {
    if ((regexec(&regex_str, str, 0, NULL, 0))) {
      res = 1;
    }
  }
  return res;
}

int output(int *flags, int numbers_of_files, int numbers_of_sovp, char *argv[],
           int d, int flag_error) {
  if (flags[3] && !flags[4] && !flag_error) {
    if (numbers_of_files && !flags[6]) {
      printf("%s:", argv[d]);
    }
    printf("%d\n", numbers_of_sovp);
  }
  if (flags[3] && flags[4] && !flag_error) {
    if (numbers_of_sovp > 0) {
      if (numbers_of_files && !flags[6]) {
        printf("%s:1\n%s\n", argv[d], argv[d]);
      } else {
        printf("1\n%s\n", argv[d]);
      }
    } else {
      if (numbers_of_files && !flags[6]) {
        printf("%s:0\n", argv[d]);
      } else {
        printf("0\n");
      }
    }
  }
  if (!flags[3] && flags[4] && !flag_error) {
    if (numbers_of_sovp > 0) {
      printf("%s\n", argv[d]);
    }
  }
  return 0;
}

int func_flag_o(regex_t *regex_str, int *flags, int numbers_of_files, int d,
                int number_of_str, int *numbers_of_sovp, int *flag, int *flag1,
                int *flag2, regmatch_t match, size_t *offset, char *argv[],
                size_t *plus, char *str) {
  while ((regexec(regex_str, str + (*plus), 1, &match, 0)) == 0) {
    if (!flags[3] && !flags[4]) {
      if (numbers_of_files && !flags[6]) {
        if (*flag) {
          printf("%s:", argv[d]);
          flag = 0;
        }
      }
      if (flags[5]) {
        if (*flag1) {
          printf("%d:", number_of_str);
          *flag1 = 0;
        }
      }
      for (size_t i = (*plus) + match.rm_so; i < (*plus) + match.rm_eo; i++) {
        printf("%c", str[i]);
      }
      printf("\n");
    } else {
      if (*flag2) {
        (*numbers_of_sovp)++;
        *flag2 = 0;
      }
    }
    (*plus) = (*offset) + match.rm_eo;
    (*offset) = (*offset) + match.rm_eo;
  }
  return 0;
}

int func_of_flag_e(int *flag_povtoreniya, int *flags, int numbers_of_files,
                   char *argv[], int number_of_str, int *numbers_of_sovp, int d,
                   char *str) {
  if (!(*flag_povtoreniya)) {
    if (!flags[3] && !flags[4]) {
      if (numbers_of_files && !flags[6]) {
        printf("%s:", argv[d]);
      }
      if (flags[5]) {
        printf("%d:", number_of_str);
      }
      printf("%s\n", str);
    } else {
      (*numbers_of_sovp)++;
    }
  }
  *flag_povtoreniya = 1;
  return 0;
}

int func_of_flag_v(int *flag_povtoreniya, int *flags, int numbers_of_files,
                   char *argv[], int number_of_str, int *numbers_of_sovp, int d,
                   char *str, int number_of_templates) {
  (*flag_povtoreniya)++;
  if ((*flag_povtoreniya) == number_of_templates) {
    if (!flags[3] && !flags[4]) {
      if (numbers_of_files && !flags[6]) {
        printf("%s:", argv[d]);
      }
      if (flags[5]) {
        printf("%d:", number_of_str);
      }
      printf("%s\n", str);
    } else {
      (*numbers_of_sovp)++;
    }
  }
  return 0;
}
