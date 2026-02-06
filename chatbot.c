#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#define MAX_QA 400
#define MAX_LEN 200

void playRPS();

int min(int a, int b, int c) {
    if (a <= b && a <= c) return a;
    if (b <= a && b <= c) return b;
    return c;
}

int distfxn(char *s1, char *s2) {
    int len1 = strlen(s1), len2 = strlen(s2);
    int dp[len1 + 1][len2 + 1];

    for (int i = 0; i <= len1; i++) dp[i][0] = i;
    for (int j = 0; j <= len2; j++) dp[0][j] = j;

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (tolower(s1[i - 1]) == tolower(s2[j - 1])) ? 0 : 1;
            dp[i][j] = min(dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + cost);
        }
    }
    return dp[len1][len2];
}

int loadQA(char questions[MAX_QA][MAX_LEN], char answers[MAX_QA][MAX_LEN]) {
    FILE *file = fopen("chatdata.txt", "r");
    if (!file) {
        printf("Error: Could not open chatdata.txt\n");
        return 0;
    }

    int count = 0;
    while (fgets(questions[count], MAX_LEN, file)) {
        char *sep = strchr(questions[count], '|');
        if (sep) {
            *sep = '\0';
            strcpy(answers[count], sep + 1);
            answers[count][strcspn(answers[count], "\n")] = '\0';
            count++;
        }
    }

    fclose(file);
    return count;
}

void saveNewQA(char *question, char *answer) {
    FILE *file = fopen("chatdata.txt", "a");
    if (!file) {
        printf("Error: Could not open file for saving!\n");
        return;
    }
    fprintf(file, "%s|%s\n", question, answer);
    fclose(file);
}

int containsDigit(char *str) {
    for (int i = 0; str[i]; i++) {
        if (isdigit(str[i])) return 1;
    }
    return 0;
}

int main() {
    char input[MAX_LEN], answer[MAX_LEN];
    char questions[MAX_QA][MAX_LEN], answers[MAX_QA][MAX_LEN];
    int total = loadQA(questions, answers);

    if (total == 0) return 1;

    printf("ChatBot: Hello! I’ve loaded %d responses. Type 'bye' to exit.\n", total);

    while (1) {
        printf("You: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        for (int i = 0; input[i]; i++) input[i] = tolower(input[i]);

        if (strcmp(input, "bye") == 0) {
            printf("ChatBot: Goodbye! 👋\n");
            break;
        }

        if (strcmp(input, "play rps") == 0 || strcmp(input, "play rock paper scissors") == 0) {
            playRPS();
            continue;
        }

        if (strncmp(input, "what is", 7) == 0 && containsDigit(input)) {
            double a, b;
            char op;

            if (sscanf(input + 7, "%lf %c %lf", &a, &op, &b) == 3) {
                double result;
                switch (op) {
                    case '+': result = a + b; break;
                    case '-': result = a - b; break;
                    case '*': result = a * b; break;
                    case '/':
                        if (b == 0) {
                            printf("ChatBot: Cannot divide by zero!\n");
                            continue;
                        }
                        result = a / b;
                        break;
                    default:
                        printf("ChatBot: Unknown operator.\n");
                        continue;
                }
                printf("ChatBot: %.2lf\n", result);
                continue;
            }
        }

        int bestMatch = -1, bestScore = 999;
        for (int i = 0; i < total; i++) {
            int dist = distfxn(input, questions[i]);
            if (dist < bestScore) {
                bestScore = dist;
                bestMatch = i;
            }
        }

          int len = strlen(input);
          int threshold = (len <= 5) ? 1 : (len <= 10) ? 2 : (len <= 20) ? 3 : 4;

           if (bestMatch != -1 && bestScore <= threshold){
            printf("ChatBot: %s\n", answers[bestMatch]);
             } else {
            printf("ChatBot: Sorry, I didn’t get that.\n");
            printf("ChatBot: Can you teach me what to reply to \"%s\"?\n", input);
            printf("You (answer): ");
            fgets(answer, sizeof(answer), stdin);
            answer[strcspn(answer, "\n")] = '\0';

            if (strlen(answer) > 0) {
                saveNewQA(input, answer);
                strcpy(questions[total], input);
                strcpy(answers[total], answer);
                total++;
                printf("ChatBot: Got it! I’ll remember that next time.\n");
            }
        }
    }

    return 0;
}

void playRPS() {
    char user[20];
    const char *choices[] = {"rock", "paper", "scissors"};
    srand(time(NULL));

    int userScore = 0, botScore = 0, draws = 0;

    printf("ChatBot: Let's play Rock-Paper-Scissors!\n");
    printf("ChatBot: Type rock, paper, or scissors (or 'exit' to quit).\n");

    while (1) {
        printf("You: ");
        fgets(user, sizeof(user), stdin);
        user[strcspn(user, "\n")] = '\0';

        for (int i = 0; user[i]; i++) user[i] = tolower(user[i]);

        if (strcmp(user, "exit") == 0) {
            printf("ChatBot: Final Score — You: %d | Bot: %d | Draws: %d\n",
                   userScore, botScore, draws);
            printf("ChatBot: Game ended. Returning to chat...\n");
            break;
        }

        if (strcmp(user, "rock") != 0 && strcmp(user, "paper") != 0 && strcmp(user, "scissors") != 0) {
            printf("ChatBot: That’s not a valid move! Please type rock, paper, or scissors.\n");
            continue;
        }

        int botChoice = (rand() + clock()) % 3;

        printf("ChatBot: I choose %s!\n", choices[botChoice]);

        if (strcmp(user, choices[botChoice]) == 0) {
            printf("ChatBot: It's a draw!\n");
            draws++;
        } else if (
            (strcmp(user, "rock") == 0 && botChoice == 2) ||
            (strcmp(user, "paper") == 0 && botChoice == 0) ||
            (strcmp(user, "scissors") == 0 && botChoice == 1)
        ) {
            printf("ChatBot: You win this round! 🎉\n");
            userScore++;
        } else {
            printf("ChatBot: I win this round! 😎\n");
            botScore++;
        }

        printf("Score — You: %d | Bot: %d | Draws: %d\n\n", userScore, botScore, draws);
    }
}
