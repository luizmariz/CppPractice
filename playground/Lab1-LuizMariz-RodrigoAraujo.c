/*
 * Primeiro laboratório de Computadores e Programação
 *
 * A ideia do laboratório é mostrar ao aluno outras formas de fazer as operações
 * que ele já está acostumado a fazer, mas de formas diferentes.
 * Criamos esse desafio com o objetivo de fazê-lo pensar em diferentes formas de se
 * atingir o mesmo resultado. Se possível, tentando pensar em qual seria mais
 * eficiente.
 *
 * O aluno deverá usar apenas um subset de operações de C para realizar o que
 * for pedido no enunciado.
 *
 * Todas as operações permitidas serão especificadas em cada questão.
 *
 * Forma de avaliação:
 *      - Quantas operações o aluno utlizou para realizar a tarefa (dentro da quantidade aceitável)
 *      - Explicação do código -- deverá ser o mais claro possível (como qualquer
 *          código), imaginando que qualquer pessoa sem conhecimento prévio da
 *          matéria consiga entender o que foi feito.
 *      - As resoluções com menos operações do que a do monitor terão bonificação.
 *
 * Dupla:
 *      Alunos:
 *          Luiz Gustavo Oliveira Mariz - 118033995
 *          Rodrigo Araújo – DRE 118045366
 *
 *      Versão do GCC utilizada: 7.5.0
 *
 */

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#include <stdio.h>
#include <stdint.h>


/* Número é par ou não
 *      Permitido:
 *          Operações: ~ & ^ | << >>
 *
 *      Número máximo de operações: 3
 *      Monitor: 2
 *
 *      Retorna 1 se x é par, retorna 0 caso contrário
 *
 *      Exemplo:
 *          ehPar(0) -> 1
 *          ehPar(2) -> 1
 *          ehPar(7) -> 0
 */
int32_t ehPar(int32_t x) {
    /*
        Para descobrirmos se é par o que utilizaremos é a comparação no último bit do valor.
        Caso o último bit seja 1 é ímpar, caso seja 0 é par.
        O operador & compara bit a bit dos seus operandos e se ambos forem 1 retorna 1,
        dessa forma comparando com 1(0x1) sabemos que se o último valor é 1 retornará 1, e por isso é ímpar,
        e com o operando ~ temos o complemento do valor e teremos se é par
    */
    return ~x & 1;
}

/*
 * Módulo 8
 *      Permitido:
 *          Operações: ~ & ^ | ! << >>
 *
 *      Número máximo de operações: 3
 *      Monitor: 1
 *
 *      Retorna x % 8
 *
 *      Exemplo:
 *          mod8(1) -> 1
 *          mod8(7) -> 7
 *          mod8(10) -> 2
 */
int32_t mod8(int32_t x) {
    /*
     * Explicação:
     *     Basta comparar os 3 últimos bits, mas por que?
     *
     *     Considere as seguintes afirmativas:
     *          1) 8 na base 2 equivale a 0b1000;
     *          2) x mod8 equivale ao resto da divisão inteira x / 8;
     *          3) Qualquer número binário do tipo bbbb...bbbb000 (Uma sequência de bits terminada em 000) é divisível por 8.
     *
     *          Por que? Bem, 0b1000 é 1*2^3 = 8,  0b11000 é 1*2^4 + 1*2^3 = 24, 0b100000 é 1*2^5 = 32 e assim a diante...
     *
     *          Observe que todos os números dessa forma são múltiplos de 8, o que faz sentido, pois
     *          tudo nessa forma poderá ser decomposto como:
     *
     *                  [2^3 * 2^y , y > 3, y pertencente aos naturais]
     *
     *          Obs.: O bit mais significativo que indica o sinal).
     *
     *     Com a afirmativa 3 em mente, agora temos que qualquer número em bits pode ser descrito
     *     em uma parte divísivel por 8 + o resto.
     *          Exemplo:
     *                  0b11111 => 0b11000 + 0b00111
     *                  47      => 40      + 7
     *
     *                  Que podemos escrever como 47 mod 8 == 7
     *
     *                  0b00011 => 0b00000 + 0b00011
     *                  3       => 0       + 3
     *
     *                  Que podemos escrever como 3 mod 8 == 3
     *
     *     Assim temos que um int32 mod 8 será sua parte divisível por 8 (Portanto, qualquer coisa do tipo descrito em 3)
     *     + seu resto que será algo da forma 0000...0bbb sendo bbb os 3 últimos bits, portanto os 3 menos significativos.
     *
     *     Dito isso fica fácil, basta criar uma máscara que retorne sempre os 3 bits menos significativos, o que pode ser
     *     obtido usando o operador & (AND) e a máscara 0111.
     *          Observe:
     *                  0b11111 & 0b00111 => (Operando bit a bit temos) 0b00111
     */
    return x & 7;
}

/* Negativo sem -
 *      Permitido:
 *          Operações: ~ & ^ | ! << >> +
 *
 *      Número máximo de operações: 5
 *      Monitor: 2
 *
 *      Retorna -x
 *
 *      Exemplo:
 *          negativo(1) -> -1
 *          negativo(42) -> -42
 */
int32_t negativo(int32_t x) {
    /*
        Para conseguirmos o negativo de um valor precisamos fazer o complemento a dois em sua representação binária,
        dessa forma fazemos o seu complemento e somamos 1
    */
    return ~x + 1;
}

/* Implementação do & usando bitwise
 *      Permitido:
 *          Operações: ~ ^ | ! << >>
 *
 *      Número máximo de operações: 7
 *      Monitor: 4
 *
 *      Retorna x & y
 *
 *      Exemplo:
 *          bitwiseAnd(1, 2) -> 0
 *              01 & 10 -> 00
 *          bitwiseAnd(3, 11) -> 3
 *              11 & 1011 -> 0011
 */
int32_t bitwiseAnd(int32_t x, int32_t y) {
    /*
        Podemos chegar em um resultado seguindo o primeiro Teorema de De Morgan, que diz que:
        !(A & B) = !A | !B
        negando dos dois lados ficamos com
        (A & B) = !(!A | !B)
        e com isso tiramos nossa resposta.
        Basta aplicarmos o operador |(Or) no complemento(~) de x e no de y
        e nesse resultado aplicarmos o complemento

        Exemplo:

        (01 & 11) -> 01

        ~(~01 | ~11) -> ~(10 | 00) -> ~(10) -> 01
    */
    return ~(~x | ~y);
}

/* Igual sem ==
 *      Permitido:
 *          Operações: ~ & ^ | << >> !
 *
 *      Número máximo de operações: 3
 *      Monitor: 2
 *
 *      Retorna 1 se x == y, 0 caso contrário
 *
 *      Exemplo:
 *          ehIgual(10, 10) -> 1
 *          ehIgual(16, 8) -> 0
 */
int32_t ehIgual(int32_t x, int32_t y) {
    /*
     * Explicação:
     *      O operador lógico xor, bit a bit, vai sempre retornar 0 em uma comparação igual. O que nos dá
     *      0 como saída  da operação binária XOR entre 2 números equivalentes e um binário qualquer entre 2
     *      não equivalentes.
     *
     *      Exemplo:
     *          0b1000 XOR 0b1000 => 0b0000
     *          0b1011 XOR 0b1000 => 0b0011
     *
     *      Agora podemos aproveitar que o operador lógico ! para retornar 1 caso a 
     */
    return !(x^y);
}

/* Limpa bit n
 *      Permitido:
 *          Operações: ~ & ^ | ! << >>
 *
 *      Número máximo de operações: 4
 *      Monitor: 3
 *
 *      Retorna o x com o bit n = 0,
 *      n pode variar entre 0 e 31, do LSB ao MSB
 *
 *      Exemplo:
 *          limpaBitN(3, 0) -> 2
 *          limpaBitN(3, 1) -> 1
 */
int32_t limpaBitN(int32_t x, int8_t n) {
    /*
     * Explicação:
     *      Tomando o exemplo limpaBitN(3, 1) -> 1 e considerando 4bits na arquitetura como ilustração.
     *
     *      Precisamos andar n casas para a esquerda e zerar esse bit. Podemos obter isso da seguinte forma:
     *          1) Podemos zerar um bit usando a operação AND com 0 na posição desejada
     *            0b0011 & 0b1101 -> 0b0001
     *
     *          2) Mas como obter a mascara 0b1101 ? Simples, podemos dar um shift a esquerda n vezes em 0b0001
     *             e inverter o valor bit a bit
     *             0b0001 << 1 -> 0b0010
     *             ~0b0010 -> 0b1101 (Que é o que precisávamos)
     */
    return x & ~(1 << n);
}

/*
 * Bit na posição p do inteiro x
 *      Permitido:
 *          Operações bitwise: << >> | & + -
 *
 *      Número máximo de operações: 6
 *      Monitor: 2
 *
 *      Retorna o valor do bit na posição p no inteiro x
 *      Valor de retorno pode ser apenas 0 ou 1
 *
 *      p será um valor entre 0 e 31
 *
 *      Exemplo:
 *          23 em binário: 0   0 ... 0 1 0 1 1 1
 *               posições: 31 30 ... 5 4 3 2 1 0
 *
 *          bitEmP(23, 31) -> 0
 *          bitEmP(23, 5) -> 0
 *          bitEmP(23, 4) -> 1
 *          bitEmP(23, 3) -> 0
 *          bitEmP(23, 2) -> 1
 *          bitEmP(23, 1) -> 1
 *          bitEmP(23, 0) -> 1
 *
 */
int32_t bitEmP(int32_t x, uint8_t p) {
    /*
        Podemos pegar o valor na posição fazendo o shift à direita
        para que o valor na posição p vá para o LSB.
        Sendo assim, podemos verificar se o valor em LSB é 0 ou 1 usando o operador & e o operando 1.
    */
    return x >> p & 1;
}

/*
 * Byte na posição p do inteiro x
 *      Permitido:
 *          Operações: << >> | ! &
 *
 *      Número máximo de operações: 6
 *      Monitor: 3
 *
 *      Retorna o valor do bit na posição p no inteiro x
 *      Valor de retorno pode ser entre 0 e 0xFF
 *
 *      p será um valor entre 0 e 3
 *      0 retorna LSB
 *      3 retorna MSB
 *
 *      Exemplo:
 *          byteEmP(0x12345678, 0) -> 0x78
 *          byteEmP(0x12345678, 1) -> 0x56
 *          byteEmP(0x12345678, 2) -> 0x34
 *          byteEmP(0x12345678, 3) -> 0x12
 *
 */
int32_t byteEmP(int32_t x, uint8_t p) {
    /*
     * Explicação:
     *      Tomando o exemplo byteEmP(0x321, 1) -> 0x03.
     *
     *      Nesse caso, precisamos pegar o byte mais significativo 0x03:
     *          1) Para fazer isso, basta fazer uma operação AND com 0xff que conseguimos extrair
     *             um byte do nosso binário
     *
     *          2) Agora precisamos apenas "posicionar" esse byte nas casas corretas e podemos atingir isso
     *             utilizando um shift a direita, "empurrando" nosso binário para a direita.
     *             0x321 >> n == 8 -> 0x03
     *
     *          3) Visto que n varia em função de p, precisamos encontrar uma forma de, nesse caso,
     *             1 virar 8. Sendo assim observe que dado os valores de p temos os seguintes n:
     *
     *                  p vira n -> 0 vira 0, 1 vira 8, 2 vira 16, 3 vira 24
     *
     *             Ou seja n = p*8 que equivale a p=n*2^3
     *
     *              Agora ficou simples, para multiplicar um número por uma potência de 2 basta fazer
     *              um shift a esquerda pelo expoente da potência, portanto basta usar p << 3 para deslocar
     *              os bits de x
     */
    return 0xff & (x >> (p << 3));
}

/*
 * Seta byte na posição p do inteiro x como y
 *      Permitido:
 *          Operações: << >> | ~ ! &
 *
 *      Número máximo de operações: 7
 *      Monitor: 5
 *
 *      Retorna x com o valor y no byte da posição p
 *
 *      p será um valor entre 0 e 3
 *      0 retorna LSB
 *      3 retorna MSB
 *
 *      Exemplo:
 *          setaByteEmP(0x12345678, 0xFF, 0) -> 0x123456FF
 *          setaByteEmP(0x12345678, 0xFF, 1) -> 0x1234FF78
 *          setaByteEmP(0x12345678, 0xFF, 2) -> 0x12FF5678
 *          setaByteEmP(0x12345678, 0xFF, 3) -> 0xFF345678
 *
 */
int32_t setaByteEmP(int32_t x, int32_t y, uint8_t p) {
    /*
     * Explicação:
     *      Tomando o exemplo setaByteEmP(0x12345678, 0xff, 1) -> 0x1234ff78 dado acima.
     *
     *      Nesse caso, precisamos "zerar" o byte em x que será substituido (Para que possamos via OR substituir ele):
     *          1) Para fazer isso, podemos fazer um shift a esquerda em 0xff pelo número de bytes obtendo, considerando
     *             p == 1, 0xff00 por exemplo (Já sabemos como fazer isso, na func byteEmP explico que basta fazer o shift p << 3 vezes)
     *
     *          2) Agora basta ~(1) para obter 0x00ff que com um AND entre x vamos conseguir "zerar" os bytes desejados. Porque
     *             0x12345678 & 0x00ff -> 0x12340078
     *
     *          3) O último passo é substituir esse "pedaço zerado" por y. Nesse caso basta primeiro "alinhar" os bits, o que já sabemos
     *             fazer via um shift p << 3 vezes. Feito isso, basta fazer uma operação OU para mesclar os bits.
     *
     *             0xff << p == 1 << 3 -> 0xff00
     *
     *             0x12340078 |
     *             0x0000ff00
     *
     *             0x1234ff78 (Que é o retorno desejado)
     */
    return (~(0xff << (p << 3)) & x) | (y << (p << 3));
}

/*
 * Minimo
 *      Permitido:
 *          Operações: << >> | ^ < > ~ ! & -
 *
 *      Número máximo de operações: 15
 *      Monitor: 5
 *
 *      Retorna o menor numero entre x e y
 *
 *      Exemplo:
 *          minimo(10, 15) -> 10
 *          minimo(-2, -1) -> -2
 *          minimo(-1, 2) -> -1
 *
 */
int32_t minimo(int32_t x, int32_t y) {
    /*
     * Explicação:
     *      Podemos tentar simular o que seria um:
     *
     *          if (x < y) return x
     *          else return y
     *
     *      Sabemos que x < y pode retornar true ou false, agora como simular o if?
     *
     *      Podemos usar a operação XOR, observe:
     *
     *          x ^ y ^ y -> x
     *          y ^ x ^ x -> y
     *
     *      Isso porque n^n resultara em 0. Lembra tambem do ehIgual?. Qualquer bit
     *      XOR 0 retorna ele mesmo:
     *
     *          b ^ 0 -> 0
     *
     *      Assim temos os seguintes casos:
     *
     *          1) Se x < y for true devemos retornar x ^ y ^ y ou x ^ 0
     *          2) Se x < y for false devemos retornar x ^ y ^ x ou y ^ 0
     *
     *      Bem, se x < y -> 1, sabemos que isso é 0x1, podemos converter isso em 0xffffffff facilmente, basta fazer -(1)
     *      Agora, qual a utilidade disso?
     *
     *      (x ^ y) & 0 -> 0
     *      (x ^ y) & 1 -> x ^ y
     *
     *      Fica fácil observar agora que:
     *          Se x menor que y:
     *              -(x > y) -> 0 então precisamos retornar 0 ^ x
     *          Agora se x maior que y:
     *              -(x > y) -> 1 então precisamos retornar x ^ y
     *
     *          O que podemos obter com o caso (1) listado acima:
     *
     *             x ^ (-(x > y) & (x ^ y))
     *
     *             Se x menor, retorna x ^ (0 ^ (x^y) -> 0) -> x
     *             Se x maior, retorna x ^ (1 ^ (x^y) -> (x^y)) -> y
     */
    return x ^ (-(x > y) & (x ^ y));
}

/*
 * Negação lógica sem !
 *      Permitido:
 *          Operações: << >> | & + ~
 *
 *      Número máximo de operações: 15
 *      Monitor: 5
 *
 *      Retorna 1 se x == 0, retorna 0 caso contrário
 *
 *      Exemplo:
 *          negacaoLogica(0) -> 1
 *          negacaoLogica(37) -> 0
 *
 */
int32_t negacaoLogica(int32_t x) {
    /*
        A lógica que usaremos é fazendo a verificação se o número não é positivo nem negativo,
        porque dessa forma podemos afirmar que é zero.
        Por isso usaremos um OU(|) em que seus operandos verificam um se é positivo e o outro negativo.
        Temos (x>>31) que faz a verificação se é negativo, fazendo o shift em 31 bits passamos a ter somente
        o bit menos significativo e no caso de negativo ele é -1, caso seja positivo ou zero é 0.
        No outro lado do OU temos (~x+1)>>31, que é o mesmo shift porém no complemento a dois de x,
        e por isso a lógica se inverte, ao fazer o shift de 31 bits no complemento a dois do positivo dá -1,
        e no complemento à dois do negativo ou do zero dá 0. 
        Logo, caso os dois operandos sejam zeros, comprovando que x é 0, ao somar 1 retornará 1,
        fazendo jus à negação lógica.
        E caso seja negativo ou positivo, ao somar 1 teremos 0,
        dado que pelo menos um dos operandos será -1 nesses casos.
    */
    return ((x >> 31) | ((~x+1)>>31)) + 1;
}

void teste(int32_t saida, int32_t esperado) {
    static uint8_t test_number = 0;
    test_number++;
    if(saida == esperado)
        printf(ANSI_COLOR_GREEN "PASSOU! Saída: %-10d\t Esperado: %-10d\n" ANSI_COLOR_RESET,
            saida, esperado);

    else
        printf(ANSI_COLOR_RED "%d: FALHOU! Saída: %-10d\t Esperado: %-10d\n" ANSI_COLOR_RESET,
            test_number, saida, esperado);
}

int main() {
    puts(ANSI_COLOR_BLUE "Primeiro lab - bits" ANSI_COLOR_RESET);
    puts("");

    puts("Teste: ehPar");
    teste(ehPar(2), 1);
    teste(ehPar(1), 0);
    teste(ehPar(3), 0);
    teste(ehPar(13), 0);
    teste(ehPar(100), 1);
    teste(ehPar(125), 0);
    teste(ehPar(1024), 1);
    teste(ehPar(2019), 0);
    teste(ehPar(2020), 1);
    teste(ehPar(-1), 0);
    teste(ehPar(-27), 0);
    teste(ehPar(-1073741825), 0);
    teste(ehPar(1073741824), 1);
    teste(ehPar(2147483647), 0);
    teste(ehPar(-2147483648), 1);
    teste(ehPar(0), 1);
    puts("");

    puts("Teste: mod8");
    teste(mod8(0), 0);
    teste(mod8(4), 4);
    teste(mod8(7), 7);
    teste(mod8(8), 0);
    teste(mod8(-1), 7);
    teste(mod8(-8), 0);
    teste(mod8(2147483647), 7);
    teste(mod8(-2147483648), 0);
    puts("");

    puts("Teste: negativo");
    teste(negativo(0), 0);
    teste(negativo(1), -1);
    teste(negativo(-1), 1);
    teste(negativo(2147483647), -2147483647);
    teste(negativo(-2147483647), 2147483647);
    teste(negativo(-2147483648), 2147483648);
    puts("");

    puts("Teste: bitwiseAnd");
    teste(bitwiseAnd(1, 3), 1);
    teste(bitwiseAnd(-1, 0), 0);
    teste(bitwiseAnd(-1, 0x7FFFFFFF), 0x7FFFFFFF);
    teste(bitwiseAnd(0b0100, 0b1100), 0b0100);
    puts("");

    puts("Teste: ehIgual");
    teste(ehIgual(1,1), 1);
    teste(ehIgual(1,0), 0);
    teste(ehIgual(0,1), 0);
    teste(ehIgual(-1,1), 0);
    teste(ehIgual(-1,-1), 1);
    teste(ehIgual(2147483647,-1), 0);
    teste(ehIgual(2147483647,-2147483647), 0);
    teste(ehIgual(2147483647,-2147483648), 0);
    teste(ehIgual(2147483647,-2147483648), 0);
    puts("");

    puts("Teste: limpaBitN");
    teste(limpaBitN(1,0), 0);
    teste(limpaBitN(0b1111,1), 0b1101);
    teste(limpaBitN(15,3), 7);
    teste(limpaBitN(-1,31), 2147483647);
    teste(limpaBitN(-1,0), -2);
    teste(limpaBitN(2147483647, 30), 1073741823);
    puts("");

    puts("Teste: bitEmP");
    teste(bitEmP(1, 0), 1);   //    b01 => retorna 1
    teste(bitEmP(1, 1), 0);   //    b01 => retorna 0
    teste(bitEmP(2, 0), 0);   //    b10 => retorna 0
    teste(bitEmP(2, 1), 1);   //    b10 => retorna 1
    teste(bitEmP(9, 2), 0);   //  b1001 => retorna 0
    teste(bitEmP(-4194305, 22), 0);
    teste(bitEmP(9, 3), 1);
    teste(bitEmP(16, 3), 0);
    teste(bitEmP(0x1 << 5, 4), 0);
    teste(bitEmP(0x1 << 31, 31), 1);
    teste(bitEmP(-1073741825, 30), 0);
    teste(bitEmP(-1073741825, 31), 1);
    puts("");

    puts("Teste: byteEmP");
    teste(byteEmP(0x766B, 1), 0x76);
    teste(byteEmP(0x766B, 0), 0x6B);
    teste(byteEmP(0x8420, 0), 0x20);
    teste(byteEmP(0x12345678, 3), 0x12);   // retorna 0x12
    teste(byteEmP(0x12345678, 2), 0x34);   // retorna 0x34
    teste(byteEmP(0x12345678, 1), 0x56);   // retorna 0x56
    teste(byteEmP(0x12345678, 0), 0x78);   // retorna 0x78
    teste(byteEmP(0x321, 1), 0x03);        // retorna 0x03
    teste(byteEmP(0x321, 0), 0x21);        // retorna 0x21
    puts("");

    puts("Teste: setaByteEmP");
    teste(setaByteEmP(0x00, 0xFF, 0), 0x000000FF);
    teste(setaByteEmP(0x00, 0xFF, 1), 0x0000FF00);
    teste(setaByteEmP(0x00, 0xFF, 2), 0x00FF0000);
    teste(setaByteEmP(0x00, 0xFF, 3), 0xFF000000);
    teste(setaByteEmP(0x01234567, 0x33, 2), 0x01334567);
    teste(setaByteEmP(0xdeadbeef, 0x00, 0), 0xdeadbe00);
    teste(setaByteEmP(0xdeadbeef, 0x00, 1), 0xdead00ef);
    puts("");

    puts("Teste: minimo");
    teste(minimo(0,1), 0);
    teste(minimo(0,10), 0);
    teste(minimo(1, 128), 1);
    teste(minimo(-1, 0), -1);
    teste(minimo(-1, -2), -2);
    teste(minimo(2147483647, 2147483646), 2147483646);
    teste(minimo(-2147483648, -2147483647), -2147483648);
    teste(minimo(-2147483648, -1), -2147483648);
    puts("");

    puts("Teste: negacaoLogica");
    teste(negacaoLogica(0), 1);
    teste(negacaoLogica(1), 0);
    teste(negacaoLogica(-1), 0);
    teste(negacaoLogica(64), 0);
    teste(negacaoLogica(-64), 0);
    teste(negacaoLogica(2147483647), 0);
    teste(negacaoLogica(-2147483648), 0);
    puts("");

}