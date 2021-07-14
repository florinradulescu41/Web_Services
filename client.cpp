
#include "helpers.cpp"
#include "json.hpp"

using json = nlohmann::json;

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    char *auth_token = NULL;
    string cookie = "";
    int sockfd;
    bool EXIT = false;
    char buffer[100];

    puts(STARTER);
    puts(WELCOME);

    while(!EXIT){

      fgets(buffer, 100, stdin);

      if(strcmp(buffer, "register\n") == 0) {

        json j;
        char username[100];
        char password[100];

        cout << "Introduceti username: ";
        fgets(username, 100, stdin);

        cout << "Introduceti parola: ";
        fgets(password, 100, stdin);

        j["username"] = username;
        j["password"] = password;

        puts(CONNECT);
        sockfd = open_connection(HOST_IP, PORT, AF_INET, SOCK_STREAM, 0);

        message = compute_post_request(HOST, REGISTER, "", "", j.dump());
        send_to_server(sockfd, message);
        response = receive_from_server(sockfd);
        cout << response << endl;
        puts(FINALMS);
        puts(WELCOME);

        continue;
      }

      if(strcmp(buffer, "login\n") == 0) {

        json j;
        char username[100];
        char password[100];

        cout << "Introduceti username: ";
        fgets(username, 100, stdin);

        cout << "Introduceti parola: ";
        fgets(password, 100, stdin);

        j["username"] = username;
        j["password"] = password;

        puts(CONNECT);
        sockfd = open_connection(HOST_IP, PORT, AF_INET, SOCK_STREAM, 0);

        message = compute_post_request(HOST, LOGIN, "", "", j.dump());
        send_to_server(sockfd, message);
        response = receive_from_server(sockfd);

        if (cookie == "") {
          const char *s1 = "Set-Cookie: ";
          const char *s2 = ";";

          char *start, *end;
          if (start = strstr(response, s1))
          {
              start += strlen(s1);
              if (end = strstr(start, s2))
              {
                  char *aux = NULL;
                  aux = (char *)malloc(end - start + 1);
                  memcpy(aux, start, end - start);
                  aux[end - start] = '\0';
                  cookie = string(aux);
              }
          }

          cookie = cookie + ";";
        }
        cout << response << endl;
        puts(FINALMS);
        puts(WELCOME);

        continue;

      }

      if(strcmp(buffer, "logout\n") == 0) {

        puts(LOGOUTM);
        sockfd = open_connection(HOST_IP, PORT, AF_INET, SOCK_STREAM, 0);

        message = compute_get_request(HOST, LOGOUT, cookie, "");
        send_to_server(sockfd, message);
        response = receive_from_server(sockfd);
        cout << response << endl;
        puts(FINALMS);
        puts(WELCOME);

        continue;

      }

      if(strcmp(buffer, "enter_library\n") == 0) {

        puts(CONNECT);
        sockfd = open_connection(HOST_IP, PORT, AF_INET, SOCK_STREAM, 0);

        message = compute_get_request(HOST, ACCESS, cookie, "");
        send_to_server(sockfd, message);
        response = receive_from_server(sockfd);
        cout << response << endl;
        puts(FINALMS);
        puts(WELCOME);

        continue;

      }

      if(strcmp(buffer,"exit\n") == 0) {
        puts(EXITCLI);
        EXIT = true;
      }

    }

    return 0;
}
