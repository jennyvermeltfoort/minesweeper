/* Everything questions.
 */

#ifndef __QUESTION_H
#define __QUESTION_H

#define QUESTION_LIST_SIZE 3
#define QUESTION_ITEM_CHOICES_AMOUNT 4

typedef char question_line_t[64];
typedef char question_line_long_t[164];
typedef struct QUESTION_ITEM
{
    question_line_long_t question;
    question_line_t choices[QUESTION_ITEM_CHOICES_AMOUNT];
    char answer;
} question_item_t;

typedef question_item_t question_list_t[QUESTION_LIST_SIZE];

question_list_t question_list_informal = {
    {
        .question = "Welk literair werk wordt beschouwd als het eerste moderne psychologische roman?",
        .choices = {
            "a. Don Quichot van Miguel de Cervantes",
            "b. De Gebroeders Karamazov van Fjodor Dostojevski",
            "c. Odyssee van Homerus",
            "d. De Toverberg van Thomas Mann",
        },
        .answer = 'b',
    },
    {
        .question = "Welk kunststroming wordt gekenmerkt door een sterke reactie tegen het impressionisme en focust op eenvoudige vormen en een beperking van kleurgebruik?",
        .choices = {
            "a. Surrealisme",
            "b. Kubisme",
            "c. Expressionisme",
            "d. Dadaïsme",
        },
        .answer = 'b',
    },
    {
        .question = "Welke schilder staat bekend om het gebruik van druppeltechniek en het creëren van abstracte expressie in de 20e eeuw?",
        .choices = {
            "a. Pablo Picasso",
            "b. Jackson Pollock",
            "c. Vincent van Gogh",
            "d. Andy Warhol",
        },
        .answer = 'b',
    },
};

question_list_t question_list_formal = {
    {
        .question = "Welke Franse schrijver en filosoof schreef het werk L'Étranger en won de Nobelprijs voor Literatuur in 1957?",
        .choices = {
            "a. Jean-Paul Sartre",
            "b. Albert Camus",
            "c. Marcel Proust",
            "d. André Gide",
        },
        .answer = 'b',
    },
    {
        .question = "Welke kunststroming uit de vroege 20e eeuw wilde de snelheid en dynamiek van de moderne technologie uitdrukken?",
        .choices = {
            "a. Surrealisme",
            "b. Futurisme",
            "c. Kubisme",
            "d. Constructivisme",
        },
        .answer = 'b',
    },
    {
        .question = "Welke Nederlandse schilder is beroemd om zijn abstracte kunstwerken die vooral gebruikmaken van primaire kleuren en rechte lijnen?",
        .choices = {
            "a. Vincent van Gogh",
            "b. Piet Mondriaan",
            "c. Rembrandt van Rijn",
            "d. Jan Steen",
        },
        .answer = 'b',
    },
};

#endif // __QUESTION_H
