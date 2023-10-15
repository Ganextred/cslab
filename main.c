#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <intrin.h>

typedef struct {
    int nalphabet;
    int nstates;
    int istate;
    int *fstates;
    int nfstates;
    int **transitions;
} Automaton;


bool canAcceptW0(int state, Automaton M, const int *w0, int nw0) {
    int currentState = state;
    for (int i = 0; i < nw0; i++) {
        currentState = M.transitions[currentState][w0[i]];
        if (currentState == -1)
            return false;
    }

    for (int i = 0; i < M.nfstates; i++) {
        if (M.fstates[i] == currentState)
            return true;
    }

    return false;
}

bool isReachable(int state, Automaton M, bool visited[]) {
    if (state == M.istate)
        return true;
    visited[state] = true;
    for (int i = 0; i < M.nalphabet; i++) {
        int nextState = M.transitions[state][i];
        if (!visited[nextState] && nextState != -1) {
            if (isReachable(nextState, M, visited)) {
                return true;
            }
        }
    }
    return false;
}

bool check_automaton(Automaton M, const int *w0, int nw0) {
    for (int i = 0; i < M.nstates; i++) {
        if (canAcceptW0(i, M, w0, nw0) && isReachable(i, M, (bool[]) {0})) {
            return true;
        }
    }

    return false;
}

Automaton readAutomatonFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    Automaton A;

    fscanf(file, "%d", &A.nalphabet);

    fscanf(file, "%d", &A.nstates);

    // Read initial state
    fscanf(file, "%d", &A.istate);

    // Read final states number
    fscanf(file, "%d", &A.nfstates);


    int *finalStates = (int *) malloc(A.nfstates * sizeof(int));
    for (int i = 0; i < A.nfstates; i++) {
        fscanf(file, " %d ", &finalStates[i]);
    }
    A.fstates = finalStates;

    A.transitions = (int **) malloc(A.nstates * sizeof(int *));
    for (int i = 0; i < A.nstates; i++) {
        A.transitions[i] = (int *) malloc(A.nalphabet * sizeof(int));
        for (int j = 0; j < A.nalphabet; j++)
            A.transitions[i][j] = -1;
    }

    int s, a, s_next;
    while (fscanf(file, " %d ", &s) != EOF) {
        fscanf(file, " %d ", &a);
        fscanf(file, " %d ", &s_next);
        A.transitions[s][a] = s_next;

    }

    fclose(file);

    return A;
}


int main() {
    // Read Automaton M and w0 from file
    Automaton A = readAutomatonFromFile("input.txt");


    char word[100];
    printf("Enter a word:");
    scanf("%s", word);

    int nw0 = strlen(word);
    int* w0 = (int*)malloc(nw0 * sizeof(int));

    for (int i = 0; i < nw0; i++) {
        w0[i] = word[i]-'a';
    }


    if (check_automaton(A, w0, nw0))
        printf("%s", "True");
    else printf("%s", "False");;

    return 0;
}