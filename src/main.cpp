#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

/* Metadata. */
#define STUDENT_NUMBER 3787494
#define STUDENT_NAME "Jenny Vermeltfoort"
#define DATE_UPDATED "09-01-2024"
#define OPDRACHT "1"
#define STUDIE "Bachelor Informatica"

#define LINE_LONG_SIZE_CHAR 128
#define LINE_XLONG_SIZE_CHAR 164
#define LINE_SHORT_SIZE_CHAR 64

#define LIMIT_FORMAL_AGE 30

#define MATH_ANSWER_FLOAT_LIMIT 0.1

#define DATE_NORM_YEAR \
    1876  // Calculations will be normalized to saturday, Januari 1th 1876.
          // Oldest person alive.
          // 1876 is a schrikkeljaar
#define DAYS_IN_YEAR 365

typedef char line_short_t[LINE_SHORT_SIZE_CHAR];
typedef char line_xlong_t[LINE_XLONG_SIZE_CHAR];

/* A struct that defines a variable question inlcuding answer asked to the user. */
typedef struct QUESTION_ITEM {
    line_xlong_t question;
    line_short_t choice_1;
    line_short_t choice_2;
    line_short_t choice_3;
    line_short_t choice_4;
    char answer;
} question_item_t;

/* An enumeration that describes the days in a week. */
typedef enum DAY {
    DATE_DAY_MONDAY,
    DATE_DAY_TUESDAY,
    DATE_DAY_WEDNESDAY,
    DATE_DAY_THURSDAY,
    DATE_DAY_FRIDAY,
    DATE_DAY_SATERDAY,
    DATE_DAY_SUNDAY,
} date_day_e;

/* A struct that defines all variable prompts asked to the user. */
typedef struct prompt_list_t {
    line_xlong_t year;
    line_xlong_t month;
    line_xlong_t day;
    line_xlong_t day_str;
    line_xlong_t math;
    line_xlong_t math_teller;
    line_xlong_t math_noemer;
    line_xlong_t math_float;
    line_xlong_t math_fault;
    line_xlong_t question;
    line_xlong_t question_fault;
    line_xlong_t ok_exact;
    line_xlong_t ok_beta;
    line_xlong_t done;
    line_xlong_t err_invalid;
} prompt_list_t;

/* Informal question. */
const question_item_t question_item_informal = {
    .question = "> Welk literair werk wordt beschouwd als het eerste moderne psychologische roman?",
    .choice_1 = "a. Don Quichot van Miguel de Cervantes",
    .choice_2 = "b. De Gebroeders Karamazov van Fjodor Dostojevski",
    .choice_3 = "c. Odyssee van Homerus",
    .choice_4 = "d. De Toverberg van Thomas Mann",
    .answer = 'b',
};

/* Formal question. */
const question_item_t question_item_formal = {
    .question =
        "> Welke Franse schrijver en filosoof schreef het werk L'Etranger en won de Nobelprijs "
        "voor "
        "Literatuur in 1957?",
    .choice_1 = "a. Jean-Paul Sartre",
    .choice_2 = "b. Albert Camus",
    .choice_3 = "c. Marcel Proust",
    .choice_4 = "d. Andre Gide",
    .answer = 'b',
};

/* Informal prompts. */
const prompt_list_t prompt_list_informal = {
    .day_str = "> Welke dag ben je geboren? (m,d,w,v,z)",
    .math = "> Vind het antwoord van het volgende probleem: ",
    .math_teller = "> Geef de teller van je antwoord.",
    .math_noemer = "> Geef de noemer van je antwoord.",
    .math_float = "> Geef het antwoord met twee decimalen na de komma.",
    .math_fault = "> Jammer genoeg is het antwoord niet goed, we gaan verder.",
    .question = "> Geef hier het antwoord op de vraag. (a, b, c, d)",
    .question_fault =
        "> Helaas klopt dat niet helemaal, waarschijnlijk ben je niet geschikt voor een opleiding "
        "aan de universiteit.",
    .ok_exact = "> Helemaal goed, je bent geschikt voor een exacte studie.",
    .ok_beta = "> Helemaal goed, je bent geschikt voor een beta studie.",
    .done = "> Je bent helemaal klaar!",
    .err_invalid = "> Je gegeven informatie klopt niet helemaal, probeer het opnieuw."};

/* Formal prompts. */
const prompt_list_t prompt_list_formal = {
    .year = "> Geef je geboorte jaar.",
    .month = "> Geef de maand waarin je geboren bent.",
    .day = "> Geef de dag waarop je geboren bent.",
    .day_str = "> Verstrek de dag waarop u bent geboren. (m,d,w,v,z)",
    .math = "> Verstrek het antwoord van het volgende probleem: ",
    .math_teller = "> Geef de teller van het antwoord.",
    .math_noemer = "> Geef de noemer van het antwoord.",
    .math_float = "> Geef het antwoord met twee decimalen na de komma.",
    .math_fault =
        "> Helaas is dit fout, mogelijk is een beta studie iets voor u, de vragenlijst gaat door.",
    .question = "> Verstrek het antwoord op de vraag. (a, b, c, d)",
    .question_fault =
        "> Helaas is het antwoord fout, u bent waarschijnlijk niet geschikt voor een opleiding aan "
        "de universiteit.",
    .ok_exact = "> Helemaal goed, u bent geschikt voor een exacte studie.",
    .ok_beta = "> Helemaal goed, u bent geschikt voor een beta studie.",
    .done = "> U heeft de vragenlijst afgerond, een prettige dag gewenst.",
    .err_invalid = "> De gegeven informatie is niet valide, probeer het opnieuw."};

/* Amount of days per month from jan till dec.*/
const uint8_t day_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
/* A list of weekdays, normalized to DATE_NORM_YEAR. 1/1/1876 is a saterday.*/
const date_day_e day_list[] = {
    DATE_DAY_SATERDAY,  DATE_DAY_SUNDAY,   DATE_DAY_MONDAY, DATE_DAY_TUESDAY,
    DATE_DAY_WEDNESDAY, DATE_DAY_THURSDAY, DATE_DAY_FRIDAY,
};

int main(int argc, char **argv) {
    time_t time_now = time(NULL);
    struct tm *time_holder = {0};
    time_t time_converted;

    int input_int = 0;
    float input_float = 0;
    char input_char = 0;

    int birth_year = 0;
    int birth_month = 0;
    int birth_day = 0;

    char math_sign;
    int math_1_teller;
    int math_1_noemer;
    int math_2_teller;
    int math_2_noemer;
    int math_answer_teller;
    int math_answer_noemer;
    float math_answer_float;

    int num_leap_days = 0;
    int num_days_from_mon = 0;
    int num_days_total = 0;
    date_day_e day_str = DATE_DAY_MONDAY;

    const prompt_list_t *prompt_list = &prompt_list_formal;
    const question_item_t *question_item = &question_item_formal;

    time_holder = localtime(&time_now);
    srand(time(NULL));

    // Infoblock
    cout << "Naam:\t\t\t" << STUDENT_NAME << "\n";
    cout << "Student nummer:\t\t" << STUDENT_NUMBER << "\n";
    cout << "Laatst geupdate:\t" << DATE_UPDATED << "\n";
    cout << "Opdracht:\t\t" << OPDRACHT << "\n";
    cout << "Studie:\t\t\t" << STUDIE << "\n";
    cout << "\n";
    cout << "> Vragenlijst: Ben jij geschikt voor een studie aan de universiteit?\n";

    // Ask for age.
    cout << prompt_list->year << "\n";
    cin >> input_int;
    if (!input_int) {
        goto end_err_invalid;
    }
    birth_year = input_int;
    time_holder->tm_year = birth_year - 1900 + 10;
    time_converted = mktime(time_holder);
    if (difftime(time_converted, time_now) > 0) {
        goto end_err_young;
    }

    cout << prompt_list->month << "\n";
    cin >> input_int;
    if (!input_int || (1 < input_int && input_int > 12)) {
        goto end_err_invalid;
    }
    birth_month = input_int;
    time_holder->tm_mon = birth_month - 1;
    time_converted = mktime(time_holder);
    if (difftime(time_converted, time_now) > 0) {
        goto end_err_young;
    }

    cout << prompt_list->day << "\n";
    cin >> input_int;
    if (!input_int || !(1 <= input_int && input_int <= 31)) {
        goto end_err_invalid;
    }
    birth_day = input_int;
    time_holder->tm_mday = birth_day;
    time_converted = mktime(time_holder);
    if (difftime(time_converted, time_now) > 0) {
        goto end_err_young;
    }

    // Start asking informal questions.
    time_holder->tm_year = time_holder->tm_year - 10;
    time_converted = mktime(time_holder);
    if (int(difftime(time_now, time_converted) / 31556925.9936) < LIMIT_FORMAL_AGE) {
        prompt_list = &prompt_list_informal;
    }

    // Calculate day of birthday.
    num_leap_days = int((birth_year - DATE_NORM_YEAR) / 4);
    if (((float)birth_year / 4 - int(birth_year / 4)) == 0.0 &&
        (birth_month <= 2))  // Correction if birthyear is leap year.
    {
        num_leap_days--;
    }
    for (int i = 0; i < birth_month - 1; i++) {
        num_days_from_mon += day_month[i];
    }
    num_days_total = (birth_year - DATE_NORM_YEAR) * DAYS_IN_YEAR + num_days_from_mon + birth_day -
                     1 + num_leap_days;
    day_str = day_list[num_days_total % 7];

    // Ask for day of the birthday and test correctness.
    cout << prompt_list->day_str << "\n";
    cin >> input_char;
    if (!input_char) {
        goto end_err_invalid;
    }
    switch (input_char) {  // eww
        case 'z':
            cout << "> Welke dag:  za(a) of zo(o))?\n";
            cin >> input_char;
            if (!input_char) {
                goto end_err_invalid;
            }
            switch (input_char) {
                case 'a':
                    if (day_str != DATE_DAY_SATERDAY) {
                        goto end_err_day_str;
                    }
                    break;
                case 'o':
                    if (day_str != DATE_DAY_SUNDAY) {
                        goto end_err_day_str;
                    }
                    break;
            }
            break;
        case 'm':
            if (day_str != DATE_DAY_MONDAY) {
                goto end_err_day_str;
            }
            break;
        case 'd':
            cout << "> Welke dag: di(i) of do(o))?\n";
            cin >> input_char;
            if (!input_char) {
                goto end_err_invalid;
            }
            switch (input_char) {
                case 'i':
                    if (day_str != DATE_DAY_TUESDAY) {
                        goto end_err_day_str;
                    }
                    break;
                case 'o':
                    if (day_str != DATE_DAY_THURSDAY) {
                        goto end_err_day_str;
                    }
                    break;
            }
            break;
        case 'w':
            if (day_str != DATE_DAY_WEDNESDAY) {
                goto end_err_day_str;
            }
            break;
        case 'v':
            if (day_str != DATE_DAY_FRIDAY) {
                goto end_err_day_str;
            }
            break;
        default:
            goto end_err_invalid;
    }

    // Initialze math question.
    math_sign = (rand() % 2) ? '+' : '-';
    math_1_teller = rand() % 20 + 1;
    math_1_noemer = rand() % 20 + 1;
    math_2_teller = rand() % 20 + 1;
    math_2_noemer = rand() % 20 + 1;
    if (math_1_noemer == math_2_noemer) {
        math_answer_teller = math_1_teller + (math_2_teller * ((math_sign == '-') ? -1 : 1));
        math_answer_noemer = math_1_noemer;
    } else {
        math_answer_teller = math_1_teller * math_2_noemer +
                             (math_2_teller * math_1_noemer * ((math_sign == '-') ? -1 : 1));
        math_answer_noemer = math_1_noemer * math_2_noemer;
    }
    math_answer_float = (float)math_answer_teller / (float)math_answer_noemer;

    // Ask math question.
    cout << prompt_list->math;
    cout << math_1_teller << "/" << math_1_noemer << " " << math_sign << " " << math_2_teller << "/"
         << math_2_noemer << " = ?\n";
    cout << prompt_list->math_teller << "\n";
    cin >> input_int;
    if (!input_int) {
        goto end_err_invalid;
    }
    if (input_int != math_answer_teller) {
        goto end_math_fault;
    }

    cout << prompt_list->math_noemer << "\n";
    cin >> input_int;
    if (!input_int) {
        goto end_err_invalid;
    }
    if (input_int != math_answer_noemer) {
        goto end_math_fault;
    }

    cout << prompt_list->math_float << "\n";
    cin >> input_float;
    if (!input_float) {
        goto end_err_invalid;
    }
    if ((math_answer_float - MATH_ANSWER_FLOAT_LIMIT) > input_float ||
        input_float > (math_answer_float + MATH_ANSWER_FLOAT_LIMIT)) {
        goto end_math_fault;
    }

    cout << prompt_list->ok_exact << "\n";
    goto end;

end_math_fault:
    cout << prompt_list->math_fault << "\n";

    // Ask beta question.
    cout << question_item->question << "\n";
    cout << "\t" << question_item->choice_1 << "\n";
    cout << "\t" << question_item->choice_2 << "\n";
    cout << "\t" << question_item->choice_3 << "\n";
    cout << "\t" << question_item->choice_4 << "\n";
    cout << prompt_list->question << "\n";

    cin >> input_char;
    if (!input_char) {
        goto end_err_invalid;
    }
    if ((input_char | 0x20) != question_item->answer) {
        cout << prompt_list->question_fault;
        cout << "Het juiste antwoord was " << question_item->answer << ".\n";
    } else {
        cout << prompt_list->ok_beta << "\n";
    }

    goto end;

end_err_young:
    cout << "! Je bent  te jong om deze vragenlijst in te vullen.\n";
    return 1;

end_err_day_str:
    cout << "! De opgegeven dag is niet valide, gegevens verwijderd!\n";
    return 1;

end_err_invalid:
    cout << "! De gegeven informatie is niet valide, probeer het opnieuw.\n";
    return 1;

end:
    cout << prompt_list->done << "\n";
    return 0;
}