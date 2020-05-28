#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>    // Needed for sockets stuff
#include <netinet/in.h>   // Needed for sockets stuff
#include <sys/socket.h>   // Needed for sockets stuff
#include <arpa/inet.h>    // Needed for sockets stuff
#include <fcntl.h>        // Needed for sockets stuff
#include <netdb.h>        // Needed for sockets stuff

#define VERBOSE
#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
  int buf_size = -1;
  int port = -1;
  char ip[16] = {'\0'};
  int sockfd, n;
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  while (1) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"buf_size", required_argument, 0, 0},
                                      {"ip", required_argument, 0, 0},
                                      {"port", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        if ((buf_size = atoi(optarg)) == 0) {
          printf("Error: bad buf_size value\n");
          return -1;
        }
        break;
      case 1:
        strcpy(ip, optarg);
        break;
      case 2:
        if ((port = atoi(optarg)) == 0) {
          printf("Error: bad port value\n");
          return -1;
        }
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (buf_size == -1 || port == -1 || strlen(ip) == 0) {
    fprintf(stderr, "Using: %s --buf_size [NUMBER] --port [NUMBER]\n",
            argv[0]);
    return -1;
  }
  char sendline[buf_size], recvline[buf_size + 1];

#ifdef VERBOSE
  printf("buf=%d port=%d ip=%s\n", buf_size, port, ip);
#endif

  // заполняет первые САЙЗ байтов той области памяти, на которую указывает servaddr, постоянным байтом 0.
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  //Функция htons() преобразует узловой порядок расположения байтов положительного короткого целого port в сетевой порядок расположения байтов
  servaddr.sin_port = htons(port);

  //Данная функция преобразует  ip в сетевой адрес (типа af), затем копирует полученную структуру с адресом в servaddr.sin_addr.
  if (inet_pton(AF_INET, ip, &servaddr.sin_addr) < 0) {
    perror("inet_pton problem");
    exit(1);
  }
  // Сокет
  // AF_INET - IPv4 протокол Интернета
  //!!!!!!
  // --!!!SOCK_DGRAM!!! - Поддерживает датаграммы
  // (ненадежные сообщения с ограниченной длиной и не поддерживающие соединения).
  // Что и делает вид транспортировки данных UPD
  //!!!!!
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  int flags = fcntl(sockfd, F_GETFL, 0);
  fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

  // struct timeval read_timeout;
  // read_timeout.tv_sec = 0;
  // read_timeout.tv_usec = 10;
  //setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

  write(1, "Enter string\n", 13);

  //sendto - отправляет сообщения в сокет
  int sendint = 0;
  while (sendint < 50) {
    snprintf(sendline, sizeof(sendline), "%d", sendint);
    printf("Sending to server= %s\n", sendline);
    if (sendto(sockfd, sendline, sizeof(sendline), 0, (SADDR *)&servaddr, SLEN) == -1) {
      perror("sendto problem");
      exit(1);
    }

    // заполняем нулями реквлайн
    // Системные вызовы recvfrom  используется для получения сообщений из сокета, и может использоваться
    // для получения данных, независимо от того, является ли сокет ориентированным на соединения или нет.
    memset(recvline, 0, sizeof(recvline));
    if (recvfrom(sockfd, recvline, buf_size, 0, NULL, NULL) == -1) {
      printf("no respond\n");
    } else {
      printf("REPLY FROM SERVER= %s\n", recvline);
    }
    sleep(1);
    sendint++;
  }
  close(sockfd);
}
