#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "todo.txt"

int add_task(char *filename, char *task);
int init_file();
int print_contents();
int handle_user_input(char *action, char *object);
int delete_task(size_t to_be_deleted);
void combine_strings(int argc, char *argv[], char *dest, size_t dest_size);
int change_task_status(char task_id);
int clear_file();

int main(int argc, char *argv[]) {

  char dest[255];

  if (argc < 2) {
    print_contents();
    return 0;
  }

  init_file();
  combine_strings(argc, argv, dest, sizeof(dest));
  handle_user_input(argv[1], dest);
  print_contents();

  return 0;
}

int add_task(char *filename, char *task) {

  size_t id = 1;
  char line[1024];

  FILE *fptr = fopen(filename, "a+");
  if (fptr == NULL) {
    return 1;
  }

  while (fgets(line, sizeof(line), fptr) != NULL) {
    id++;
  }

  fprintf(fptr, "%zu    [ ] %s\n", id, task);

  fclose(fptr);

  return 0;
}

int init_file() {

  FILE *fptr = fopen(FILENAME, "a");

  if (fptr == NULL) {
    printf("Failed to open todo.txt\n");
    return 1;
  }
  fclose(fptr);

  return 0;
}

int print_contents() {
  printf("Contents of todo file\n");

  char line_buffer[1024];

  FILE *fptr2 = fopen(FILENAME, "r");

  if (fptr2 == NULL) {
    printf("Failed to open todo.txt\n");
    return 1;
  }

  while (fgets(line_buffer, sizeof(line_buffer), fptr2)) {
    printf("%s", line_buffer);
  }

  fclose(fptr2);

  return 0;
}

int handle_user_input(char *action, char *object) {
  switch (action[0]) {
  case 'm':
    change_task_status(*object);
    break;
  case 'a':
    add_task(FILENAME, object);
    break;
  case 'p':
    break;
  case 'r':
    delete_task(strtoul(object, NULL, 10));
    break;
  case 'c':
    clear_file();
    break;
  default:
    printf("No such option\n");
  }
  return 0;
}

int delete_task(size_t to_be_deleted) {

  char line_buffer[1024];
  size_t new_id = 1;

  FILE *fptr = fopen(FILENAME, "r");
  FILE *tmp = fopen("tmp", "w");

  if (fptr == NULL || tmp == NULL) {
    return 1;
  }

  while (fgets(line_buffer, sizeof(line_buffer), fptr) != NULL) {

    size_t old_id = strtoul(line_buffer, NULL, 10);

    line_buffer[strcspn(line_buffer, "\n")] = '\0';
    if (old_id != to_be_deleted) {
      fprintf(tmp, "%zu %s\n", new_id, strchr(line_buffer, ' '));
      new_id++;
    }
  }

  fclose(fptr);
  fclose(tmp);

  if (remove(FILENAME) != 0 || rename("tmp", FILENAME) != 0) {
    return 1;
  }

  return 0;
}

void combine_strings(int argc, char *argv[], char *dest, size_t dest_size) {

  dest[0] = '\0';
  for (int i = 2; i < argc; i++) {
    size_t len = strlen(dest);
    size_t remaining = dest_size - len - 1;

    if (i < argc - 1) {
      snprintf(dest + len, remaining + 1, "%s ", argv[i]);
    } else {
      snprintf(dest + len, remaining + 1, "%s", argv[i]);
    }
  }
}

int change_task_status(char task_id) {

  FILE *fptr = fopen(FILENAME, "r");
  FILE *tmp = fopen("tmp", "w");

  if (fptr == NULL || tmp == NULL) {
    return 1;
  }

  char line_buffer[1024];

  while (fgets(line_buffer, sizeof(line_buffer), fptr) != NULL) {
    char id = line_buffer[0];

    line_buffer[strcspn(line_buffer, "\n")] = '\0';
    if (id == task_id) {
      char *brackets = strstr(line_buffer, "[ ]");

      if (brackets != NULL) {
        brackets[1] = 'X';
      } else {
        brackets = strstr(line_buffer, "[X]");
        if (brackets == NULL) {
          brackets = strstr(line_buffer, "[x]");
        }

        if (brackets != NULL) {
          brackets[1] = ' ';
        }
      }
    }

    fprintf(tmp, "%s\n", line_buffer);
  }

  fclose(tmp);
  fclose(fptr);

  remove(FILENAME);

  rename("tmp", FILENAME);

  return 0;
}

int clear_file() {
  const char *fptr = "todo.txt";
  if (remove(fptr) != 0) {
    printf("Error");
    return 1;
  };
  init_file();
  return 0;
}
