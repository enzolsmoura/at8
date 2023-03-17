/*
 * PREENCHA O NOME E RA DA DUPLA!!!!!!
 *
 * Nome: Enzo Leonardo Sabatelli de Moura
 * RA: 21.01535-0
 *
 * Nome: 
 * RA: 
 *
 * ENTREGUE APENAS O ARQUIVO MAIN.C
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "../include/sqlite3.h"
#include <string.h>
#define MAX_CHAR 11

void precipitacaoMed(sqlite3 *db, sqlite3_stmt *stmt, char dataInicio[MAX_CHAR], char dataFim[MAX_CHAR]);
void tempMax(sqlite3 *db, sqlite3_stmt *stmt, char dataInicio[MAX_CHAR], char dataFim[MAX_CHAR]);
void tempMin(sqlite3 *db, sqlite3_stmt *stmt, char dataInicio[MAX_CHAR], char dataFim[MAX_CHAR]);
void tempMed(sqlite3 *db, sqlite3_stmt *stmt, char dataInicio[MAX_CHAR], char dataFim[MAX_CHAR]);
void lerString(char s[MAX_CHAR]);
void formatarData(char s[MAX_CHAR]);

int main() {
  sqlite3 *db = NULL;
  sqlite3_stmt *stmt = NULL;
  int conexao, n;
  char dataInicio[MAX_CHAR], dataFim[MAX_CHAR];
  system("clear");
  printf("Criando conexão com o banco: \n");
  conexao = sqlite3_open("Tempo.db", &db);
  if (conexao != SQLITE_OK) {
    printf("Erro ao conectar ao banco.\n");
    exit(-1);
  }
  printf("Sucesso na conexão!\n");
  
  printf(
      "Informe a opção desejada:\n1 - Consultar a precipitacao media no "
      "periodo\n2 - Consultar os dias com a temperatura maxima no periodo\n3 "
      "- Consultar os dias com a temperatura minima no periodo\n4 - Consultar "
      "a temperatura media no periodo\n0 - Sair do sistema\n");

  scanf("%i", &n);
  while(n!=0){
    switch (n) {
    default:
      printf("Opção não reconhecida. Por favor, tente outra opção. \n");
      break;
    case 1:
      printf("Período inicial: ");
      lerString(dataInicio);
      printf("Período final: ");
      lerString(dataFim);
      formatarData(dataInicio);
      formatarData(dataFim);
      precipitacaoMed(db, stmt, dataInicio, dataFim);
      break;
    case 2:
      printf("Período inicial: ");
      lerString(dataInicio);
      printf("Período final: ");
      lerString(dataFim);
      formatarData(dataInicio);
      formatarData(dataFim);
      tempMax(db, stmt, dataInicio, dataFim);
      break;
    case 3:
      printf("Período inicial: ");
      lerString(dataInicio);
      printf("Período final: ");
      lerString(dataFim);
      formatarData(dataInicio);
      formatarData(dataFim);
      tempMin(db, stmt, dataInicio, dataFim);
      break;
    case 4:
      printf("Período inicial: ");
      lerString(dataInicio);
      printf("Período final: ");
      lerString(dataFim);
      formatarData(dataInicio);
      formatarData(dataFim);
      tempMed(db, stmt, dataInicio, dataFim);
      break;
    } 
    printf("Informe a opcao desejada: \n");
    scanf("%i",&n);
  }
  printf("Saindo do programa... \n");
  sqlite3_finalize(stmt);
  sqlite3_close(db);
  return 0;
}

void lerString(char s[MAX_CHAR]) {
  scanf("\n");
  fgets(s, MAX_CHAR, stdin);
  if (s[strlen(s)-1] == '\n') {
    s[strlen(s)-1] = '\0';
  }
}

void formatarData(char s[MAX_CHAR]) {
  char data[MAX_CHAR], dia[3], mes[3], ano[5];
  char *token = strtok(s, ".");
  sprintf(dia, "%s", token);
  token = strtok(NULL, ".");
  sprintf(mes, "%s", token);
  token = strtok(NULL, ".");
  sprintf(ano, "%s", token);
  sprintf(data, "%s-%s-%s", ano, mes, dia);
  strcpy(s, data);
}

void precipitacaoMed(sqlite3 *db, sqlite3_stmt *stmt, char dataInicio[MAX_CHAR], char dataFim[MAX_CHAR]) {
  char query[500];
  sprintf(query, "SELECT AVG(precipitacao) FROM Medicoes WHERE data BETWEEN '%s' AND '%s'", dataInicio, dataFim);
  sqlite3_prepare(db, query, -1, &stmt, NULL);
  sqlite3_step(stmt);
  printf("Precipitação Media = %.2f\n", sqlite3_column_double(stmt,0));
  
}

void tempMax(sqlite3 *db, sqlite3_stmt *stmt, char dataInicio[MAX_CHAR], char dataFim[MAX_CHAR]) {
  char query[500];
  sprintf(query,"SELECT data, tmax FROM Medicoes WHERE data BETWEEN '%s' AND '%s' AND tmax = (SELECT max(tmax) FROM Medicoes WHERE data BETWEEN '%s' AND '%s')", dataInicio, dataFim, dataInicio, dataFim);

  sqlite3_prepare(db, query, -1, &stmt, NULL);
  while (sqlite3_step(stmt) != SQLITE_DONE){
    printf("Data = %s\t", sqlite3_column_text(stmt, 0));
    printf("Temperatura Maxima = %.2f",sqlite3_column_double(stmt, 1));    
    printf("\n");
  }
}

void tempMin(sqlite3 *db, sqlite3_stmt *stmt, char dataInicio[MAX_CHAR], char dataFim[MAX_CHAR]) {
  char query[500];
  sprintf(query,"SELECT data, tmin FROM Medicoes WHERE data BETWEEN '%s' AND '%s' AND tmin = (SELECT min(tmin) FROM Medicoes WHERE data BETWEEN '%s' AND '%s')", dataInicio, dataFim, dataInicio, dataFim);

  sqlite3_prepare(db, query, -1, &stmt, NULL);
  sqlite3_prepare(db, query, -1, &stmt, NULL);
  while (sqlite3_step(stmt) != SQLITE_DONE){
    printf("Data = %s\t", sqlite3_column_text(stmt, 0));
    printf("Temperatura Minima = %.2f",sqlite3_column_double(stmt, 1));    
    printf("\n");
  }
}


void tempMed(sqlite3 *db, sqlite3_stmt *stmt, char dataInicio[MAX_CHAR], char dataFim[MAX_CHAR]) {
  char query[500];
  sprintf(query, "SELECT (\"AVG(tmax)\"+\"AVG(tmin)\")/2 FROM (SELECT AVG(tmax),AVG(tmin) FROM Medicoes WHERE data BETWEEN '%s' AND '%s')", dataInicio, dataFim);
  sqlite3_prepare(db, query, -1, &stmt, NULL);
  sqlite3_step(stmt);
  printf("Temperatura Media = %.2f\n", sqlite3_column_double(stmt,0));
}



