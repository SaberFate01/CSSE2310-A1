/* uqwordiply.c
 * ---------------
 * Version 1.2
 * written by YunFei Pei S4716841
 * 220323 22:25PM 
 * for Assessment 1 CSSE2310
 * Note: I renamed this file from UQWordiply.c to uqwordiply.c on revision 21
 * I mentioned it in the commit comments, please check UQWordiply.c on svn
 * if you would like to know my older versions.
 *
 * Thank you for reading.
 */

// Include all necessary libraries
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <csse2310a1.h>
#include <ctype.h>

// Define all the macros
#define MAX_DICTIONARY_SIZE 1000000
#define MAX_WORD_LENGTH 100
#define MAX_GAMES 5

// Defining all the global variables
// The default path for dictionary.
char* path = "/usr/share/dict/words";
//The global variable for starter word.
const char* word;
//The global variable for storing all the words of the dictionary
char* wordArray[MAX_DICTIONARY_SIZE];
//The global variable for the length of starter word.
int len;
//The global variable for checking if the command line includes 
//--start or --len
bool hasStartOrLen = false;
//The global variable for checking if the command line includes --dictionary
bool hasDict = false;
//The global variable for total length of words by user.
int totalLength = 0;
//The global ariable for the longest valid word by user
int longestInputLength = 0;
//A global variable to check if the game has concluded.
bool gameFinished = false;
//Set the buffer for the user input.
char buffer[50];

// Define starter word function from csse2310a1.h:
const char* get_wordiply_starter_word(unsigned int wordLen);

/* check_string()
 * -------------
 * Function takes a char word and test if it contains any numbers or spaces 
 * or the number of letters is neither 3 nor 4, prompts a warning if so.
 *
 * word[]: a char inputted by the user that we are going to check
 *
 * returns: 0 if everything is alright, force quit by 2 if digits, space 
 * detected, or invalid starter length.
 *
 * REF: https://stackoverflow.com/questions/
 * 30695064/checking-if-string-is-only-letters-and-spaces
 */
int check_string(const char word[])
{
    int wordLength = strlen(word);
    while (*word) {
        if (isdigit(*word)) {
            fprintf(stderr, "uqwordiply: invalid starter word\n");
            exit(2);
        } else if (isspace(*word)) {
            fprintf(stderr, "uqwordiply: invalid starter word\n");
            exit(2);
        }
        word++;
    }
    if (wordLength != 3 && wordLength != 4) {
        fprintf(stderr, "uqwordiply: invalid starter word\n");
        exit(2);
    }
    return 0;
}

/* set_start_word()
 * --------------
 * A simple function used to set the starting word 
 * if the user used --start command. Update all the letters to caps,
 * update the global variable to the word by the user
 *
 * char startWord[]: the startword entered by the user
 *
 * returns 0 if everything goes alright
 */
int set_start_word(char startWord[])
{
    check_string(startWord);
    int i;
    for (i = 0; i < strlen(startWord); i++) {
        startWord[i] = toupper(startWord[i]);
    }
    len = i;
    word = startWord;
    return 0;
}

/* set_len()
 * --------
 *  A function used to set the len of a starting word 
 *  if the user used --len function. If the user entered a number that is 
 *  neither 3 nor 4, prompt an error. And in the end call the 
 *  get_wordiply_starter_word function to create a new starter word.
 *
 *  char startLen[]: the length of the starter entered by the user
 *
 *  returns 0 if everything goes alright, force exit by 1 if the user 
 *  entered a wrong len
 */
int set_len(char startLen[])
{
    len = atoi(startLen);
    if (len != 3 && len != 4) {
        fprintf(stderr, "Usage: uqwordiply [--start starter-word |"
		" --len length] [--dictionary filename]\n");
        exit(1);
    }
    word = get_wordiply_starter_word(len);
    return 0;
}

/* set_path()
 * ---------
 *  A function that checks if the path by user can
 *  be read, if can, change the default path, and gives a new starter word
 *  if the user did not assign one.
 *
 *  char startPath[] the path indicated by user
 *
 *  returns 0 if everything goes alright
 *  REF:https://stackoverflow.com/questions/
 *  11772291/how-can-i-print-a-quotation-mark-in-c
 */
int set_path(char startPath[])
{
    FILE* checkPath = fopen(startPath, "r");
    if (!checkPath) {
        fprintf(stderr, "uqwordiply: dictionary file \"%s\" "
		"cannot be opened\n", startPath);
        exit(3);
    } else {
        path = startPath;
        if (hasStartOrLen == false) {
            word = get_wordiply_starter_word(len);
        }
        fclose(checkPath);
    }
    return 0;
}

/*
 * check_command()
 * --------------
 * Checks the command input by the user, and stores the record of any command
 * entered by the user, prints error if the user entered invalid argument for
 * the three commands, invalid commands, or tried to enter both --start and 
 * --len.
 *
 * argc, argv[]: the argc and argv from main()
 *
 * Return: 0 if everything is alright, 
 * Force exit the program by 1
 * if the user enters a wrong command
 *
 * REF: https://stackoverflow.com/questions/
 * 17846212/generate-a-random-number-between-1-and-10-in-c
 */
int check_command(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--start") == 0 && i + 1 < argc) {
            if (hasStartOrLen == false){
                set_start_word(argv[i + 1]);
                i++;
                hasStartOrLen = true;
            } else {
                fprintf(stderr, "Usage: uqwordiply [--start starter-word |"
			" --len length] [--dictionary filename]\n");
                exit(1);
            }
        } else if (strcmp(argv[i], "--len") == 0 && i + 1 < argc) {
            if (hasStartOrLen == false) {
                set_len(argv[i + 1]);
                hasStartOrLen = true;
                i++;
            } else {
                fprintf(stderr, "Usage: uqwordiply [--start starter-word |"
			" --len length] [--dictionary filename]\n");
                exit(1);
            }
        } else if (strcmp(argv[i], "--dictionary") == 0 && i + 1 < argc) {
            if (hasDict == true) {
                fprintf(stderr, "Usage: uqwordiply [--start starter-word |"
			" --len length] [--dictionary filename]\n");
                exit(1);
            } else {
                set_path(argv[i + 1]);
                hasDict = true;
                i++;
            }
        } else {
            fprintf(stderr, "Usage: uqwordiply [--start starter-word |"
		    " --len length] [--dictionary filename]\n");
            exit(1);
        }
    }
    return 0;
}

/*store_array();
 *------------
 * A function to read through the entire library using getline(), and store
 * The words into the array wordArray by a loop
 * dynamically allocate memories for the array, and saves the words
 * in the dictionary into the array using strcpy
 * close the file when the file is finished
 *
 * REF: After consulting the tutor during my practical session
 * REF: Ed lessons 3.2 Read from file system
 * REF: https://stackoverflow.com/questions/35144153/
 * trying-to-read-list-of-words-from-file-and-store-into-an-array-in-c
 */
int store_array()
{
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Error: Could not open file\n");
        return 4;
    }
    char* line = NULL;
    size_t len = 0;
    int index = 0;
    while (getline(&line, &len, file) != -1 && index < MAX_DICTIONARY_SIZE) {
        line[strcspn(line, "\n")] = 0;
        wordArray[index] = malloc(sizeof(char) * (strlen(line) + 1));
        strcpy(wordArray[index], line);
        index++;
    }
    fclose(file);
    if (line) {
        free(line);
    }
    return 0;
}

/*remove_special()
 *---------------
 *Function removes all the special characters, like
 *digits and punctuations in the arrays
 *
 *returns 0 after the process is finished.
 *
 * REF: After consulting tutor from practical session
 */
int remove_special()
{
    int i = 0;
    while (wordArray[i] != NULL) {
        int j = 0;
        while (wordArray[i][j] != '\0') {
            if (isdigit(wordArray[i][j])) {
                wordArray[i] = NULL;
                break;
            }
            if (ispunct(wordArray[i][j])) {
                wordArray[i] = NULL;
                break;
            }
            wordArray[i][j] = toupper(wordArray[i][j]);

            j++;
        }
        i++;
    }
    return 0;
}

/*check_input()
 * -----------
 *  This function checks the input by the user in function play_game()
 *  checks if the input contains digits, spaces
 *  or if it is same to starter word
 *  or the starter word is not a substring of it
 *
 *  return 0 if everything is alright, 2 if
 *  contains digits, not substring, or spaces
 *  4 if input and starter word is the same
 *
 *  REF: My earlier failed code
 *  REF: Adice from practical on 230323
 *  REF: https://stackoverflow.com/questions/39692672/
 *  how-to-check-if-a-string-is-empty-in-c-nothing-has-worked-so-far 
 *  https://stackoverflow.com/questions/2693776/
 *  removing-trailing-newline-character-from-fgets-input
 */
int check_input(char userInput[])
{
    bool onlyLetters = true;
    char* inputPntr = userInput;
    userInput[strcspn(userInput, "\n")] = '\0';
    int i = 0;
    for (i = 0; userInput[i] != '\0'; i++) {
	if (!isalpha(userInput[i])) {
	    printf("Guesses must contain only letters - try again.\n");
	    return 2;
	}
    }
    if (userInput[0] == '\n') {
	printf("Guesses must contain the starter word - try again.\n");
	return 2;
    }
    if (strcmp(inputPntr, word) == 0) {
        printf("Guesses can't be the starter word - try again.\n");
        return 4;
    } else {

        if (onlyLetters == true && !strstr(inputPntr, word)) {
            printf("Guesses must contain the starter word - try again.\n");
            return 2;
        }
    }

    return 0;
}

/* check_longest()
 * -------------
 *  A function that calls the wordArray
 *  twice in while loops, takes down the longest
 *  possible words and its length that the starter
 *  word is a substring of and print it out
 *
 *  return 0 if everything is alright
 *  REF: https://stackoverflow.com/questions/44224066/find-longest-word-in-c
 */
int check_longest()
{
    int i = 0;
    int wordLength;
    int longestLength = 0;
    char* longestWord = NULL;
    while (i < MAX_DICTIONARY_SIZE) {
        if (wordArray[i] != NULL && strstr(wordArray[i], word)) {
            wordLength = strlen(wordArray[i]);
            if (wordLength > longestLength) {
                longestLength = wordLength;
                longestWord = wordArray[i];
            }
        }
        i++;
    }
    printf("Longest word(s) possible:\n");
    if (longestWord != NULL) {
        i = 0;
        while (i < MAX_DICTIONARY_SIZE) {
            if (wordArray[i] != NULL && strstr(wordArray[i], word)) {
                wordLength = strlen(wordArray[i]);
                if (wordLength == longestLength) {
                    printf("%s (%d)\n", wordArray[i], longestLength);
                }
            }
            i++;
        }
    }

    return 0;
}

/*check_user_word_array()
 * -------------------
 *  A function that calls in an array of words by
 *  the user and the number of attempts of user, checks if the user
 *  entered a similar word
 *  and checks the longest word entered by the
 *  user with the number of letters in total
 *
 *  char** array: the array of words by the user
 *  int count: the number of attempts by user
 *
 *  returns 0 if everything is alright, return 1 if the user entered a word
 *  that he has already used. 
 *  REF: Function check_longest();
 */
int check_user_word_array(char** array, int count)
{
    int i = 0;
    while (i < count) {
        if (strcmp(array[count], array[i]) == 0) {
            printf("You've already guessed that word - try again.\n");
            return 1;
        }
        int wordLength = strlen(array[i]);
        if (array[i] != NULL && wordLength > longestInputLength) {
            longestInputLength = wordLength;
        }
        i++;
    }
    if (gameFinished == true || count == 4) {
        if (count == 4) {
            gameFinished = true;
            count = 5;
        }
        int k = 0;
        while (k < count) {
            int wordLength = strlen(array[k]);
            totalLength += wordLength;
            k++;
        }
        printf("\nTotal length of words found: %d\n", totalLength);
        printf("Longest word(s) found:\n");
        int j = 0;
        while (j < count) {
            int wordLength = strlen(array[j]);
            if (array[4] != NULL && strlen(array[4]) > longestInputLength) {
                printf("%s (%ld)\n", array[4], strlen(array[4]));
                longestInputLength = strlen(array[4]);
                break;
            } else {
                if (wordLength == longestInputLength) {
                    printf("%s (%d)\n", array[j], wordLength);
                }
            }
            j++;
        }
    }
    return 0;
}

/* play_game()
 * ----------
 * This function, as its name suggests, plays the game.
 * It asks for an user input with scanf()
 * Then converts it to upper case, and checks if
 * the designated word is a substring of it.
 * Checks if the input has already been used.
 * And then it loops through the entire dictionary
 * to check if there is a word that is equal to
 * the input.
 * The loop terminates if the user enters ctrl + D standing for EOF
 *
 * Returns 0 when finished, 4 if the user terminates without correct answwer
 * REF: My previous failed code
 * REF for inputArray: https://stackoverflow.com/questions/19690143/
 * why-do-i-always-get-the-last-elements-of-my-string-array
 * REF for styling to cut down the number of lines used by ChatGPT
 */
int play_game()
{
    char* inputArray[5];
    char userInput[MAX_WORD_LENGTH];
    int checked, correct = 0,count = 1;
    printf("The starter word is: %s\n", (word));
    printf("Enter words containing this word.\n");
    while (correct < MAX_GAMES) {
	printf("Enter guess %d:\n", correct + 1); 
	if (fgets(userInput, sizeof(userInput),stdin) == NULL){
	    gameFinished = true;
	    if (correct > 0) { 
		check_user_word_array(inputArray, correct); 
	    } 
	    break;
	}
	for (int j = 0; j < strlen(userInput); j++) {
            userInput[j] = toupper(userInput[j]);
        }
        checked = check_input(userInput);
        if (checked == 2 || checked == 4) {
            continue;
        }
        inputArray[correct] = malloc(strlen(userInput) + 1);
        strcpy(inputArray[correct], userInput);
        if (check_user_word_array(inputArray, correct) == 1) {
            continue;
        }
	int i = 0;
	while (i < MAX_DICTIONARY_SIZE) {
	    if (wordArray[i] != NULL&& strcmp(userInput, wordArray[i]) == 0) {
		correct++;
		if (correct > 4) {
		    gameFinished = true;
		}
		break;
	    }
	    i++;
	}
	if (i == MAX_DICTIONARY_SIZE && correct != count) {
	    printf("Guess not found in dictionary - try again.\n");
	}  
    }
    gameFinished = true;
    count++;
    if (correct > 0) {
        check_longest();
    } else {
        exit(4);
    }
    return 0;
}

/* main()
    * --------
    * The main function that calls all the necessary functions and make the
    * program work
    *
    * argc, argv[], the default required input for main
    *
    * returns 0 if everything goes alright
    * frees the memory allocated to the array before leaving.
    */
int main(int argc, char* argv[])
{
    if (argc == 1) {
        len = (rand() % 2) + 3;
        word = get_wordiply_starter_word(len);
    } else {
        check_command(argc, argv);
    }    
    store_array();
    remove_special();
    fprintf(stdout, "Welcome to UQWordiply!\n");
    play_game();
    for (int i = 0; i < MAX_DICTIONARY_SIZE; i++) {
        free(wordArray[i]);
    }
    return 0;
}
