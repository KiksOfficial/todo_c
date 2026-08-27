#include <stdio.h>
#include <string.h>

#define FILENAME "todo.txt"

int add_task(char *filename, char *task);
int init_file();
int print_contents();
int handle_user_input(char *action, char *object);
int delete_task(char *to_be_deleted);

int main(int argc, char *argv[]) {

  init_file();
  handle_user_input(argv[1], argv[2]);
  print_contents();

  return 0;
}

int add_task(char *filename, char *task) {
  FILE *fptr = fopen(filename, "a");
  if (fptr == NULL) {
    return 1;
  }

  fputs(task, fptr);
  fputs("\n", fptr);
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
  }

  while (fgets(line_buffer, sizeof(line_buffer), fptr2)) {
    printf("%s", line_buffer);
  }

  fclose(fptr2);

  return 0;
}

int handle_user_input(char *action, char *object) {
  switch (action[0]) {
  case 'a':
    add_task(FILENAME, object);
    break;
  case 'p':
    print_contents();
    break;
  case 'r':
    delete_task(object);
    break;
  default:
    printf("No such option\n");
  }
  return 0;
}

int delete_task(char *to_be_deleted) {

  char line_buffer[1024];
  FILE *fptr = fopen(FILENAME, "r");
  FILE *tmp = fopen("tmp", "w");

  if (fptr == NULL || tmp == NULL) {
    return 1;
  }

  while (fgets(line_buffer, sizeof(line_buffer), fptr) != NULL) {

    line_buffer[strcspn(line_buffer, "\n")] = '\0';
    if (strcmp(line_buffer, to_be_deleted)) {
      fputs(line_buffer, tmp);
      fputs("\n", tmp);
    }
  }

  fclose(fptr);
  fclose(tmp);

  remove(FILENAME);

  rename("tmp", FILENAME);

  return 0;
}
