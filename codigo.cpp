#include <iostream>
#include <climits>
#include <cstdlib>
#include <ctime>

using namespace std;

#define MAX_SPACE_TO_WIN  3
#define ROWS 6
#define COLUMNS 7
#define EMPTY 0
#define HUMAN 1
#define AI 2

int gerarNumeroAleatorio() {
    static bool sementeInicializada = false;
    if (!sementeInicializada) {
        srand(time(0));
        sementeInicializada = true;
    }
return rand() % 7;
}

typedef struct node {
    int tabuleiro[6][7];
    char estado;
    int coluna_jogada;
    int score;
    struct node* p[7];
    struct node* pai;
} Node;

int verificaVencedor(Node* arvore);

int proxLinhaLivre(Node* arvore, int coluna);

void insereElemento(Node* &arvore, int coluna, int timeX);

bool temElemento(Node* arvore, int linha, int coluna);

void cria(Node* &arvore) {

    arvore = new Node;

    arvore->pai = nullptr;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            arvore->tabuleiro[i][j] = 0;
        }
    }

    for(int i=0;i<7;i++){
        arvore->p[i] = nullptr;
    }
}

void gera(Node* &arvore, int depth, int MaxDepth,int time){
    
    int state = verificaVencedor(arvore);
    
    switch (state)
    {
    case 2:
        arvore->estado = 'V';
        break;
    case 1:
        arvore->estado = 'D';
        break;
    case 3:
        arvore->estado = 'E';
        break;
    case 0:
        arvore->estado = '?';
    default:
        break;
    }
    
    if((arvore->estado == 'V' || arvore->estado == 'D') || depth > MaxDepth){
       for(int i=0;i<7;i++){
            arvore->p[i] = nullptr;
        }
    }else{
        for(int i=0;i<7;i++){   
            if(proxLinhaLivre(arvore, i) != -1){ 
                arvore->p[i] = new Node;
                arvore->p[i]->pai = arvore;
                for(int a=0;a<6;a++)
                    for(int b=0;b<7;b++)
                        arvore->p[i]->tabuleiro[a][b] = arvore->tabuleiro[a][b];
                insereElemento(arvore->p[i],i,time);  
                arvore->coluna_jogada = i;
                gera(arvore->p[i], depth + 1, MaxDepth, time == 2 ? 1 : 2);
            }else{
                arvore->p[i] = nullptr;
            }
        }
    }
}

void destroi(Node* &arvore) {
    for(int i=0;i<7;i++){
        if(arvore->p[i] != nullptr){
            destroi(arvore->p[i]);
        }
    }

    delete arvore;
    arvore = nullptr;
}

void printTabuleiro(Node* arvore) {
    cout << "(0) (1) (2) (3) (4) (5) (6)" << endl;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            cout << " " << arvore->tabuleiro[i][j] << " ";
        }
    cout << endl;
    }

    cout << endl;
    }

bool temElemento(Node* arvore, int linha, int coluna) {
    return (arvore->tabuleiro[linha][coluna] != 0);
}

int proxLinhaLivre(Node* arvore, int coluna) {
    int linha = 5;
    while (linha >= 0) {
        if (!temElemento(arvore, linha, coluna))
            return linha;
        else
            linha--;
    }
    return -1;
}

void insereElemento(Node* &arvore, int coluna, int timeX) {
    if (proxLinhaLivre(arvore, coluna) != -1)
        arvore->tabuleiro[proxLinhaLivre(arvore, coluna)][coluna] = timeX;
}

int verificaVencedor(Node* arvore) {
    int timeX = 0;
    
    for(int a=0;a<2;a++){ 
        timeX++;
        // Verificar vitória na horizontal
        for (int linha = 0; linha < 6; linha++) {
            for (int coluna = 0; coluna < 4; coluna++) {
                if (arvore->tabuleiro[linha][coluna] == timeX &&
                arvore->tabuleiro[linha][coluna + 1] == timeX &&
                arvore->tabuleiro[linha][coluna + 2] == timeX &&
                arvore->tabuleiro[linha][coluna + 3] == timeX) {
                    return timeX;
                }
            }
        }

        // Verificar vitória na vertical
        for (int coluna = 0; coluna < 7; coluna++) {
            for (int linha = 0; linha < 3; linha++) {
                if (arvore->tabuleiro[linha][coluna] == timeX &&
                arvore->tabuleiro[linha + 1][coluna] == timeX &&
                arvore->tabuleiro[linha + 2][coluna] == timeX &&
                arvore->tabuleiro[linha + 3][coluna] == timeX) {
                    return timeX;
                }
            }
        }

        //verifica vitoria na diagonal para baixo
        for (int linha = 0; linha < 3; linha++) {
            for (int coluna = 0; coluna < 4; coluna++) {
                if (arvore->tabuleiro[linha][coluna] == timeX &&
                arvore->tabuleiro[linha + 1][coluna + 1] == timeX &&
                arvore->tabuleiro[linha + 2][coluna + 2] == timeX &&
                arvore->tabuleiro[linha + 3][coluna + 3] == timeX) {
                    return timeX;
                }
            }
        }

        // Verificar vitória na diagonal para cima
        for (int linha = 3; linha < 6; linha++) {
            for (int coluna = 0; coluna < 4; coluna++) {
                if (arvore->tabuleiro[linha][coluna] == timeX &&
                arvore->tabuleiro[linha - 1][coluna + 1] == timeX &&
                arvore->tabuleiro[linha - 2][coluna + 2] == timeX &&
                arvore->tabuleiro[linha - 3][coluna + 3] == timeX) {
                    return timeX;
                }
            }
        }
    }
    // Verificar empate
    int linhasCheias = 0;
    for (int j=0; j<7; j++)
        if (proxLinhaLivre(arvore, j) == -1)
            linhasCheias++;
    if(linhasCheias == 6)
        return 3;
    return 0; // Nenhum time venceu ainda
}

int evaluate_adjacents(int adjacents[4], int player){
    int oponnent = AI;
    if(player == AI)
        oponnent = HUMAN;
    int score = 0;
    int player_pieces = 0;
    int empty_spaces = 0;
    int opponent_pieces = 0;
    for(int p=0;p<4;p++){
        if (adjacents[p] == player)
            player_pieces +=1;
        else if(adjacents[p]==EMPTY)
            empty_spaces+=1;
        else if (adjacents[p]==oponnent)
            opponent_pieces +=1;
    }
    if (player_pieces == 4)
        score += 99999;
    else if (player_pieces == 3 && empty_spaces == 1)
        score += 100;
    else if (player_pieces == 2 && empty_spaces == 2)
        score += 10;
    return score;
}

int calculaScore(Node* arvore, int player){
    int score = 0;
    //mais peso para colunas centrais
    for(int col=2;col<5;col++)
        for(int row=0; row<ROWS;row++)
            if(arvore->tabuleiro[row][col] == player){
                if(col == 3)
                    score +=3;
                else
                    score+=2;
            }

    //horizontal
    for(int col=0; col<COLUMNS - MAX_SPACE_TO_WIN;col++)
        for(int row=0; row < ROWS;row++){
            int adjacent_pieces[4] = {arvore->tabuleiro[row][col], arvore->tabuleiro[row][col+1],
                                      arvore->tabuleiro[row][col+2], arvore->tabuleiro[row][col+3]};
            score += evaluate_adjacents(adjacent_pieces,player);
        }

    //vertical
    for(int col=0; col<COLUMNS;col++)
        for(int row=0; row < ROWS-MAX_SPACE_TO_WIN;row++){
            int adjacent_pieces[4] = {arvore->tabuleiro[row][col], arvore->tabuleiro[row+1][col],
                                      arvore->tabuleiro[row+2][col], arvore->tabuleiro[row+3][col]};
            score += evaluate_adjacents(adjacent_pieces,player);
        }
    
    //diagonal upwards
    for(int col=0; col<COLUMNS-MAX_SPACE_TO_WIN;col++)
        for(int row=0; row < ROWS-MAX_SPACE_TO_WIN;row++){
            int adjacent_pieces[4] = {arvore->tabuleiro[row][col], arvore->tabuleiro[row+1][col+1],
                                      arvore->tabuleiro[row+2][col+2], arvore->tabuleiro[row+3][col+3]};
            score += evaluate_adjacents(adjacent_pieces,player);
        }
    //diagonal downwards
    for(int col=0; col<COLUMNS-MAX_SPACE_TO_WIN;col++)
        for(int row=MAX_SPACE_TO_WIN; row < ROWS;row++){
            int adjacent_pieces[4] = {arvore->tabuleiro[row][col], arvore->tabuleiro[row-1][col+1],
                                      arvore->tabuleiro[row-2][col+2], arvore->tabuleiro[row-2][col+3]};
            score += evaluate_adjacents(adjacent_pieces,player);
        }
    return score;
}

int minimax(Node* arvore, int depth, bool isMaximazer){
    if(depth==0 || arvore->estado == 'V' || arvore->estado == 'D' || arvore->estado == 'E'){
        if(arvore->estado != '?'){
            if(arvore->estado == 'D')
                return -1000000000;
            else if(arvore->estado == 'V')
                return 1000000000;
            else    
                return 0;
        }else
            return calculaScore(arvore, isMaximazer ? AI : HUMAN);
    }

    if(isMaximazer){
        int bestScore = INT_MIN;
        for(int i = 0; i<7;i++){
            if(arvore->p[i] != nullptr){
                int score = minimax(arvore->p[i],depth-1,false);
                bestScore = max(score,bestScore);
            }
        }
        return bestScore;
    }else{
         int bestScore = INT_MAX;
        for (int i = 0; i < 7; ++i) {
            if (arvore->p[i] != nullptr) {
                int score = minimax(arvore->p[i], depth - 1, true);
                bestScore = min(score, bestScore);
            }
        }
        return bestScore;
    }
}

int escolherMelhorJogada(Node* arvore, int depth) {
    int melhorJogada = -1;
    int melhorScore = INT_MIN;

    for (int i = 0; i < 7; ++i) {
        if (arvore->p[i] != nullptr) {
            int score = minimax(arvore->p[i], depth, false);
            if (score > melhorScore) {
                melhorScore = score;
                melhorJogada = i;
            }
        }
    }

    return melhorJogada;
}

int main() {
    Node* arvore;
    cria(arvore);
    int profMax=6;

    while(true){
        int EstadoJogo = verificaVencedor(arvore);
        if(EstadoJogo != 0) break;

        int colunaSelecionada;
        cout << "Selecione uma coluna (0 a 6): ";
        cin >> colunaSelecionada;
        insereElemento(arvore,colunaSelecionada,1); // Jogador humano joga
        printTabuleiro(arvore);

        EstadoJogo = verificaVencedor(arvore);
        if (EstadoJogo != 0) break; 

        gera(arvore, 0, profMax, 2); // Gera a árvore de decisões para a IA
        int melhorJogada = escolherMelhorJogada(arvore, profMax); // Encontra a melhor jogada usando Minimax
        insereElemento(arvore, melhorJogada, 2); // IA joga
        printTabuleiro(arvore);

        for (int i = 0; i < 7; i++) {
            if (arvore->p[i] != nullptr) {
                destroi(arvore->p[i]);
                arvore->p[i] = nullptr;
            }
        }
    }

    cout << "Fim de jogo!" << endl;
    int resultadoFinal = verificaVencedor(arvore);
    if (resultadoFinal == 1) {
        cout << "Você ganhou!" << endl;
    } else if (resultadoFinal == 2) {
        cout << "Você perdeu :<" << endl;
    } else {
        cout << "Empate!" << endl;
    }

    destroi(arvore); // Limpa a memória
    return 0;
}

