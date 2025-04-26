#define pstAvanco 4  //Vou usar define para as constantes, usa menos memoria
#define pstAliment 5
#define pstExpulsao 3
#define snsrBatente 7
#define botaoInicio 6
#define potenciometro A1    //Isto talvez de erro
#define filtroDebounce 200  //Isto define quanto tempo o botão deve ser segurado para registar a mudança

int valPoten = 0;     //Esse é o valor do potenciometro
int delayMap = 0;     //Isso é o valor do Map que controla a velocidade da maquina
int dbncUEstado = 0;  //Registra ultima estado do debounce

int maquinaLigada = LOW;
unsigned long dbncUTempo = 0;  //Registra quando foi a ultima vez que o botão foi pressionado

void setup() {

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(A1, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  digitalWrite(pstAvanco, HIGH);
  digitalWrite(pstExpulsao, HIGH);
  digitalWrite(pstAliment, HIGH);
  //Serial.begin(9600);               //Isto é para Debug
}

void loop() {

  if (digitalRead(botaoInicio) == LOW) {
    delay(1000);
    if (digitalRead(botaoInicio) == HIGH) {maquinaLigada = !maquinaLigada;}  //Liga e desliga a maquina sem usar um debounce
  }

  if (maquinaLigada == 1) {
    valPoten = analogRead(potenciometro);      //Estas duas linhas definem o delay entra cada ciclo
    delayMap = map(valPoten, 0, 1024, 1200, 400);
    dbncUEstado = digitalRead(snsrBatente);    //Debounce

    if (digitalRead(snsrBatente) == HIGH) {cicloDeAlimentacao();}       //Se o sensor do batente não estiver apertado, rodar ciclo de alimentação

    if (digitalRead(snsrBatente) != dbncUEstado) {dbncUTempo = millis();}       //Se qualquer mudança do sensor ocorrer, anotar hora

    if ((millis() - dbncUTempo > filtroDebounce) && (digitalRead(snsrBatente) == LOW)) { cicloDeFuro(); }   //Se debounce e sensor ativados, Rodar ciclo de furo
  }
  //Serial.println(maquinaLigada);
}


void cicloDeAlimentacao() {

  delay(delayMap);
  digitalWrite(pstAliment, LOW);  //O pistão ESTENDE no LOW
  delay(200);                     //Tempo para o pistão de alimentação estender totalmente
  digitalWrite(pstAliment, HIGH);
}

void cicloDeFuro() {

  digitalWrite(pstAliment, LOW);  //Pressionar batente e Furar
  digitalWrite(pstAvanco, LOW);
  delay(400);                     //Esperar Furar
  digitalWrite(pstAvanco, HIGH);  //Levantar prensa e soltar batente
  digitalWrite(pstAliment, LOW);
  delay(200);
  digitalWrite(pstExpulsao, LOW);  //Espera,expulsa peça, espera, retrai pistão
  delay(200);
  digitalWrite(pstExpulsao, HIGH);
  delay(100);  //Esperançosamente, esse delay impedira que expulse 2 vezes
}